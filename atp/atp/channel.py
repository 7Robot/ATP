from threading import Thread
from atp import encode, decode
from atp.protos import load as load_protos
import argparse
import sys
import socket
from logging import getLogger

human_formats = {
        'B': 'unsigned char',
        'H': 'unsigned short',
        'I': 'unsigned int',
        'b': 'signed char',
        'h': 'signed short',
        'i': 'signed int',
        'f': 'float',
        }

class Channel:

    # strem: file or socket for reading and writting packets
    # callback: a function call on packet reception
    # proto: semantical description of a protocol (you should use atp.protos)
    def __init__(self, stream, callback, proto_name, proto, **kwargs):

        self._logger = getLogger("comm.channel")

        transmitter = "arm"
        follow = False

        for arg in kwargs:
            if arg == "transmitter":
                if kwargs[arg] != None:
                    transmitter = kwargs[arg]
            elif arg == "follow":
                if kwargs[arg] != None:
                    follow = kwargs[arg]
            else:
                self._logger.warning("unexpected '%s' argument" %arg)

        self._stream = stream
        self._callback = callback
        self._proto_name = proto_name.lower()
        self._proto = proto

        for packet_name in self._proto['packets']:
            packet = self._proto['packets'][packet_name]
            if packet['transmitter'] == transmitter or packet['transmitter'] == 'both' or transmitter == 'both':
                self.__setattr__(packet_name, self._create_method(packet_name, packet))

        def recv(id, args):
            if id == -1:
                callback(None, args)
                return
            else:
                know_packet = False
                for packet_name in self._proto['packets']:
                    packet = self._proto['packets'][packet_name]
                    if packet['id'] == id:
                        know_packet = True
                        break
                if not know_packet:
                    self._logger.warning("unknow packet id %d" %id)
                    return
                if packet['transmitter'] == transmitter and packet['transmitter'] != 'both' and transmitter != 'both':
                    self._logger.info("ignoring %s message" %packet['transmitter'])
                    return
                if len(packet['args']) != len(args) \
                        and len(packet['args']) + 2 != len(args):
                    self._logger.warning("packet with id %d expected " \
                            "%d arguments, %d was given"
                            %(id, len(packet['args']), len(args)))
                    return
                arguments = dict(zip(packet['args'], args))
                if len(args) == len(packet['args']) + 2:
                    arguments['timestamp'] = args[-2]
                    arguments['microseconds'] = args[-1]
                self._callback(packet_name, arguments)

        thread = decode(stream, recv, follow)

    def _create_method(self, name, packet):
        def send(*args):
            if len(args) == len(packet['args']):
                self.send(name, packet, *args)
            else:
                self._logger.warning("'%s' expects %d arguments, %d given" \
                        ", packet not sended !"
                        %(name, len(packet['args']), len(args)))
        send.__doc__ = '\n'
        for arg in packet['args']:
            send.__doc__ += "\t%s: %s\n" %(arg, human_formats[packet['args'][arg]])
        return send

    def send(self, name, packet, *args):
        formats = list(map(lambda x: packet['args'][x], list(packet['args'])))
        encode(self._stream, packet['id'], list(zip(args, formats)))


def print_packet(name, args):
    print("[%s]" %name)
    for arg in args:
        print("\t%s:" %arg, args[arg])
