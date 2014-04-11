#!/bin/bash

TEXT_FIRST=1300
TEXT_LAST=1300

ATP_FIRST=1301
ATP_LAST=1305

for port in $(seq $TEXT_FIRST $TEXT_LAST); do
    setsid tcphub $port text </dev/null >/dev/null 2>&1 &
done
for port in $(seq $ATP_FIRST $ATP_LAST); do
    setsid tcphub $port atp </dev/null >/dev/null 2>&1 &
done
