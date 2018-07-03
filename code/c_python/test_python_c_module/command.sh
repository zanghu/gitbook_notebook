#!/bin/sh

set -ex

if [ $# -nq 1 ]
then
  echo "legal number of params is 1, but now is $#"
  exit 1
else
  echo "ok, param number is $#"
fi

#VALGRIND_CHECK="valgrind --tool=memcheck --track-fds=yes --leak-check=full --show-leak-kinds=all --undef-value-errors=yes ./Test"

#BUILD_COMMAND="g++ -Wall -g test.cpp -I $ANACONDA3_HOME/include -L $ANACONDA3_HOME/lib -lpython3.6m -o Test"
BUILD_COMMAND="g++ -Wall -g test.cpp -I $ANACONDA3_HOME/include  -Wl,-rpath,$ANACONDA3_HOME/lib -lpython3.6m -o Test"

#LIB_COMMAND="g++ -fPIC -shared -Wall -g test.cpp -I $ANACONDA3_HOME/include -L $ANACONDA3_HOME/lib -lpython3.6m -o cmathapi.so"
LIB_COMMAND="g++ -fPIC -shared -Wall -g test.cpp -I $ANACONDA3_HOME/include -Wl,-rpath,$ANACONDA3_HOME/lib -lpython3.6m -o cmathapi.so"

if [ "$1" = build ]
then
  $BUILD_COMMAND
elif [ "$1" = lib ]
then
  $LIB_COMMAND
else
  echo "unknow command param $1, error"
  exit 1
fi
