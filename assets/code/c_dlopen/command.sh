#!/bin/sh

set -ex

rm -rf libcalculate.so
rm -rf Test

gcc -g -Wall -m32 -fPIC -shared calculate.c -o libcalculate.so
echo "compile *.so finish"

gcc -g -Wall -m32 test.c -ldl -o Test
echo "compile test finish"
