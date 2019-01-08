#!/bin/sh

set -ex

gcc -g -Wall -m32 -L../../../../build/lib_so test_http_get.c -lcurl -o Test

export LD_LIBRARY_PATH=../../../../build/lib_so

./Test