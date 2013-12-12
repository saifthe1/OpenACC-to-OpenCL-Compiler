
all:
	make -C docs all
	make -C libopenacc all

check:
	make -C docs check
	make -C libopenacc check

clean:
	make -C docs clean
	make -C libopenacc clean
