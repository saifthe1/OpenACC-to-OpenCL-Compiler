
#include "OpenACC/internal/region.h"
#include "OpenACC/internal/kernel.h"
#include "OpenACC/internal/loop.h"
#include "OpenACC/internal/compiler.h"

#include "OpenACC/utils/tuning.h"
#include "OpenACC/utils/profiling.h"

#include <math.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

#include <assert.h>

acc_compiler_data_t compiler_data = {
  (const char * ) LIBOPENACC_DIR, /* Absolute directory for OpenACC runtime (needed to compile OpenCL C codes) */
  (const char * ) "lib/opencl/libopenacc.cl",  /* Name of the OpenCL C runtime file */
  (const char * ) KERNEL_DIR, /* Absolute directory where the OpenCL C kernels have been stored. */
  0, NULL
};

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

int main(int argc, char ** argv) {

  if (argc != 7) {
    printf("usage: %s version_db device_name version_id n m p\n", argv[0]);
    exit(-1);
  }

  // Load versions DB
  sqlite3 * versions_db = acc_sqlite_open(argv[1], 1);

  char * devices_name[1] = {argv[2]};

  // Get arguments
  size_t version_id = atoi(argv[3]);
  size_t n = atoi(argv[4]);
  size_t m = atoi(argv[5]);
  size_t p = atoi(argv[6]);

  // Load 'compiler_data' from version DB
  {
    struct acc_sqlite_load_compiler_data_filter_t_ * filter = malloc(sizeof(struct acc_sqlite_load_compiler_data_filter_t_));
      filter->enabled_versions = malloc(sizeof(size_t *));
      filter->num_enabled_versions = malloc(sizeof(size_t));
      filter->region_offset = malloc(sizeof(size_t));

      filter->region_offset[0] = 0;
      filter->num_enabled_versions[0] = 1;
      filter->enabled_versions[0] = malloc(sizeof(size_t));
      filter->enabled_versions[0][0] = version_id;

    acc_sqlite_load_compiler_data(versions_db, filter);

    free(filter->enabled_versions[0]);
    free(filter->enabled_versions);
    free(filter->num_enabled_versions);
    free(filter->region_offset);
    free(filter);
  }

  // Build data parameter descriptor to read paramter from Experiments DB
  struct acc_tuner_data_params_desc_t_ * data_params = acc_tuning_build_data_params(3, "n", e_sqlite_int, "m", e_sqlite_int, "p", e_sqlite_int);

  // Initialize the tuner
  assert(acc_tuning_init(1, devices_name, NULL, data_params, 0, NULL, 0, NULL, versions_db) == 0);

  // Get/Init region descriptor
  assert(compiler_data.num_regions == 1);
  assert(compiler_data.regions[0] != NULL);
  struct acc_region_desc_t_ * region = compiler_data.regions[0];
    region->num_devices = 1;
    region->devices     = NULL;
    region->num_distributed_data = 0;
    region->distributed_data     = NULL;

  // Get/Init kernel descriptor
  assert(region->num_kernels == 1);
  assert(region->kernels[0] != NULL);
  struct acc_kernel_desc_t_ * kernel = region->kernels[0];
    size_t size_params[3] = {sizeof(int), sizeof(int), sizeof(int)};
    kernel->num_params    = 3;
    kernel->size_params   = size_params;
    kernel->num_scalars  = 0;
    kernel->size_scalars = NULL;
    kernel->num_datas = 3;
    kernel->num_loops = 3; // Should be loaded from DB, but CG save 0 in the corresponding field...

  // Initialize data
  float ** a, ** b, ** c;
  init_data(n, m, p, &a, &b, &c);

  // Construct execution data
  struct acc_tuner_exec_data_t * exec_data = malloc(sizeof(struct acc_tuner_exec_data_t));
    // Build an instance of the region
    exec_data->region = acc_build_region(region);
      exec_data->region->devices[0].num_gang = 0;
      exec_data->region->devices[0].num_worker = 0;
      exec_data->region->devices[0].vector_length = 0;

    // Build an instance of the kernel
    exec_data->kernel = acc_build_kernel(kernel);
      // Set kernel's parameters
      exec_data->kernel->param_ptrs[0] = &n;
      exec_data->kernel->param_ptrs[1] = &m;
      exec_data->kernel->param_ptrs[2] = &p;

      // Set kernel's data pointers
      exec_data->kernel->data_ptrs[0] = a[0];
      exec_data->kernel->data_size[0] = n * p * sizeof(float);
      exec_data->kernel->data_ptrs[1] = b[0];
      exec_data->kernel->data_size[1] = p * m * sizeof(float);
      exec_data->kernel->data_ptrs[2] = c[0];
      exec_data->kernel->data_size[2] = n * m * sizeof(float);

      // Set kernel's loops i
      exec_data->kernel->loops[0]->lower = 0;
      exec_data->kernel->loops[0]->upper = n;
      exec_data->kernel->loops[0]->stride = 1;

      // Set kernel's loops j
      exec_data->kernel->loops[1]->lower = 0;
      exec_data->kernel->loops[1]->upper = m;
      exec_data->kernel->loops[1]->stride = 1;

      // Set kernel's loops k
      exec_data->kernel->loops[2]->lower = 0;
      exec_data->kernel->loops[2]->upper = p;
      exec_data->kernel->loops[2]->stride = 1;

    // Data to be copyin before kernel
    exec_data->num_data_in = 2;
    size_t data_in[2] = {0, 1};
    exec_data->data_in = data_in;

    // Data to be copyout after kernel
    exec_data->num_data_out = 1;
    size_t data_out[1] = {2};
    exec_data->data_out = data_out;

    // Data to be create before kernel
    exec_data->num_data_create = 1;
    size_t data_create[1] = {2};
    exec_data->data_create = data_create;

  // Execute all configuration that match: version_id, n, m, p, and have not been previously executed
  acc_tuning_execute(exec_data, version_id, n, m, p);

  free_data(a, b, c);

  acc_sqlite_close(versions_db);

  return 0;
}

