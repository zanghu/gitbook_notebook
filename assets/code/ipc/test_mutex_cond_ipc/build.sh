#!/bin/sh

set -ex

gcc -g -Wall test.c protected_shm.c -pthread -o Test