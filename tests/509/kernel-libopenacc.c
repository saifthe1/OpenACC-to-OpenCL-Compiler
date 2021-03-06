
#include <stdio.h>

#include "OpenACC/openacc.h"
#include "OpenACC/utils/timer.h"

#include "OpenACC/private/debug.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/kernel.h"
#include "OpenACC/private/loop.h"
#include "OpenACC/private/data-env.h"
#include "OpenACC/private/memory.h"

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_desc_0_0;
extern struct acc_region_desc_t_ region_desc_0;

void kernel_509(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  acc_timer_start(data_timer);
 
  acc_push_data_environment();

  acc_region_t region_0 = acc_build_region(&region_desc_0);

    region_0->devices[0].num_gang = num_gang_0;
    region_0->devices[0].num_worker = num_worker_0;
    region_0->devices[0].vector_length = vector_length_0;

    region_0->devices[1].num_gang = num_gang_1;
    region_0->devices[1].num_worker = num_worker_1;
    region_0->devices[1].vector_length = vector_length_1;

    region_0->distributed_data[0].ptr  = a[0];
    region_0->distributed_data[0].size = n;
    region_0->distributed_data[1].ptr  = c[0];
    region_0->distributed_data[1].size = n;

  acc_present_or_copyin_regions_(region_0, a[0], n * p * sizeof(float));
  acc_present_or_copyin_regions_(region_0, b[0], p * m * sizeof(float));
  acc_present_or_create_regions_(region_0, c[0], n * m * sizeof(float));

  acc_timer_start(comp_timer);

  acc_region_start(region_0); // construct parallel start

  // Create a kernel descriptor
  acc_kernel_t kernel_0 = acc_build_kernel(&kernel_desc_0_0);

    // Set parameter arguments
    kernel_0->param_ptrs[0] = &n;
    kernel_0->param_ptrs[1] = &m;
    kernel_0->param_ptrs[2] = &p;

    // Set data arguments
    kernel_0->data_ptrs[0] = b[0];
    kernel_0->data_size[0] = p * m * sizeof(float);
    kernel_0->data_ptrs[1] = a[0];
    kernel_0->data_size[1] = n * p * sizeof(float);
    kernel_0->data_ptrs[2] = c[0];
    kernel_0->data_size[2] = n * m * sizeof(float);

    // Configure loops
    kernel_0->loops[0]->lower = 0;
    kernel_0->loops[0]->upper = n;
    kernel_0->loops[0]->stride = 1;

    kernel_0->loops[1]->lower = 0;
    kernel_0->loops[1]->upper = m;
    kernel_0->loops[1]->stride = 1;

    kernel_0->loops[2]->lower = 0;
    kernel_0->loops[2]->upper = p;
    kernel_0->loops[2]->stride = 1;

  // Enqueue the kernel for the current region
  acc_enqueue_kernel(region_0, kernel_0);

  acc_region_stop(region_0); // construct parallel stop

  acc_timer_stop(comp_timer);

  acc_present_or_copyout_regions_(region_0, c[0], n * m *sizeof(float));
 
  acc_pop_data_environment();

  acc_timer_stop(data_timer);
}

