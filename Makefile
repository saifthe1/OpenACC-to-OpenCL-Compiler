
all:
	make -C docs all
	make -C libopenacc all
	make -C tests all

check: all
	make -C docs check
	make -C libopenacc check
	make -C tests check

clean:
	make -C docs clean
	make -C libopenacc clean
	make -C tests clean
