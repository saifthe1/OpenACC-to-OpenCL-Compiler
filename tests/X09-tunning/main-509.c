
#include "OpenACC/openacc.h"

#include "OpenACC/private/debug.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/kernel.h"
#include "OpenACC/private/loop.h"
#include "OpenACC/private/data-env.h"
#include "OpenACC/private/memory.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/private/init.h"

#include "OpenACC/utils/profiling.h"

#include "sqlite3.h"

#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_desc_0_0;
extern struct acc_region_desc_t_ region_desc_0;

extern unsigned portions[2];
extern size_t gpu_kernel_id;
extern size_t mic_kernel_id;

void kernel_509(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1
) {
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

  acc_region_start(region_0); // construct parallel start

  // Create a kernel descriptor
  acc_kernel_t kernel_0 = acc_build_kernel(&kernel_desc_0_0);

    // Set parameter arguments
    kernel_0->param_ptrs[0] = &n;
    kernel_0->param_ptrs[1] = &m;
    kernel_0->param_ptrs[2] = &p;

    // Set data arguments
    kernel_0->data_ptrs[0] = a[0];
    kernel_0->data_size[0] = n * p * sizeof(float);
    kernel_0->data_ptrs[1] = b[0];
    kernel_0->data_size[1] = p * m * sizeof(float);
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

  acc_present_or_copyout_regions_(region_0, c[0], n * m *sizeof(float));
 
  acc_pop_data_environment();
}

void init_data(size_t n, size_t m, size_t p, float *** a, float *** b, float *** c);
void free_data(float ** a, float ** b, float ** c);

int main(int argc, char ** argv) {

  // Initialize OpenACC (for profiling)
  acc_init_once();

  char * experiment_desc = " gpu_kernel_id INT, mic_kernel_id INT, gpu_portion INT, mic_portion INT, gpu_gang INT, gpu_worker INT, mic_gang INT, mic_worker INT, n INT, m INT, p INT ";
  acc_profiling_set_experiment(experiment_desc);

  assert(argc == 8);

  size_t num_gang_0 = atoi(argv[1]);
  size_t num_worker_0 = atoi(argv[2]);
  size_t vector_length_0 = 1;

  size_t num_gang_1 = atoi(argv[3]);
  size_t num_worker_1 = atoi(argv[4]);
  size_t vector_length_1 = 1;

  size_t n = atoi(argv[5]);
  size_t m = atoi(argv[6]);
  size_t p = atoi(argv[7]);

  char run_desc[1024];
  sprintf(run_desc, " '%zd' , '%zd' , '%u' , '%u' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' ",
                    gpu_kernel_id, mic_kernel_id, portions[0], portions[1], num_gang_0, num_worker_0, num_gang_1, num_worker_1, n, m, p);
  acc_profiling_new_run(run_desc);

  int i, j;

  float ** a;
  float ** b;
  float ** c;

  init_data(n, m, p, &a, &b, &c);

  kernel_509(n, m, p, a, b, c, num_gang_0, num_worker_0, vector_length_0, num_gang_1, num_worker_1, vector_length_1);

  free_data(a, b, c);

  acc_profiling_exit();

  return 0;
}

void init_data(size_t n, size_t m, size_t p, float *** a, float *** b, float *** c) {
  size_t i, j;

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

void free_data(float ** a, float ** b, float ** c) {

  free(a[0]);
  free(a);

  free(b[0]);
  free(b);

  free(c[0]);
  free(c);
}

