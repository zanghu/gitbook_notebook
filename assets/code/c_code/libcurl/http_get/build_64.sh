#!/bin/sh

set -ex

gcc -g -Wall -m64 -L../../../../build/lib_so test_http_get.c buffer.c -lcurl -o Test

#export LD_LIBRARY_PATH=../../../../build/lib_so

./Test