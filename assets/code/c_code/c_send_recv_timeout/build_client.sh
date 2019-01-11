#!/bin/sh

set -ex

gcc -m32 -g -Wall test_send.c client_socket.c -o Test_send
gcc -m32 -g -Wall test_recv.c client_socket.c -o Test_recv