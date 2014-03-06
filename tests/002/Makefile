
all: dev-list

dev-list: dev-list.c
	g++ -I../../libopenacc/include -L../../libopenacc/lib -lopenacc -lOpenCL dev-list.c -o dev-list

clean:
	rm -f dev-list

