"""
Module ATP
Encode et decode les messages ATP à l’aide du module « struct »
"""

import binascii
import struct
import sys
import time
from threading import Thread
from logging import getLogger

FORMATS = { 1 : '<B', 2 : '<H', 4 : '<I', 17 : '<b', \
        18 : '<h', 20 : '<i', 36 : '<f' }

def async(fcnt):
    """
    Décorateur pour lancer dans un nouveau thread une fonction.
    """
    def run(*k, **kw):
        t = Thread(target=fcnt, args=k, kwargs=kw)
        t.start()
        return t

    return run

def encode(stream, id, args):
    logger = getLogger("atp.encode")
    buf = bytearray()
    buf += struct.pack('B', 129)
    buf += struct.pack('B', int(id))
    for value, form in args:
        code = None
        for c, f in FORMATS.items():
            if form == f[1]:
                code = c
                break
        if code == None:
            logger.warning("unknow type (%c), assuming unsigned char" %form)
            buf += struct.pack('B', 128)
            return
        if form == 'f':
            value = float(value)
        else:
            value = int(value)
        buf += struct.pack('B', code)
        buf += struct.pack('<'+form, value)
    buf += struct.pack('B', 128)
    stream.write(buf)
    stream.flush()

@async
def decode(stream, callback, follow = False):
    """
    Décode les messages ATP.
    Si follow = False, appelle callback avec id = -1 lorsque la fin du flux est
    atteinte.
    Si follow = True, reessaye de lire le flux après une temporisation lorsque
    la fin est atteinte.
    """

    logger = getLogger("atp.decode")
    data = b''
    expected = 'begin'
    length = 1
    form = ''
    paquet_id = 0
    #errors = 0
    args = []

    while True:
        pending = stream.read(1)
        if not len(pending):
            if follow:
                time.sleep(0.1)
                continue
            else:
                callback(-1, [])
                sys.exit()
        data += pending
        while len(data) >= length:
            if expected == 'begin':
                pending = data[0]
                data = data[1:]
                if pending == 129: # Début de trame
                    expected = 'id'
                else:
                    logger.warning('expected beginning flag (%d)' %pending)
                    #errors += 1
            elif expected == 'id':
                pending = data[0]
                data = data[1:]
                paquet_id = pending
                args = []
                expected = 'type'
            elif expected == 'type':
                pending = data[0]
                data = data[1:]
                if pending == 128: # Fin de trame
                    callback(paquet_id, args)
                    expected = 'begin'
                elif pending == 132: # timestamp
                    form = FORMATS[4]
                    length = pending & 0b1111
                    value = b''
                    expected = 'data'
                elif pending == 148: # microseconds
                    form = FORMATS[4]
                    length = pending & 0b1111
                    value = b''
                    expected = 'data'
                else:
                    try:
                        form = FORMATS[pending]
                    except KeyError:
                        logger.warning('unknow format (%d)" \
                                ", waiting next flag' %pending)
                        #errors += 1
                        expected = 'begin'
                    length = pending & 0b1111
                    value = b''
                    expected = 'data'
            elif expected == 'data':
                try:
                    value = struct.unpack(form, data[0:length])
                except struct.error:
                    logger.warning("struct.unpack exception" \
                            ", ignoring argument %s of format %s" \
                            %(binascii.hexlify(data[0:length]), form))
                else:
                    args.append(value[0])
                finally:
                    data = data[length:]
                    expected = 'type'
                    length = 1
