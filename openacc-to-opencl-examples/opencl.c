/*!
 * \file opencl.c
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * This file contains the expected output of "OpenACC to OpenCL Compiler" for the file openacc.c
 * 
 * The naming convention used for this file should be followed by the compiler. It will help debugging and change tracking
 *
 */

#ifdef ENABLE_PROGRAM_VECT_ADD
# ifndef ENABLE_LIB_CSTDLIB
#  error "Vector Addition requires C Standard Library"
# endif
#endif

#ifdef ENABLE_LIB_CSTDLIB
# include <cstdlib>
#endif

#include "OpenACC/lib-openacc.h"

#ifdef ENABLE_PROGRAM_VECT_ADD

#ifndef VECT_ADD_N
# define VECT_ADD_N 100
#endif

#ifndef VECT_ADD_DATA_TYPE
# define VECT_ADD_DATA_TYPE float
#endif

int program_vector_addition(int argc, char ** argv) {
  const unsigned long n = VECT_ADD_MACRO_N;

  VECT_ADD_DATA_TYPE a[n];
  VECT_ADD_DATA_TYPE b[n];
  VECT_ADD_DATA_TYPE c[n];

  unsigned i;

  for (i = 0; i < n; i++) {
    a[i] = rand();
    b[i] = rand();
    c[i] = 0.;
  }

  acc_init(acc_build_device(e_acc_device_opencl_default)); ///< Initialise OpenACC Runtime for a default OpenCL device (NOP if acc_init() had already been called)

  unsigned long region_1_num_gang = ;
  unsgined long region_1_num_worker = ;
  acc_parallel_t region_1 = acc_build_parallel(1, &region_1_num_gang, &region_1_num_worker);

  acc_fail_if_error(acc_parallel_init(region_1));

  {

    float * args_copyin[2] = {a, b};
    float * args_copyout[1] = {res};
    
    acc_fail_if_error(acc_kernel_launch());
    #pragma acc loop
    for (i = 0; i < n; i++) {
      res[i] = a[i] + b[i];
    }
  }

  VECT_ADD_DATA_TYPE error = 0.;
  for (i = 0; i < n; i++) {
    error += (res[i] - (a[i] + b[i])) * (res[i] - (a[i] + b[i]));
  }

  return error < VECT_ADD_C_STYLE_TOLERANCE;
}

#endif
