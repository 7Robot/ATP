#!/usr/bin/env python3

from atp import decode
import sys
import fcntl
import os
import argparse


def print_packet(id, args):
    print("[%d]" %id)
    for i in args:
        print("\t", i)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Decode ATP packets')
    parser.add_argument('-f', '--follow', dest='follow', action='store_true', help='output appended data as the file grows')
    args = parser.parse_args()
    decode(sys.stdin.buffer, print_packet, args.follow)
