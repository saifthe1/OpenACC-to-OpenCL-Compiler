/*!
 * \addtogroup grp_libopenacc_tests
 * @{
 *
 * \file X09-tunning/main.c
 *
 */

#include "OpenACC/openacc.h"

#include "OpenACC/private/debug.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/kernel.h"
#include "OpenACC/private/loop.h"
#include "OpenACC/private/data-env.h"
#include "OpenACC/private/memory.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/private/init.h"

#include "OpenACC/internal/region.h"
#include "OpenACC/internal/kernel.h"

#include "OpenACC/utils/profiling.h"

#include "sqlite3.h"

#include <math.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_desc_0_0;
extern struct acc_region_desc_t_ region_desc_0;

void kernel_109(
  size_t n, size_t m, size_t p,
  float ** a, float ** b, float ** c,
  size_t num_gang, size_t num_worker, size_t vector_length
) {

  acc_push_data_environment();  // construct data start

  { // (1)
 
    acc_region_t region_0 = acc_build_region(&region_desc_0);

      region_0->devices[0].num_gang = num_gang;
      region_0->devices[0].num_worker = num_worker;
      region_0->devices[0].vector_length = vector_length;

    acc_present_or_copyin_regions_(region_0, a[0], n * p * sizeof(float));
    acc_present_or_copyin_regions_(region_0, b[0], p * m * sizeof(float));
    acc_present_or_create_regions_(region_0, c[0], n * m * sizeof(float));

    acc_region_start(region_0); // construct parallel start

    { // (2)

      // Create a kernel descriptor
      acc_kernel_t kernel_0 = acc_build_kernel(&kernel_desc_0_0);

      // Set parameter arguments
      kernel_0->param_ptrs[0] = &n;
      kernel_0->param_ptrs[1] = &m;
      kernel_0->param_ptrs[2] = &p;

      // Set data arguments
      kernel_0->data_ptrs[0] = a[0];
      kernel_0->data_ptrs[1] = b[0];
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

    acc_present_or_copyout_regions_(region_0, c[0], n * m *sizeof(float));

  } // (1)

  acc_pop_data_environment(); // construct data stop
}

void init_data(size_t n, size_t m, size_t p, float *** a, float *** b, float *** c);
void free_data(float ** a, float ** b, float ** c);

int main(int argc, char ** argv) {

  // Initialize OpenACC (for profiling)
  acc_init_once();

  char * experiment_desc = " version_by_devices[0] INT, acc_device_type CHAR(40), gang INT, worker INT, n INT, m INT, p INT ";
  acc_profiling_set_experiment(experiment_desc);

  assert(argc == 7);

  size_t num_gang = atoi(argv[1]);
  size_t num_worker = atoi(argv[2]);
  size_t vector_length = 1;

  size_t n = atoi(argv[3]);
  size_t m = atoi(argv[4]);
  size_t p = atoi(argv[5]);

  // Set the kernel version to be used
  size_t version_by_devices[1];
  version_by_devices[0] = atoi(argv[6]);
  kernel_desc_0_0.version_by_devices = version_by_devices;

  assert(version_by_devices[0] < kernel_desc_0_0.num_versions);

  // Set compilation flags for OpenCL codes
  region_desc_0.num_options = 2;
  region_desc_0.options = malloc(region_desc_0.num_options * sizeof(char *));
  size_t ocl_comp_opt_cnt = 0;

  // Add debug flag
  region_desc_0.options[ocl_comp_opt_cnt++] = "-g";

  // Enable the requested version of the kernel
  assert(kernel_desc_0_0.versions[version_by_devices[0]] != NULL);
  region_desc_0.options[ocl_comp_opt_cnt] = malloc((10 + strlen(kernel_desc_0_0.name) + strlen(kernel_desc_0_0.versions[version_by_devices[0]]->suffix)) * sizeof(char));
  sprintf(region_desc_0.options[ocl_comp_opt_cnt], "-DENABLE_%s%s", kernel_desc_0_0.name, kernel_desc_0_0.versions[version_by_devices[0]]->suffix);
  ocl_comp_opt_cnt++;

  char run_desc[1024];
  sprintf(run_desc, " '%zd' , '%s' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' ",
                    version_by_devices[0], acc_device_name[acc_runtime.curr_device_type], num_gang, num_worker, n, m, p);
  acc_profiling_new_run(run_desc);

  int i, j;

  float ** a;
  float ** b;
  float ** c;

  init_data(n, m, p, &a, &b, &c);

  kernel_109(n, m, p, a, b, c, num_gang, num_worker, vector_length);

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

/*! @} */

