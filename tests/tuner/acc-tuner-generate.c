
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


size_t loop_length_matmul(size_t loop_id, void * params_) {
  assert(loop_id >= 0 && loop_id <= 2);
  assert(params_ != NULL);

  size_t * params = (size_t *)params_;

  return params[loop_id];
}

int main(int argc, char ** argv) {
  size_t i, j, k;
  size_t exp;

  if (argc != 3) {
    printf("usage: %s version_db device_name\n", argv[0]);
    exit(-1);
  }

  sqlite3 * versions_db = acc_sqlite_open(argv[1], 1);

  char * devices_name[1] = {argv[2]};

  size_t power_of_2[36];
  power_of_2[0] = 1;
  for (i = 1; i < 36; i++)
    power_of_2[i] = power_of_2[i-1] * 2;

  struct acc_tuner_device_ranges_t_ * ranges_per_devices = acc_tuning_build_device_ranges(1);
    const size_t num_versions = 2090;
    ranges_per_devices[0].num_versions = num_versions;
      ranges_per_devices[0].version_ids = malloc(num_versions * sizeof(size_t));
      for (i = 0; i < num_versions; i++)
        ranges_per_devices[0].version_ids[i] = i;

    size_t gang_exp_min = 7;
    size_t gang_exp_max = 13;
    ranges_per_devices[0].num_gang_values = gang_exp_max - gang_exp_min + 1;
    ranges_per_devices[0].gang_values = malloc(ranges_per_devices[0].num_gang_values * sizeof(size_t));
    for (exp = gang_exp_min; exp <= gang_exp_max; exp++)
      ranges_per_devices[0].gang_values[exp - gang_exp_min] = power_of_2[exp];

    size_t worker_exp_min = 6;
    size_t worker_exp_max = 10;
    ranges_per_devices[0].num_worker_values = worker_exp_max - worker_exp_min + 1;
    ranges_per_devices[0].worker_values = malloc(ranges_per_devices[0].num_worker_values * sizeof(size_t));
    for (exp = worker_exp_min; exp <= worker_exp_max; exp++)
      ranges_per_devices[0].worker_values[exp - worker_exp_min] = power_of_2[exp];

    size_t vector_exp_min = 0;
    size_t vector_exp_max = 0;
    ranges_per_devices[0].num_vector_values = vector_exp_max - vector_exp_min + 1;
    ranges_per_devices[0].vector_values = malloc(ranges_per_devices[0].num_vector_values * sizeof(size_t));
    for (exp = vector_exp_min; exp <= vector_exp_max; exp++)
      ranges_per_devices[0].vector_values[exp - vector_exp_min] = power_of_2[exp];

  size_t params_exp_min = 9;
  size_t params_exp_max = 16;
  size_t num_params_seeds = params_exp_max - params_exp_min + 1;
  size_t * params_seeds = malloc(num_params_seeds * sizeof(size_t));
  for (exp = params_exp_min; exp <= params_exp_max; exp++)
    params_seeds[exp - params_exp_min] = power_of_2[exp];

  size_t max_alloc_size = power_of_2[30];
  size_t max_num_params_values = num_params_seeds * num_params_seeds * num_params_seeds;
  size_t num_params_values = 0;
  size_t * params_values = malloc(3 * max_num_params_values * sizeof(size_t));
  for (i = 0; i < num_params_seeds; i++)
    for (j = 0; j < num_params_seeds; j++)
      for (k = 0; k < num_params_seeds; k++) {
         size_t alloc_size = (params_seeds[i] * params_seeds[j] + params_seeds[i] * params_seeds[k] + params_seeds[j] * params_seeds[k]) * sizeof(float);
         if (alloc_size <= max_alloc_size) {
           params_values[3 * num_params_values + 0] = params_seeds[i];
           params_values[3 * num_params_values + 1] = params_seeds[j];
           params_values[3 * num_params_values + 2] = params_seeds[k];
           num_params_values++;
         }
       }

  struct acc_tuner_data_params_desc_t_ * data_params = acc_tuning_build_data_params(3, "n", e_sqlite_int, "m", e_sqlite_int, "p", e_sqlite_int);

  acc_tuning_init(1, devices_name, ranges_per_devices, data_params, num_params_values, params_values, 1, &loop_length_matmul, versions_db);

  acc_sqlite_close(versions_db);

  acc_profiling_exit();

  return 0;
}

