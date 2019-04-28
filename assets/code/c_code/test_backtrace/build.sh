#!/bin/bash

set -ex

gcc -g -Wall -rdynamic test_backtrace.c -o Test