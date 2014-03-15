/*!
 * \addtogroup grp_libopenacc_tests
 * @{
 *
 * \file tests/test_01.c
 *
 */

#include <math.h>

#include <stdio.h>

#include <assert.h>

#include "OpenACC/openacc.h"
#include "OpenACC/private/init.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/internal/compiler.h"

acc_compiler_data_t compiler_data = {
  (const char * ) LIBOPENACC_DIR, /* Absolute directory for OpenACC runtime (needed to compile OpenCL C codes) */
  (const char * ) "opencl/libopenacc.cl",  /* Name of the OpenCL C runtime file */
  (const char * ) KERNEL_DIR, /* Absolute directory where the OpenCL C kernels have been stored. */
  0, NULL
};

int main() {
  acc_init_once();

  unsigned dev;
  for (dev = 0; dev < acc_device_last; dev++) {
    printf("%s: %u, %u\n", acc_device_name[dev], acc_runtime.devices[dev].first, acc_runtime.devices[dev].num);
  }

  return 0;
}

/*! @} */

