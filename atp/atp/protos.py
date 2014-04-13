import inspect
from collections import OrderedDict
import importlib.abc
import configparser

import imp as _imp
import os as _os 
import sys

cformats = { 'B' : 'unsigned char',
            'H' : 'unsigned int',
            'I' : 'unsigned long int',
            'b' : 'char',
            'h' : 'int',
            'i' : 'long int',
            'f' : 'float' }

class Proto:
    pass

class Packet:
    def __init__(self, id, transmitter = "both", attrs = []):
        self.id = id
        if transmitter not in [ "pic", "arm", "both" ]:
            print("Warning: transmitter must be 'pic', 'arm' or 'both'. "
                "Assume 'both'.", file=sys.stderr)
            self.transmitter = "both"
        else:
            self.transmitter = transmitter
        self.attrs = attrs


def import_file(fpath):
    ''' 
    fpath - the relative or absolute path to the .py file which is imported.
    
    Returns the imported module.
    
    NOTE: if import_file is called twice with the same module, the module is reloaded.
    '''
    if hasattr(_os, 'getcwdu'):
        # python 2 returns question marks in os.path.realpath for
        # ascii input (eg '.').
        original_path = _os.path.realpath(_os.getcwdu())
    else:
        original_path = _os.path.realpath(_os.path.curdir)
    dst_path = _os.path.dirname(fpath)
    if dst_path == '': 
        dst_path = '.' 
    
    # remove the .py suffix
    script_name = _os.path.basename(fpath)
    if script_name.endswith('.py'):
        mod_name = script_name[:-3]
    else:
        # packages for example.
        mod_name = script_name
    
    _os.chdir(dst_path)
    fhandle = None
    try:
        tup = importlib.abc.find_module(mod_name, ['.'])
        module = importlib.load_module(mod_name, *tup)
        fhandle = tup[0]
    finally:
        _os.chdir(original_path)
        if fhandle is not None:
            fhandle.close()
    
    return module

def load_packet(packet):
    p = OrderedDict()

    p['id'] = packet.id
    p['transmitter'] = packet.transmitter
    p['args'] = OrderedDict()
    for arg, type in packet.attrs:
        p['args'][arg] = type

    return p

def load_proto(version, common, proto):

    p = OrderedDict()

    p['version'] = version
    p['id'] = proto.type
    p['packets'] = OrderedDict()

    for name, packet in inspect.getmembers(common,
            lambda x: isinstance(x, Packet)):
        p['packets'][name] = load_packet(packet)

    for name, packet in inspect.getmembers(proto,
            lambda x: isinstance(x, Packet)):
        p['packets'][name] = load_packet(packet)

    return p

def load(semantic_file):

    f = open(semantic_file)
    source = f.read()
    code = compile(source, semantic_file, 'exec')
    # don't ask, it works
    l = { "Proto": Proto, "Packet": Packet } # locals
    g = { "Proto": Proto, "Packet": Packet } # globals
    exec(code, g, l)

    protos = OrderedDict()
    for name in l:
        if inspect.isclass(l[name]) and issubclass(l[name], Proto):
            if name != "Proto" and name != "Common":
                protos[name] = load_proto(l["version"], l["Common"], l[name])

    return protos
