
all:
	make -C docs all
	make -C libopenacc all
	make -C examples all

check:
	make -C docs check
	make -C libopenacc check
	make -C examples check

clean:
	make -C docs clean
	make -C libopenacc clean
	make -C examples clean
