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
# define VECT_ADD_N 16*64*4*18
#endif

int program_vector_addition(int argc, char ** argv) {
  const unsigned long n = VECT_ADD_N;

  float a[n];
  float b[n];
  float c[n];

  unsigned i;

  for (i = 0; i < n; i++) {
    a[i] = rand();
    b[i] = rand();
    c[i] = 0.;
  }

// --- BEGINNING OF THE TRANSFORMED CODE ---

  // Initialise OpenACC Runtime for a default OpenCL device (NOP if acc_init() had already been called)
  acc_init(acc_build_device(e_acc_device_opencl_default));

  unsigned long region_0_num_gang = 16;     // clause num_gang(16)
  unsigned long region_0_num_worker = 64;   // clause num_worker(64)
  unsigned long region_0_vector_length = 4; // clause vector_length(4)

  // construct parallel region descriptor
  acc_parallel_t region_0 = acc_build_parallel(1, &region_0_num_gang, &region_0_num_worker, region_0_vector_length);

  // Start the parallel region
  acc_fail_if_error(acc_parallel_start(region_0));

  acc_copyin(a + 0, n); // clause copyin(a[0:n]) : offset 0,           size n
  acc_copyin(a + 0, n); // clause copyin(b[n])   : offset 0 (default), size n

  { // beginning of the basic block scoped under the parallel directive

    // Create a kernel descriptor
    acc_kernel_t kernel_0 = acc_build_kernel(0);

      // Set the scalar arg (pointer + size)
      kernel_0->scalar_arguments[0] = &n;
      kernel_0->scalar_sizes[0]     = sizeof(n);

      // Set data args using device pointers
      kernel_0->data_arguments[0] = acc_deviceptr(a);
      kernel_0->data_arguments[1] = acc_deviceptr(b);
      kernel_0->data_arguments[2] = acc_deviceptr(c);

    // Enqueue the kernel for the current region
    acc_fail_if_error(acc_enqueue_kernel(region_0, kernel_0));

  } // end of the basic block scoped under the parallel directive

  acc_copyout(c + 0, n); // clause copyout(c) : offset 0 (default), size n (type_of(c) = float[n])

  // Stop the parallel region
  acc_fail_if_error(acc_parallel_stop(region_1));

  // Implicit barrier at the end of the parallel region
  acc_async_wait_all();

// --- END OF THE TRANSFORMED CODE ---

  float error = 0.;
  for (i = 0; i < n; i++) {
    error += (res[i] - (a[i] + b[i])) * (res[i] - (a[i] + b[i]));
  }

  return error < VECT_ADD_C_STYLE_TOLERANCE;
}

#endif
