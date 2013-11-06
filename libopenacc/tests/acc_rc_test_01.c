/*!
 * \addtogroup grp_libopenacc_tests
 * @{
 *
 * \file tests/acc_kernel.c
 * 
 * In this file, we tests:
 *
 */

#include <math.h>

#include <assert.h>

#include "OpenACC/openacc.h"
#include "OpenACC/private/debug.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/kernel.h"
#include "OpenACC/private/loop.h"

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_0x00_desc;
extern struct acc_region_desc_t_ region_0x00_desc;

const unsigned long n = 16*64*1*18;

int main() {
  float a[n];
  float b[n];
  float c[n];

  unsigned i;

  for (i = 0; i < n; i++) {
    a[i] = rand();
    b[i] = rand();
    c[i] = 0.;
  }

  acc_set_device_type(acc_device_any);
  acc_set_device_num(0, acc_device_any);

  acc_device_t dev = acc_get_device_type();
  int num = acc_get_device_num(dev);

  unsigned long region_0_num_gang = 16;     // clause num_gang(16)
  unsigned long region_0_num_worker = 64;   // clause num_worker(64)
  unsigned long region_0_vector_length = 1; // clause vector_length(1)

  // construct parallel region descriptor
  acc_region_t region_0 = acc_build_region(&region_0x00_desc, 1, &region_0_num_gang, &region_0_num_worker, region_0_vector_length);

  acc_region_start(region_0);

  acc_copyin(a + 0, n); // clause copyin(a[0:n])
  acc_copyin(a + 0, n); // clause copyin(b[n])

  // Create a kernel descriptor
  acc_kernel_t kernel_0 = acc_build_kernel(&kernel_0x00_desc);

  kernel_0->data_ptrs[0] = acc_deviceptr(a);
  kernel_0->data_ptrs[1] = acc_deviceptr(b);
  kernel_0->data_ptrs[2] = acc_deviceptr(c);

  kernel_0->loops[0]->lower = 0;
  kernel_0->loops[0]->upper = n;
  kernel_0->loops[0]->stride = 1;

  // Enqueue the kernel for the current region
  acc_enqueue_kernel(region_0, kernel_0);

  acc_copyout(c + 0, n); // clause copyout(c)

  acc_region_stop(region_0);

  return 0;
}

/*! @} */

