from ctypes import *
from ctypes.util import find_library
from sys import exit

def jsread(path, callback, _verbose = False):

    if _verbose:
        verbose = 1
    else:
        verbose = 0

    def converter(a_count, a, b_count, b):

        axes = []
        for i in range(0, a_count):
            axes.append(a[i])

        buttons = []
        for i in range(0, b_count):
            buttons.append(b[i])

        callback(axes, buttons)

        return 0

    jsread = cdll.LoadLibrary(find_library("jsread"))
    if not hasattr(jsread, "jsread"):
        print("Fatal error: can't found jsread library.")
        exit(1)

    CMPFUNC = CFUNCTYPE(c_int, c_int, POINTER(c_int), c_int, POINTER(c_byte))
    cb = CMPFUNC(converter)

    jsread.jsread(path.encode('utf-8'), cb, verbose)

