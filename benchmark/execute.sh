#!/bin/sh

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

rm -rf $1

./db-tools/init_db $1

for test in 101 102 103
do
  cd $test

  for t0 in 0 1 16 32 64 128 256
  do
    for t1 in 0 1 16 32 64 128 256
    do
      for t2 in 0 1 16 32 64 128 256
      do
        if isValidTiling $t0 $t1 $t2
        then
          cd $t0-$t1-$t2

          ./eval ../../$1 "test" 6 2 8 32 1 1 6 134217728

          cd ..
        fi
      done
    done
  done
  cd ..
done