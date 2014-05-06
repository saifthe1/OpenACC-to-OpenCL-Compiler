
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

void acc_tuner_load_gen_data(
  struct acc_tuner_gen_data_t * gen_data,
  int (*version_filter)(struct acc_sqlite_version_entry_t *),
  size_t gang_exp_min,
  size_t gang_exp_max,
  size_t worker_exp_min,
  size_t worker_exp_max,
  size_t vector_exp_min,
  size_t vector_exp_max,
  size_t params_exp_min,
  size_t params_exp_max,
  size_t max_alloc_exp
) {
  size_t i, j, k;
  size_t exp;

  assert(acc_sqlite_table_exists(acc_tuner->versions_db, "Versions"));

  char region_id_cond[20];
  sprintf(region_id_cond, "region_id == '%zd'", gen_data->region_id);
  char kernel_id_cond[20];
  sprintf(kernel_id_cond, "kernel_id == '%zd'", gen_data->kernel_id);
  char * version_conds[2] = {region_id_cond, kernel_id_cond};

  struct acc_sqlite_version_entry_t * version_entries;
  gen_data->per_devices_gen_data[0].num_versions = acc_sqlite_read_table(
                                                     acc_tuner->versions_db, "Versions", 2, version_conds,
                                                     version_entry_num_fields, version_entry_field_names, version_entry_field_types,
                                                                               version_entry_field_sizes, version_entry_field_offsets,
                                                     sizeof(struct acc_sqlite_version_entry_t), (void**)&version_entries
                                                   );
  assert(gen_data->per_devices_gen_data[0].num_versions > 0);

  gen_data->per_devices_gen_data[0].version_ids = malloc(gen_data->per_devices_gen_data[0].num_versions * sizeof(size_t));
  size_t cnt = 0;
  for (i = 0; i < gen_data->per_devices_gen_data[0].num_versions; i++)
    if (version_filter == NULL || (*version_filter)(&(version_entries[i]))) {
      gen_data->per_devices_gen_data[0].version_ids[cnt] = i;
      cnt++;
    }
  gen_data->per_devices_gen_data[0].num_versions = cnt;
  gen_data->per_devices_gen_data[0].version_ids = realloc(gen_data->per_devices_gen_data[0].version_ids, gen_data->per_devices_gen_data[0].num_versions * sizeof(size_t));
  free(version_entries);

  size_t power_of_2[36];
  power_of_2[0] = 1;
  for (i = 1; i < 36; i++)
    power_of_2[i] = power_of_2[i-1] * 2;

  gen_data->per_devices_gen_data[0].num_gang_values = gang_exp_max - gang_exp_min + 1;
  gen_data->per_devices_gen_data[0].gang_values = malloc(gen_data->per_devices_gen_data[0].num_gang_values * sizeof(size_t));
  for (exp = gang_exp_min; exp <= gang_exp_max; exp++)
    gen_data->per_devices_gen_data[0].gang_values[exp - gang_exp_min] = power_of_2[exp];

  gen_data->per_devices_gen_data[0].num_worker_values = worker_exp_max - worker_exp_min + 1;
  gen_data->per_devices_gen_data[0].worker_values = malloc(gen_data->per_devices_gen_data[0].num_worker_values * sizeof(size_t));
  for (exp = worker_exp_min; exp <= worker_exp_max; exp++)
    gen_data->per_devices_gen_data[0].worker_values[exp - worker_exp_min] = power_of_2[exp];

  gen_data->per_devices_gen_data[0].num_vector_values = vector_exp_max - vector_exp_min + 1;
  gen_data->per_devices_gen_data[0].vector_values = malloc(gen_data->per_devices_gen_data[0].num_vector_values * sizeof(size_t));
  for (exp = vector_exp_min; exp <= vector_exp_max; exp++)
    gen_data->per_devices_gen_data[0].vector_values[exp - vector_exp_min] = power_of_2[exp];

  size_t num_params_seeds = params_exp_max - params_exp_min + 1;
  size_t * params_seeds = malloc(num_params_seeds * sizeof(size_t));
  for (exp = params_exp_min; exp <= params_exp_max; exp++)
    params_seeds[exp - params_exp_min] = power_of_2[exp];

  size_t max_alloc_size = power_of_2[max_alloc_exp];
  size_t max_num_params_values = num_params_seeds * num_params_seeds * num_params_seeds;
  gen_data->num_params_values = 0;
  gen_data->params_values = malloc(3 * max_num_params_values * sizeof(size_t));
  for (i = 0; i < num_params_seeds; i++)
    for (j = 0; j < num_params_seeds; j++)
      for (k = 0; k < num_params_seeds; k++) {
         size_t alloc_size = (params_seeds[i] * params_seeds[j] + params_seeds[i] * params_seeds[k] + params_seeds[j] * params_seeds[k]) * sizeof(float);
         if (alloc_size <= max_alloc_size) {
           ((size_t *)gen_data->params_values)[3 * gen_data->num_params_values + 0] = params_seeds[i];
           ((size_t *)gen_data->params_values)[3 * gen_data->num_params_values + 1] = params_seeds[j];
           ((size_t *)gen_data->params_values)[3 * gen_data->num_params_values + 2] = params_seeds[k];
           gen_data->num_params_values++;
         }
       }

  gen_data->num_portions = 1;

  gen_data->loop_length_func = &loop_length_matmul;
}

int main(int argc, char ** argv) {
  if (argc != 12) {
    printf("usage: %s version_db device_name gang_exp_min gang_exp_max worker_exp_min worker_exp_max vector_exp_min vector_exp_max params_exp_min params_exp_max max_alloc_exp\n", argv[0]);
    exit(-1);
  }

  sqlite3 * versions_db = acc_sqlite_open(argv[1], 1, 1);

  char * devices_name[1] = {argv[2]};

  size_t gang_exp_min   = atoi(argv[3]);
  size_t gang_exp_max   = atoi(argv[4]);
  size_t worker_exp_min = atoi(argv[5]);
  size_t worker_exp_max = atoi(argv[6]);
  size_t vector_exp_min = atoi(argv[7]);
  size_t vector_exp_max = atoi(argv[8]);
  size_t params_exp_min = atoi(argv[9]);
  size_t params_exp_max = atoi(argv[10]);
  size_t max_alloc_exp  = atoi(argv[11]);

  // Pre-open profile DB as in-memory DB
  acc_profiler = acc_profiler_build_profiler();
  acc_profiling_get_db_file_names();
  acc_profiler->db_file = acc_sqlite_open(acc_profiler->db_file_name, 0, 1);

  acc_profiling_init();

  struct acc_tuner_data_params_desc_t_ * data_params = acc_tuning_build_data_params(3, "n", e_sqlite_int, "m", e_sqlite_int, "p", e_sqlite_int);

  acc_tuning_init(1, devices_name, data_params, versions_db);

  acc_tuning_create_experiment();

  struct acc_tuner_gen_data_t * gen_data = acc_tuning_build_gen_data();
  acc_tuner_load_gen_data(gen_data, NULL, gang_exp_min, gang_exp_max, worker_exp_min, worker_exp_max, vector_exp_min, vector_exp_max, params_exp_min, params_exp_max, max_alloc_exp);

  acc_sqlite_close(versions_db);

  acc_profiling_exit();

  return 0;
}

