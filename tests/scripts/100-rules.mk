
OPENACC_DIR=$(TOP_DIR)/libopenacc
KERNEL_DIR=$(TOP_DIR)/tests/$(TEST_ID)

include ../scripts/environment.mk

all: $(TEST_ID)-libopenacc $(TEST_ID)-reference

check: $(TEST_ID)-libopenacc $(TEST_ID)-libopenacc $(TEST_ID)-reference
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(OPENACC_LIB_DIR) ./$(TEST_ID)-libopenacc $(NBR_GANG) $(NBR_WORKER) $(NBR_VECTOR) $(TEST_PARAMS)
	@echo
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(OPENACC_LIB_DIR) ./$(TEST_ID)-reference 1 1 1 $(TEST_PARAMS)
	@echo

clean:
	rm -f *.o $(TEST_ID)-libopenacc* $(TEST_ID)-reference

$(TEST_ID)-libopenacc: $(TEST_ID)-libopenacc.o $(TEST_ID)-host-data.o $(DEPS)
	$(LD) $(LDFLAGS) $(LIBS) $(TEST_ID)-libopenacc.o $(LIBS) $(TEST_ID)-host-data.o -o $(TEST_ID)-libopenacc

$(TEST_ID)-libopenacc.o: \
            ../100-main.c $(TEST_ID)-kernel-libopenacc.c $(TEST_ID)-init.c \
            $(ACC_PUBLIC_DEPS) $(ACC_PRIVATE_DEPS)
	$(CXX) $(CFLAGS) $(INCLUDES) \
	  -DOPENACC -DOPENACC_TIMER \
	  -DKERNEL_FILE=\"$(KERNEL_DIR)/$(TEST_ID)-kernel-libopenacc.c\" \
	  -DINIT_FILE=\"$(KERNEL_DIR)/$(TEST_ID)-init.c\" \
	  -c ../100-main.c -o $(TEST_ID)-libopenacc.o

$(TEST_ID)-host-data.o: $(TEST_ID)-host-data.c $(ACC_INTERNAL_DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) $(HOST_ENV_MACRO) -c $(TEST_ID)-host-data.c -o $(TEST_ID)-host-data.o

$(TEST_ID)-reference: $(TEST_ID)-reference.o $(OPENACC_LIB_DIR)/utils/timer.o
	$(LD) $(LDFLAGS) -lrt $(OPENACC_LIB_DIR)/utils/timer.o $(TEST_ID)-reference.o -o $(TEST_ID)-reference

$(TEST_ID)-reference.o: \
            ../100-main.c $(TEST_ID)-kernel-openacc.c $(TEST_ID)-init.c \
            $(ACC_PUBLIC_DEPS) $(ACC_PRIVATE_DEPS)
	$(CXX) $(CFLAGS) $(INCLUDES) \
	  -DOPENACC_TIMER \
	  -DKERNEL_FILE=\"$(KERNEL_DIR)/$(TEST_ID)-kernel-openacc.c\" \
	  -DINIT_FILE=\"$(KERNEL_DIR)/$(TEST_ID)-init.c\" \
	  -c ../100-main.c -o $(TEST_ID)-reference.o

# LibOpenACC dependency

$(OPENACC_LIB_FILE):
	$(MAKE) -C $(OPENACC_LIB_DIR) lib$(OPENACC_LIB_NAME).so

$(OPENACC_LIB_DIR)/utils/timer.o:
	$(MAKE) -C $(OPENACC_LIB_DIR) utils/timer.o

