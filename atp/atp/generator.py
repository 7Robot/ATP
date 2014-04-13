#!/usr/bin/env python3

import os
from string import Template

from atp import FORMATS as formats
from atp.protos import load, cformats


template_c = """// Generated from version ${VERSION} of semantic

#include "atp.h"

${FUNCTIONS}

int AtpDecode(int id,
        unsigned char *ucharv, int ucharc,
        unsigned int *ushortv, int ushortc,
        unsigned long int *uintv, int uintc,
        char *charv, int charc,
        int *shortv, int shortc,
        long int *intv, int intc,
        float *floatv, int floatc)
{${DECODERS}
    return 0;
}

"""

template_decoder = """
    if (id == ${ID}) {
        ${FUNCTION}(${ARGV});
        return 1;
    }"""

template_h = """// Generated from version ${VERSION} of semantic

#ifndef _${PROTO_UPPER}_H_
#define _${PROTO_UPPER}_H_

#define BOARD_ID ${ID}
#define BOARD_NAME ${PROTO}

${PROTOTYPES}

int AtpDecode(int id,
        unsigned char *ucharv, int ucharc,
        unsigned int *ushortv, int ushortc,
        unsigned long int *uintv, int uintc,
        char *charv, int charc,
        int *shortv, int shortc,
        long int *intv, int intc,
        float *floatv, int floatc);

#endif // _${PROTO_UPPER}_H_
"""

template_encoder_c = """
void ${FUNCTION}(${ARGS}) {
    char bytes[] = {
        129,
        ${ID},${CODE}
        128
    };
    SendBytes(bytes, ${COUNT});
}
"""

template_arg = """
        ${ARG},"""

template_long_arg = """
        ((char*)&${ARG})[${I}],"""

template_encoder_h = """
void ${FUNCTION}(${ARGS});
"""

template_decoder_c = """
// You should redefine this function
__attribute__((weak)) void ${FUNCTION}(${ARGS}) {${CODE}}
"""

template_decoder_h = """
void ${FUNCTION}(${ARGS});
"""

class PicGenerator:

    arrayname = { 'B': 'ucharv',
            'H': 'ushortv',
            'I': 'uintv',
            'b': 'charv',
            'h': 'shortv',
            'i': 'intv',
            'f': 'floatv' }

    def __init__(self, semantic, dest):

        if not os.path.exists(dest):
            print("Creating directory '%s'" %dest)
            os.makedirs(dest)
        self.dest = dest

        self.protos = semantic

        self.formats = dict((v[1],k) for k, v in formats.items())

    def genAll(self):
        for proto_name in self.protos:
            proto = self.protos[proto_name]
            self.genProto(proto_name, proto)

    def genProto(self, proto_name, proto):
        print("Generating files for '%s'..." %proto_name)

        cfile = open("%s/atp-%s.c" %(self.dest, proto_name.lower()), 'w')
        hfile = open("%s/atp-%s.h" %(self.dest, proto_name.lower()), 'w')

        params = {}
        params['VERSION'] = proto["version"]
        params['PROTO'] = proto_name
        params['PROTO_UPPER'] = proto_name.upper()
        params['ID'] = proto['id']
        params['PROTOTYPES'] = ''
        params['FUNCTIONS'] = ''
        params['DECODERS'] = ''
        
        for packet_name in proto['packets']:
            packet = proto['packets'][packet_name]
            c, h, d = self.genPacket(packet_name, packet)
            params['FUNCTIONS'] += c
            params['PROTOTYPES'] += h
            params['DECODERS'] += d

        cfile.write(Template(template_c).substitute(params))
        hfile.write(Template(template_h).substitute(params))

    def genPacket(self, packet_name, packet):
        c = ''
        h = ''
        d = ''
        if packet['transmitter'] == 'arm' or packet['transmitter'] == 'both':
            _c, _h, _d = self.genDecoder(packet_name, packet)
            c += _c
            h += _h
            d += _d
        if packet['transmitter'] == 'pic' or packet['transmitter'] == 'both':
            _c, _h = self.genEncoder(packet_name, packet)
            c += _c
            h += _h
        return (c, h, d)

    def genDecoder(self, packet_name, packet):
        params = {}
        params['ID'] = packet['id']
        params['FUNCTION'] = 'On' + packet_name[0].upper() + packet_name[1:]
        params['ARGS'] = ', '.join(map(lambda x: cformats[packet['args'][x]]+' '+x, packet['args']))
        params['ARGV'] = ''

        counts = dict(zip(self.formats,map(lambda x: 0, self.formats)))
        for arg in packet['args']:
            type = packet['args'][arg]
            params['ARGV'] += self.arrayname[type] + "[%d], " %counts[type]
            counts[type] += 1
        params['ARGV'] = params['ARGV'][:-2]

        if packet_name == 'getId':
            params['CODE'] = ' SendBoardId(); '
        elif packet_name == 'test':
            params['CODE'] = ' SendTest(B, H, I, b, h, i, f); '
        else:
            params['CODE'] = ' SendUnimplemented(); '

        print("\tdecoder '%s'" %params['FUNCTION'])
        c = Template(template_decoder_c).substitute(params)
        h = Template(template_decoder_h).substitute(params)
        d = Template(template_decoder).substitute(params)
        return (c, h, d)

    def genEncoder(self, packet_name, packet):
        params = {}
        params['FUNCTION'] = 'Send' + packet_name[0].upper() + packet_name[1:]
        params['ARGS'] = ', '.join(map(lambda x: cformats[packet['args'][x]]+' '+x, packet['args']))
        params['ID'] = packet['id']
        params['COUNT'] = 3
        params['CODE'] = ''
        for arg in packet['args']:
            count, code = self.genArg(arg, packet['args'][arg])
            params['COUNT'] += count
            params['CODE'] += code
        print("\tencoder '%s'" %params['FUNCTION'])
        c = Template(template_encoder_c).substitute(params)
        h = Template(template_encoder_h).substitute(params)
        return (c, h)

    def genArg(self, arg, format):
        format = self.formats[format]
        length = format & 0b1111
        code = Template(template_arg).substitute({'ARG': format})
        if length == 1:
            code += Template(template_arg).substitute({'ARG': arg})
        else:
            for i in range(0, length):
                code += Template(template_long_arg).substitute({'ARG': arg, 'I': i})
        return (length+1, code)
