#!/bin/sh

set -ex

# 选择需要编译运行的测试用例
if [ $1 = "32" ]
then
  gcc -g -Wall -m32 test_curl_post.c buffer.c -lcurl -I$CURL_INC_DIR -L$CURL_LIB_DIR -o Test
  export LD_LIBRARY_PATH=$CURL_LIB_DIR
elif [ $1 = "64" ]
then
  gcc -g -Wall -m64 test_curl_post.c -lcurl -o Test
else
  gcc -g -Wall test_curl_post.c -lcurl -o Test
fi

echo $LD_LIBRARY_PATH

./Test