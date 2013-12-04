
all:

check: # vect-add vect-add-acc vect-add-acc-ocl

vect-add: vect-add.o
	gcc vect-add.o -o vect-add

vect-add-acc: vect-add-acc.o
	gcc ../libopenacc/lib/libopenacc.o vect-add-acc.o -o vect-add-acc

vect-add-acc-ocl: transformed-vect-add.o generated-vect-add.o ../libopenacc/lib/libopenacc.o
	gcc ../libopenacc/lib/libopenacc.o transformed-vect-add.o generated-vect-add.o -o vect-add-acc-ocl

vect-add.o: vect-add.c
	gcc -c vect-add.c -o vect-add.o

vect-add-acc.o: vect-add.c
	gcc -D_OPENACC_C=1 -D_OPENACC_C_NAME="OpenACC to OpenCL ROSE Compiler" -D_OPENACC_C_VERSION=201310 -I../libopenacc/include -g -c vect-add.c -o vect-add-acc.o

transformed-vect-add.o: transformed-vect-add.c
	gcc -D_OPENACC_C=1 -D_OPENACC_C_NAME="OpenACC to OpenCL ROSE Compiler" -D_OPENACC_C_VERSION=201310 -D__ROSE_TRANSFORMED__=1 -I../libopenacc/include -g -c transformed-vect-add.c -o transformed-vect-add.o

generated-vect-add.o: generated-vect-add.c
	gcc -D_OPENACC_C=1 -D_OPENACC_C_NAME="OpenACC to OpenCL ROSE Compiler" -D_OPENACC_C_VERSION=201310 -D__ROSE_GENERATED__=1   -I../libopenacc/include -g -c generated-vect-add.c -o generated-vect-add.o

clean:
	rm -rf *.o vect-add vect-add-acc vect-add-acc-ocl

