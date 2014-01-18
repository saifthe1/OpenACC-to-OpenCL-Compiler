
SQLITE_PATH=/media/ssd/lib/sqlite

TESTS_PATH=/media/ssd/projects/currents/openacc-to-opencl-compiler/rose/projects/openacc-to-opencl/tests

OPENACC_PATH=/media/ssd/projects/currents/openacc-to-opencl-compiler/libopenacc

all: \
       init_db synthesis \
       eval-101-t0 eval-101-t1 eval-101-t2 \
       eval-102-t0 eval-102-t1 eval-102-t2 \
       eval-103-t0 eval-103-t1 eval-103-t2

#

init_db.o: init_db.c
	gcc -I$(SQLITE_PATH)/include -c $^ -o $@

init_db: init_db.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 $^ -o $@

#

synthesis.o: synthesis.c
	gcc -I$(SQLITE_PATH)/include -c $^ -o $@

synthesis: synthesis.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 $^ -o $@

#

eval-101.o: eval.c $(TESTS_PATH)/101-vector-constant-addition/101-kernel-libopenacc.c $(TESTS_PATH)/101-vector-constant-addition/101-init.c
	gcc -I$(SQLITE_PATH)/include -I$(OPENACC_PATH)/include \
	  -DKERNEL_FILE=\"$(TESTS_PATH)/101-vector-constant-addition/101-kernel-libopenacc.c\" \
	  -DINIT_FILE=\"$(TESTS_PATH)/101-vector-constant-addition/101-init.c\" \
	  -c eval.c -o $@

host-data-101-t0.o: $(TESTS_PATH)/101-vector-constant-addition/101-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=1 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/101-vector-constant-addition\" \
	  -c $^ -o $@

host-data-101-t1.o: $(TESTS_PATH)/101-vector-constant-addition/101-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=2 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/101-vector-constant-addition\" \
	  -c $^ -o $@

host-data-101-t2.o: $(TESTS_PATH)/101-vector-constant-addition/101-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=3 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/101-vector-constant-addition\" \
	  -c $^ -o $@

eval-101-t0: eval-101.o host-data-101-t0.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-101.o host-data-101-t0.o -o $@

eval-101-t1: eval-101.o host-data-101-t1.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-101.o host-data-101-t1.o -o $@

eval-101-t2: eval-101.o host-data-101-t2.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-101.o host-data-101-t2.o -o $@

#

eval-102.o: eval.c $(TESTS_PATH)/102-vector-constant-product/102-kernel-libopenacc.c $(TESTS_PATH)/102-vector-constant-product/102-init.c
	gcc -I$(SQLITE_PATH)/include -I$(OPENACC_PATH)/include \
	  -DKERNEL_FILE=\"$(TESTS_PATH)/102-vector-constant-product/102-kernel-libopenacc.c\" \
	  -DINIT_FILE=\"$(TESTS_PATH)/102-vector-constant-product/102-init.c\" \
	  -c eval.c -o $@

host-data-102-t0.o: $(TESTS_PATH)/102-vector-constant-product/102-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=1 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/102-vector-constant-product\" \
	  -c $^ -o $@

host-data-102-t1.o: $(TESTS_PATH)/102-vector-constant-product/102-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=2 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/102-vector-constant-product\" \
	  -c $^ -o $@

host-data-102-t2.o: $(TESTS_PATH)/102-vector-constant-product/102-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=3 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/102-vector-constant-product\" \
	  -c $^ -o $@

eval-102-t0: eval-102.o host-data-102-t0.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-102.o host-data-102-t0.o -o $@

eval-102-t1: eval-102.o host-data-102-t1.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-102.o host-data-102-t1.o -o $@

eval-102-t2: eval-102.o host-data-102-t2.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-102.o host-data-102-t2.o -o $@

#

eval-103.o: eval.c $(TESTS_PATH)/103-vector-vector-addition/103-kernel-libopenacc.c $(TESTS_PATH)/103-vector-vector-addition/103-init.c
	gcc -I$(SQLITE_PATH)/include -I$(OPENACC_PATH)/include \
	  -DKERNEL_FILE=\"$(TESTS_PATH)/103-vector-vector-addition/103-kernel-libopenacc.c\" \
	  -DINIT_FILE=\"$(TESTS_PATH)/103-vector-vector-addition/103-init.c\" \
	  -c eval.c -o $@

host-data-103-t0.o: $(TESTS_PATH)/103-vector-vector-addition/103-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=1 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/103-vector-vector-addition\" \
	  -c $^ -o $@

host-data-103-t1.o: $(TESTS_PATH)/103-vector-vector-addition/103-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=2 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/103-vector-vector-addition\" \
	  -c $^ -o $@

host-data-103-t2.o: $(TESTS_PATH)/103-vector-vector-addition/103-host-data.c
	gcc  -I$(OPENACC_PATH)/include \
	  -DKERNEL_VERSION=3 \
	  -DACC_RUNTIME_ABS_DIR=\"$(OPENACC_PATH)\" \
	  -DACC_KERNELS_ABS_DIR=\"$(TESTS_PATH)/103-vector-vector-addition\" \
	  -c $^ -o $@

eval-103-t0: eval-103.o host-data-103-t0.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-103.o host-data-103-t0.o -o $@

eval-103-t1: eval-103.o host-data-103-t1.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-103.o host-data-103-t1.o -o $@

eval-103-t2: eval-103.o host-data-103-t2.o
	gcc -L$(SQLITE_PATH)/lib -lsqlite3 -L$(OPENACC_PATH)/lib -lopenacc -lOpenCL -lrt eval-103.o host-data-103-t2.o -o $@

#

clean:
	rm -f *.o init_db synthesis eval-101-t0  eval-101-t1  eval-101-t2  eval-102-t0  eval-102-t1  eval-102-t2  eval-103-t0  eval-103-t1  eval-103-t2

