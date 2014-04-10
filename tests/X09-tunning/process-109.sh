#!/bin/bash

echo "# run_id,version_id,gang,worker,n,m,p,comp_time,total_time" > $1.xeonphi.csv
echo "# run_id,version_id,gang,worker,n,m,p,comp_time,total_time" > $1.gtx460.csv
echo "# run_id,version_id,gang,worker,n,m,p,comp_time,total_time" > $1.any.csv
> $1.errors.log

for experiment in `sqlite3 $1.db "select rowid,version_id,gang,worker,n,m,p from Runs"`
do

  echo "******************************************************"
  echo "experiment=$experiment"
  echo "------------------------------------------------------"

  run_id=`echo $experiment | awk 'BEGIN{FS="|"}{print $1}'`
  version_id=`echo $experiment | awk 'BEGIN{FS="|"}{print $2}'`
  gang=`echo $experiment | awk 'BEGIN{FS="|"}{print $3}'`
  worker=`echo $experiment | awk 'BEGIN{FS="|"}{print $4}'`
  n=`echo $experiment | awk 'BEGIN{FS="|"}{print $5}'`
  m=`echo $experiment | awk 'BEGIN{FS="|"}{print $6}'`
  p=`echo $experiment | awk 'BEGIN{FS="|"}{print $7}'`

  acc_device=`sqlite3 $1.db "select acc_device_type from Runs where rowid=$run_id"`
  
#  version=`sqlite3 versions.db "select * from versions where id==$version_id"`
#  loop_gang=`echo $version | awk 'BEGIN{FS="|"}{print $2}'`
#  loop_worker=`echo $version | awk 'BEGIN{FS="|"}{print $3}'`
#  loop_0_tile_0=`echo $version | awk 'BEGIN{FS="|"}{print $4}'`
#  loop_0_tile_1=`echo $version | awk 'BEGIN{FS="|"}{print $5}'`
#  loop_0_tile_2=`echo $version | awk 'BEGIN{FS="|"}{print $6}'`
#  loop_1_tile_0=`echo $version | awk 'BEGIN{FS="|"}{print $7}'`
#  loop_1_tile_1=`echo $version | awk 'BEGIN{FS="|"}{print $8}'`
#  loop_1_tile_2=`echo $version | awk 'BEGIN{FS="|"}{print $9}'`

  num_events=`sqlite3 $1.db "select count(rowid) from Events where run_id=$run_id"`
  if [ $num_events -ne 4 ]
  then
    echo "$num_events/4 events recorded for run #$run_id..."
    echo "$num_events/4 events recorded for run #$run_id..." >> $1.log
    if [ "x$acc_device" == "xIntel(R) XeonPhi(TM)" ]
    then
      echo "    see: version-$version_id/exec-XEONPHI-$gang-$worker-$n-$m-$p.log"
      echo "    see: version-$version_id/exec-XEONPHI-$gang-$worker-$n-$m-$p.log" >> $1.errors.log
    elif [ "x$acc_device" == "xGeForce GTX 460" ]
    then
      echo "    see: version-$version_id/exec-GTX-460-$gang-$worker-$n-$m-$p.log"
      echo "    see: version-$version_id/exec-GTX-460-$gang-$worker-$n-$m-$p.log" >> $1.errors.log
    fi
    continue
  fi

  events_start=`sqlite3 $1.db "select min(cl_profiling_command_queued) from Events where run_id=$run_id"`
  events_end=`sqlite3 $1.db "select max(cl_profiling_command_end) from Events where run_id=$run_id"`

# echo "events_start=$events_start"
# echo "events_end=$events_end"
# echo "------------------------------------------------------"

  total_time=$(((($events_end)-($events_start))/1000000))

  comp_start=`sqlite3 $1.db "select cl_profiling_command_start from Events where run_id==$run_id and command_name=='kernel_launch'"`
  comp_end=`sqlite3 $1.db "select cl_profiling_command_end from Events where run_id==$run_id and command_name=='kernel_launch'"`

# echo "comp_start=$comp_start"
# echo "comp_end=$comp_end"
# echo "------------------------------------------------------"

  comp_time=$(((($comp_end)-($comp_start))/1000000))

  echo "$run_id on \"$acc_device\""
#  echo "            version #$version_id -> gang:$loop_gang worker:$loop_worker loop_0=($loop_0_tile_0,$loop_0_tile_1,$loop_0_tile_2) loop_1=($loop_1_tile_0,$loop_1_tile_1,$loop_1_tile_2)"
  echo "            version #$version_id"
  echo "            with gang=$gang worker=$worker n=$n m=$m p=$p"
  echo "      computation time : $comp_time ms"
  echo "      total time       : $total_time ms"

  if [ "x$acc_device" == "xIntel(R) XeonPhi(TM)" ]
  then
    echo "$run_id,$version_id,$gang,$worker,$n,$m,$p,$comp_time,$total_time" >> $1.xeonphi.csv
  elif [ "x$acc_device" == "xGeForce GTX 460" ]
  then
    echo "$run_id,$version_id,$gang,$worker,$n,$m,$p,$comp_time,$total_time" >> $1.gtx460.csv
  else
    echo "Unknown device: $acc_device"
    echo "$run_id,$version_id,$gang,$worker,$n,$m,$p,$comp_time,$total_time" >> $1.any.csv
  fi

#  for event in `sqlite3 $1.db "select * from Events where run_id=$run_id order by cl_profiling_command_queued"`
#  do
#    command_name=`echo $event | awk 'BEGIN{FS="|"}{print $3}'`
#    command_id=`echo $event | awk 'BEGIN{FS="|"}{print $4}'`
#    cl_profiling_command_queued=$((`echo $event | awk 'BEGIN{FS="|"}{print $5}'`-$events_start))
#    cl_profiling_command_submit=$((`echo $event | awk 'BEGIN{FS="|"}{print $6}'`-$events_start-$cl_profiling_command_queued))
#    cl_profiling_command_start=$((`echo $event | awk 'BEGIN{FS="|"}{print $7}'`-$events_start-$cl_profiling_command_submit))
#    cl_profiling_command_end=$((`echo $event | awk 'BEGIN{FS="|"}{print $8}'`-$events_start-$cl_profiling_command_start))
#
#    echo "            $command_name $cl_profiling_command_queued $cl_profiling_command_submit $cl_profiling_command_start $cl_profiling_command_end"
#
#    if [ "x$command_name" == "xkernel_launch" ]
#    then
#      comp_time=$((($cl_profiling_command_end-$cl_profiling_command_start)/1000000))
#    fi
#  done
done
