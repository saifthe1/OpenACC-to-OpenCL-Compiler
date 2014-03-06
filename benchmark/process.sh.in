
echo "set term png" > $1.plotter

echo -n "set xtics (" >> $1.plotter

/media/ssd/lib/sqlite/bin/sqlite3 \
            $1.db \
            "select rowid, tile_0, tile_1, tile_2 from experiments" \
       | awk 'BEGIN{ \
                FS="|"; \
              } \
              { \
                if (NR > 1) printf ", "; \
                printf "\"%u,%u,%u\" %u", $2, $3, $4, $1; \
              }' \
       >> $1.plotter

echo ")" >> $1.plotter

echo "set xtics rotate by -90" >> $1.plotter
echo "set xtics out offset 0,0" >> $1.plotter
echo "set xtics font \"Times-Roman, 10\"" >> $1.plotter
echo "set ytics font \"Times-Roman, 10\"" >> $1.plotter

for gang in 2 4 8 16
do
  for worker in 32 64 128 256 512
  do
    /media/ssd/lib/sqlite/bin/sqlite3 \
            $1.db \
            "select experiment, computation_time from runs where gang == $gang and worker == $worker" \
       | sed "s/|/ /g" \
       > $1\_$gang\_$worker.dat

    echo "set output \"$1\_$gang\_$worker.png\"" >> $1.plotter
    echo "plot \"$1\_$gang\_$worker.dat\" using 1:2" >> $1.plotter
  done
done

gnuplot $1.plotter

