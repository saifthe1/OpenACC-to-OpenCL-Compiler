
#include "OpenACC/private/debug.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/kernel.h"
#include "OpenACC/private/loop.h"
#include "OpenACC/private/data-env.h"
#include "OpenACC/private/memory.h"

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_0x00_desc;
extern struct acc_region_desc_t_ region_0x00_desc;

void kernel_103(
  unsigned long n, float * a, float * b, float * c,
  unsigned long num_gang, unsigned long num_worker, unsigned long vector_length,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned i;

  acc_timer_start(data_timer);

  acc_push_data_environment();  // construct data start

  acc_copyin(a, n * sizeof(float)); // clause copyin(a[0:n])
  acc_copyin(b, n * sizeof(float)); // clause copyin(b[0:n])

  acc_present_or_create(c, n * sizeof(float)); // default for clause copyout(c[0:n])

  { // (1)

    acc_timer_start(comp_timer);

    acc_region_t region_0 = acc_build_region(&region_0x00_desc);

      region_0->devices[0].num_gang = num_gang;
      region_0->devices[0].num_worker = num_worker;
      region_0->devices[0].vector_length = vector_length;

    acc_present_or_copyin_regions_(region_0, a, n * sizeof(float));

    acc_region_start(region_0); // construct parallel start

    { // (2)

      // Create a kernel descriptor
      acc_kernel_t kernel_0 = acc_build_kernel(&kernel_0x00_desc);

      // Set data arguments
      kernel_0->data_ptrs[0] = a;
      kernel_0->data_ptrs[1] = b;
      kernel_0->data_ptrs[2] = c;

      // Configure the loop
      kernel_0->loops[0]->lower = 0;
      kernel_0->loops[0]->upper = n;
      kernel_0->loops[0]->stride = 1;

      // Enqueue the kernel for the current region
      acc_enqueue_kernel(region_0, kernel_0);

    } // (2)

    acc_region_stop(region_0); // construct parallel stop

    acc_timer_stop(comp_timer);

    acc_present_or_copyout_regions_(region_0, a, n * sizeof(float));

  } // (1)

  acc_copyout(c, n * sizeof(float)); // clause copyout(c[0:n])

  acc_pop_data_environment(); // construct data stop

  acc_timer_stop(data_timer);
}

