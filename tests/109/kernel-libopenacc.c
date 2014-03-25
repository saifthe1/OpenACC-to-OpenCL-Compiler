
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

void kernel_109(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang, unsigned long num_worker, unsigned long vector_length,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned i;

  acc_timer_start(data_timer);

  acc_push_data_environment();  // construct data start

  { // (1)
 
    acc_region_t region_0 = acc_build_region(&region_desc_0);

      region_0->devices[0].num_gang = num_gang;
      region_0->devices[0].num_worker = num_worker;
      region_0->devices[0].vector_length = vector_length;

    acc_present_or_copyin_regions_(region_0, a[0], n * p * sizeof(float));
    acc_present_or_copyin_regions_(region_0, b[0], p * m * sizeof(float));
    acc_present_or_create_regions_(region_0, c[0], n * m * sizeof(float));

    acc_timer_start(comp_timer);

    acc_region_start(region_0); // construct parallel start

    { // (2)

      // Create a kernel descriptor
      acc_kernel_t kernel_0 = acc_build_kernel(&kernel_desc_0_0);

      // Set parameter arguments
      kernel_0->param_ptrs[0] = &n;
      kernel_0->param_ptrs[1] = &m;
      kernel_0->param_ptrs[2] = &p;

      // Set data arguments
      kernel_0->data_ptrs[0] = b[0];
      kernel_0->data_ptrs[1] = a[0];
      kernel_0->data_ptrs[2] = c[0];

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

    } // (2)

    acc_region_stop(region_0); // construct parallel stop

    acc_timer_stop(comp_timer);

    acc_present_or_copyout_regions_(region_0, c[0], n * m *sizeof(float));

  } // (1)

  acc_pop_data_environment(); // construct data stop

  acc_timer_stop(data_timer);
}

void read_params(
  int argc, char ** argv,
  int * n, int * m, int * p
) {
  assert(argc == 7);

  *n = atoi(argv[4]);
  *m = atoi(argv[5]);
  *p = atoi(argv[6]);
}

void init_data(int n, int m, int p, float *** a, float *** b, float *** c) {
  int i, j;

  float * data;
  float ** tmp;

  data = (float*)malloc(n * p * sizeof(float));
  for (i = 0; i < n*p; i++)
    data[i] = i; //rand() * 1. / rand();
  tmp = (float**)malloc(n * sizeof(float*));
  for (i = 0; i < n; i++)
    tmp[i] = &(data[i * p]);
  *a = tmp;

  data = (float*)malloc(p * m * sizeof(float));
  for (i = 0; i < p*m; i++)
    data[i] = i; //rand() * 1. / rand();
  tmp = (float**)malloc(p * sizeof(float*));
  for (i = 0; i < p; i++)
    tmp[i] = &(data[i * m]);
  *b = tmp;

  data = (float*)malloc(n * m * sizeof(float));
  for (i = 0; i < n*m; i++)
    data[i] = 0;
  tmp = (float**)malloc(n * sizeof(float*));
  for (i = 0; i < n; i++)
    tmp[i] = &(data[i * m]);
  *c = tmp;
}

void free_data(int n, int m, int p, float ** a, float ** b, float ** c) {
  int i;

  free(a[0]);
  free(a);

  free(b[0]);
  free(b);

  free(c[0]);
  free(c);
}

void launch(
  int argc, char ** argv,
  unsigned long num_gang,
  unsigned long num_worker,
  unsigned long vector_length,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  int n, m, p;

  int i, j;

  read_params(argc, argv, &n, &m, &p);

  float ** a;
  float ** b;
  float ** c;

  init_data(n, m, p, &a, &b, &c);

  kernel_109(n, m, p, a, b, c, num_gang, num_worker, vector_length, data_timer, comp_timer);

  acc_timer_delta(data_timer);
  acc_timer_delta(comp_timer);

  free_data(n, m, p, a, b, c);
}

