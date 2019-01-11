#!/bin/sh

set -ex

# 选择需要编译运行的测试用例

# 无参数的情况下默认使用Test作为可执行文件名 
if [[ x"$1" = x ]]
then
    valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes ./Test
# 有参数情况下默认第一个参数是可执行文件名
else [[ $1 = "b4" ]]
    valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes ./$1
fi

# 内存检查
#valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes ./Test
#valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes --log-file=vcheck.log ./Test
#valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes --track-origins=yes --log-file=vcheck.log --xml-file=vcheck.xml --xml=yes ./Test

# 死锁检查
#valgrind --tool=helgrind --log-file=vcheck_lock.log ./Test