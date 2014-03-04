#!/bin/sh

export TOP_DIR=/media/ssd/projects/currents/openacc-to-opencl-compiler

export OPENACC_INC_PATH=$TOP_DIR/libopenacc/include
export OPENACC_LIB_PATH=$TOP_DIR/libopenacc/lib

export SQLITE_INC_PATH=/media/ssd/lib/sqlite/include
export SQLITE_LIB_PATH=/media/ssd/lib/sqlite/lib

export OPENCL_INC_PATH=/usr/include
export OPENCL_LIB_PATH=/usr/lib

export TESTS_PATH=$TOP_DIR/tests


export CC=gcc
export CFLAGS="-g -fno-stack-protector"

export LD=g++
export LDFLAGS="-L$OPENCL_LIB_PATH -lOpenCL -L$OPENACC_LIB_PATH -lopenacc -L$SQLITE_LIB_PATH -lsqlite3 -lrt"

isValidTiling() {
  if [ "$1" -gt 0 ] && [ "$2" -gt 0 ] && [ "$3" -eq 0 ]
  then
    return 0;
  elif [ "$1" -gt 0 ] && [ "$2" -eq 0 ] && [ "$3" -gt 0 ]
  then
    return 0;
  elif [ "$1" -eq 0 ] && [ "$2" -gt 0 ] && [ "$3" -gt 0 ]
  then
    return 0;
  else
    return 1;
  fi
}

$CC $CFLAGS -I$OPENACC_INC_PATH -I$OPENCL_INC_PATH -I$SQLITE_INC_PATH -c eval-100.c -o eval-100.o

for test in 101 102 103
do
  cd $test

  $CC $CFLAGS -I$OPENACC_INC_PATH -I$OPENCL_INC_PATH -c $TOP_DIR/tests/$test/init.c -o init.o
  $CC $CFLAGS -I$OPENACC_INC_PATH -I$OPENCL_INC_PATH -c $TOP_DIR/tests/$test/kernel-libopenacc.c -o kernel-libopenacc.o

  for t0 in 0 1 16 32 64 128 256
  do
    for t1 in 0 1 16 32 64 128 256
    do
      for t2 in 0 1 16 32 64 128 256
      do
        if isValidTiling $t0 $t1 $t2
        then
          cd $t0-$t1-$t2

          $CC $CFLAGS -I$OPENACC_INC_PATH -I$OPENCL_INC_PATH -c host-data.c -o host-data.o

          $LD $LDFLAGS ../../eval-100.o ../init.o ../kernel-libopenacc.o host-data.o -o eval

          cd ..
        fi
      done
    done
  done
  cd ..
done