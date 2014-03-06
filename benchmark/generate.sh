#!/bin/sh

export TOP_DIR=/home/tristanvdb/projects/acc2ocl/

export OPENACC_INC_PATH=$TOP_DIR/libopenacc/include

export OPENCL_INC_PATH=/usr/include

export TESTS_PATH=$TOP_DIR/tests

export COMPILER=$TOP_DIR/rose-build/projects/openacc-to-opencl/compiler/openacc-to-opencl

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

for test in 101 102 103
do
  rm -rf $test
  mkdir -p $test

  cd $test
  for t0 in 0 1 16 32 64 128 256
  do
    for t1 in 0 1 16 32 64 128 256
    do
      for t2 in 0 1 16 32 64 128 256
      do
        if isValidTiling $t0 $t1 $t2
        then
          mkdir -p $t0-$t1-$t2
          cd $t0-$t1-$t2

          $COMPILER $TOP_DIR/tests/101/klt.lt $TOP_DIR/libopenacc/include/ /usr/include/ $t0 $t1 $t2

          echo >> host-data.c
          echo "unsigned tiles[4] = {$t0, $t1, $t2, 1};" >> host-data.c
          echo "unsigned test_id = $test;" >> host-data.c
          echo >> host-data.c

          cd ..
        fi
      done
    done
  done
  cd ..
done

