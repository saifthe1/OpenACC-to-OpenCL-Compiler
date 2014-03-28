X09-tunning
===========

This folder contains scripts and generated/hand-written codes used to tune matrix multiply when it uses multiple devices.

The scripts are made to be configure on different machines, but the codes are system independent, they can be generated on one machine and compiled/executed on another.
It prevents us from compiling ROSE on too many machines.

## 1st: tune for different devices

### Generation

The generate-109 script uses looptree descriptions from tests/109 to generate multiple versions using OpenACC kernel generator.
Resulting codes are stored in versions.tar.gz alongside a database describing the generated codes.

Generated kernels in stored versions.tar.gz have been modified because the order of the parameters a and b is not deterministic between execution of the code generator.
This is a non-issue when considering the full code generator but we use hand-written application code.

Do not regenerate the kernels without a good reason!

### Compilation

In a new directory launch '$OPENACC\_DIR/tests/X09-tunning/compile-109.sh $OPENACC\_DIR/tests/X09-tunning/versions.tar.gz'.

### Execution

In the compilation directory make a local copy of '$OPENACC\_DIR/tests/X09-tunning/execute-109.sh'.
Then edit the variables: DEVICE\_TYPE\_VALUES, GANG\_VALUES, WORKER\_VALUES, N\_VALUES, M\_VALUES, P\_VALUES ; they represent different values to try.

'./execute-109.sh' will execute the requested configuration for all versions (if gang, worker values work with the n and/or m values).
It produces 'profiles.db' containing informations about the different runs and all the available profiling informations from OpenCL.

### Analysis

The script 'process.sh' is given as an example. It produces CSV files splitted between gtx460 and xeonphi (tests I am using it for).

## 2nd: Executing on two devices

### Generation

This part is hand-written. I haven't figure out a clean way to automate it yet.

### Compilation

Copy '$OPENACC\_DIR/tests/X09-tunning/compile-509.sh' in a new directory.
This script will generate many version of splitted matrix multiply.
It will use different versions of the kernel for each device (only 2 devices for now and specialized for gtx460 and xeonphi).

Then edit variables: PORTIONS (number of portion the computation should be split on, will produce one executable per possible ratio), GPU\_KERNELS and  MIC\_KERNELS.

