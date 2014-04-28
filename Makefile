
all:
	make -C docs all
	make -C libopenacc all
	make -C tests all
	make -C tuner all

check: all
	make -C docs check
	make -C libopenacc check
	make -C tests check
	make -C tuner check

clean:
	make -C docs clean
	make -C libopenacc clean
	make -C tests clean
	make -C tuner clean
