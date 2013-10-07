/*!
 * \file vect-add.c
 *
 * \author Tristan Vanderbruggen
 * \date 09/2013
 * 
 * Part of OpenACC C tests.
 *
 * This example performs the addition of 2 vectors. This example is used as base test for version 0.0.1 of the OpenACC to OpenCL Compiler...
 * It requires to:
 *   - handle copy (in/out, 3 kind of array refs)
 *   - setting gang, worker, and vector sizes
 *   - distributing ONE (1) loop over the three levels of gang, worker, and vector
 *
 * The naming convention is NOT used by the compiler. However, in this file it is required for automated testing and checking of the results.
 *
 */

#ifdef ENABLE_LIB_CSTDLIB
# include <cstdlib>
#endif

#ifdef _OPENACC_c
#include "OpenACC/openacc.h"
#endif

#ifndef N
# define N 16*64*4*18
#endif

#ifndef TOLERANCE
# define TOLERANCE 1e-6
#endif

int main(int argc, char ** argv) {
  const unsigned long n = N;

  float a[n];
  float b[n];
  float c[n];

  unsigned i;

  for (i = 0; i < n; i++) {
    a[i] = rand();
    b[i] = rand();
    c[i] = 0.;
  }

  #pragma acc parallel copyin(a[0:n], b[n]) copyout(c) num_gangs(16) num_worker(64) vector_length(4)
  {
    #pragma acc loop gang worker vector
    for (i = 0; i < n; i++) {
      c[i] = a[i] + b[i];
    }
  }

  float error = 0.;
  for (i = 0; i < n; i++) {
    error += (c[i] - (a[i] + b[i])) * (c[i] - (a[i] + b[i]));
  }

  return error < TOLERANCE;
}

