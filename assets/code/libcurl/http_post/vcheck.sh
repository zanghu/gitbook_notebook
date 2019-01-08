#!/bin/sh

set -ex

# 内存检查
valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes ./Test
#valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes --log-file=vcheck.log ./Test
#valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes --log-file=vcheck.log --xml-file=vcheck.xml --xml=yes ./Test

# 死锁检查
#valgrind --tool=helgrind --log-file=vcheck_lock.log ./Test