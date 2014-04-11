#!/usr/bin/env python3

from atp import encode
import sys

if len(sys.argv) < 2:
    print('Usage: [id] [value@type] [value@type] …', file=sys.stderr)
    exit()

encode(sys.stdout.buffer, sys.argv[1],
        list(map(lambda x: x.split('@'), sys.argv[2:])))
