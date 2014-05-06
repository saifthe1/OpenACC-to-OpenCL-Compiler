

#include "OpenACC/internal/region.h"
#include "OpenACC/internal/kernel.h"
#include "OpenACC/internal/loop.h"
#include "OpenACC/internal/compiler.h"

#include "OpenACC/utils/tuning.h"
#include "OpenACC/utils/profiling.h"
#include "OpenACC/utils/sqlite.h"

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

float eval_vectadd_performance(void * params, long start, long end) {
  size_t n = *(size_t *)params;

  return (2. * n) / (1. * (end - start));
}

struct acc_tuner_version_perf_data_t {
  size_t min_run_id;
  float min_gflops;
  size_t max_run_id;
  float max_gflops;
  float sum_gflops;
  size_t cnt;
};

int acc_tuner_version_perf_callback(void * user_data, int cnt, char ** values, char ** names) {
  struct acc_tuner_version_perf_data_t * perf_data = (struct acc_tuner_version_perf_data_t *)user_data;

  size_t run_id = atoi(values[0]);
  float gflops = atof(values[1]);

  perf_data->cnt++;
  perf_data->sum_gflops += gflops;

  if (gflops < perf_data->min_gflops) {
    perf_data->min_run_id = run_id;
    perf_data->min_gflops = gflops;
  }
  if (gflops > perf_data->max_gflops) {
    perf_data->max_run_id = run_id;
    perf_data->max_gflops = gflops;
  }

  return 0;
}

void acc_tuner_generate_performance_per_version_table() {
  acc_sqlite_create_table(acc_tuner->versions_db, "VersionPerf", "version_id INT, min_run_id INT, min_gflops FLOAT, max_run_id INT, max_gflops FLOAT, ave_gflops FLOAT");

  char query[100];
  char * err_msg;
  int status;

  struct acc_tuner_version_perf_data_t * perf_data = malloc(sizeof(struct acc_tuner_version_perf_data_t));

  size_t version_id;
  for (version_id = 0; version_id < 2090; version_id++) {
    perf_data->min_run_id = -1; 
    perf_data->min_gflops = 1000000000;
    perf_data->max_run_id = -1;
    perf_data->max_gflops = 0;
    perf_data->sum_gflops = 0;
    perf_data->cnt = 0;

    sprintf(query, "SELECT run_id,gflops FROM Performances WHERE version_id=='%zd'", version_id);

    status = sqlite3_exec (acc_tuner->versions_db, query, &acc_tuner_version_perf_callback, perf_data, &err_msg);
    assert(status == SQLITE_OK);

    if (perf_data->cnt > 0) {
      sprintf(query, "INSERT INTO VersionPerf Values ( '%zd', '%zd', '%f', '%zd', '%f', '%f' );",
                     version_id, perf_data->min_run_id, perf_data->min_gflops, perf_data->max_run_id,
                     perf_data->max_gflops, perf_data->sum_gflops / perf_data->cnt);
      status = sqlite3_exec(acc_tuner->versions_db, query, NULL, 0, &err_msg);
      assert(status == SQLITE_OK);
    }
  }

  free(perf_data);
}

struct acc_tuner_param_perf_data_t {
  size_t min_run_id;
  size_t min_version_id;
  float min_gflops;
  size_t max_run_id;
  size_t max_version_id;
  float max_gflops;
  float sum_gflops;
  size_t cnt;
};

int acc_tuner_param_perf_callback(void * user_data, int cnt, char ** values, char ** names) {
  struct acc_tuner_param_perf_data_t * perf_data = (struct acc_tuner_param_perf_data_t *)user_data;

  size_t run_id = atoi(values[0]);
  size_t version_id = atoi(values[1]);
  float gflops = atof(values[2]);

  perf_data->cnt++;
  perf_data->sum_gflops += gflops;

  if (gflops < perf_data->min_gflops) {
    perf_data->min_run_id = run_id;
    perf_data->min_version_id = version_id;
    perf_data->min_gflops = gflops;
  }
  if (gflops > perf_data->max_gflops) {
    perf_data->max_run_id = run_id;
    perf_data->max_version_id = version_id;
    perf_data->max_gflops = gflops;
  }

  return 0;
}

void acc_tuner_generate_performance_per_param_table() {
  acc_sqlite_create_table(acc_tuner->versions_db, "ParamPerf", "n INT, m INT, p INT, min_run_id INT, min_version_id INT, min_gflops FLOAT, max_run_id INT, max_version_id INT, max_gflops FLOAT, ave_gflops FLOAT");

  size_t i;
  size_t power_of_2[15];
  power_of_2[0] = 1;
  for (i = 1; i < 15; i++)
    power_of_2[i] = power_of_2[i-1] * 2;

  char query[200];
  char * err_msg;
  int status;

  struct acc_tuner_param_perf_data_t * perf_data = malloc(sizeof(struct acc_tuner_param_perf_data_t));

  size_t n_exp;
  for (n_exp = 5; n_exp < 14; n_exp++) {
    perf_data->min_run_id = -1; 
    perf_data->min_version_id = -1; 
    perf_data->min_gflops = 1000000000;
    perf_data->max_run_id = -1;
    perf_data->max_version_id = -1;
    perf_data->max_gflops = 0;
    perf_data->sum_gflops = 0;
    perf_data->cnt = 0;

    sprintf(query, "SELECT run_id,version_id,gflops FROM Performances WHERE n=='%zd'", power_of_2[n_exp]);

    status = sqlite3_exec (acc_tuner->versions_db, query, &acc_tuner_param_perf_callback, perf_data, &err_msg);
    assert(status == SQLITE_OK);

    if (perf_data->cnt > 0) {
      sprintf(query, "INSERT INTO ParamPerf Values ( '%zd', '%zd', '%zd', '%f', '%zd', '%zd', '%f', '%f' );",
                     power_of_2[n_exp],
                     perf_data->min_run_id, perf_data->min_version_id, perf_data->min_gflops,
                     perf_data->max_run_id, perf_data->max_version_id, perf_data->max_gflops,
                     perf_data->sum_gflops / perf_data->cnt);
      status = sqlite3_exec(acc_tuner->versions_db, query, NULL, 0, &err_msg);
      assert(status == SQLITE_OK);
    }
  }

  free(perf_data);
}

void acc_tuner_generate_performance_table(
  float (*eval_performance)(void *, long, long)
) {
  assert(acc_tuner->num_devices == 1);

  size_t i, j;
  char * perf_table_content_prefix = "run_id, device INT, version_id INT, gang INT, worker INT";
  char * perf_table_content_suffix = ", gflops FLOAT";
  size_t size = strlen(perf_table_content_prefix) + strlen(perf_table_content_suffix);
  for (i = 0; i < acc_tuner->data_params->num_params; i++)
    size += strlen(acc_tuner->data_params->name_params[i]) + strlen(acc_sqlite_type_string(acc_tuner->data_params->type_params[i])) + 3;
  char * perf_table_content = malloc(size * sizeof(char));
  perf_table_content[0] = '\0';
  strcat(perf_table_content, perf_table_content_prefix);
  for (i = 0; i < acc_tuner->data_params->num_params; i++) {
    strcat(perf_table_content, ", ");
    strcat(perf_table_content, acc_tuner->data_params->name_params[i]);
    strcat(perf_table_content, " ");
    strcat(perf_table_content, acc_sqlite_type_string(acc_tuner->data_params->type_params[i]));
  }
  strcat(perf_table_content, perf_table_content_suffix);

  acc_sqlite_create_table(acc_tuner->versions_db, "Performances", perf_table_content);

  char * runs_conds[1] = {"executed == '3'"};

  size_t entry_size;
  void * run_entries;
  size_t num_fields;
  char ** field_names;
  enum acc_sqlite_type_e * field_types;
  size_t * field_sizes;
  size_t * field_offsets;

  size_t num_entries = acc_sqlite_read_run_table(
                         acc_profiler->db_file, acc_tuner->num_devices, acc_tuner->data_params, 1, runs_conds,
                         &entry_size, &run_entries, &num_fields, &field_names, &field_types, &field_sizes, &field_offsets, "run_id"
                       );

  size_t query_length = 0;
  char * query = NULL;
  size_t num_query = 0;

  #pragma omp parallel shared(acc_profiler, acc_tuner, entry_size, run_entries, num_fields, field_names, field_types, field_sizes, field_offsets, num_entries) \
                       private(i,j) firstprivate(query_length, query, num_query)
  {
  #pragma omp for
  for (i = 0; i < num_entries; i++) {

    size_t run_id = *(size_t *)(run_entries + i * entry_size + field_offsets[0]);

    char run_id_cond[20];
    sprintf(run_id_cond, "run_id == '%zd'", run_id);
    char * events_conds[1] = {run_id_cond};
    
    struct acc_sqlite_event_entry_t * event_entries;
    size_t num_events = 0;
    #pragma omp critical
    {
      num_events = acc_sqlite_read_table(
                     acc_profiler->db_file, "Events", 1, events_conds,
                     event_entry_num_fields, event_entry_field_names, event_entry_field_types,
                                             event_entry_field_sizes, event_entry_field_offsets,
                     sizeof(struct acc_sqlite_event_entry_t), (void**)&event_entries
                   );
    }
    if (num_events < 1) {
      printf("No event for run #%d.\n", run_id);
      assert(0);
    }

    size_t device_id = event_entries[0].device_id;
    size_t version_id = *(size_t *)(run_entries + i * entry_size + field_offsets[1]);
    size_t gang = *(size_t *)(run_entries + i * entry_size + field_offsets[3]);
    size_t worker = *(size_t *)(run_entries + i * entry_size + field_offsets[4]);
    float gflops = 0;
    for (j = 0; j < num_events; j++)
      gflops += (*eval_performance)(run_entries + i * entry_size + field_offsets[6], event_entries[j].cl_profiling_command_start, event_entries[j].cl_profiling_command_end);
    gflops /= num_events;

    char run_id_str[32];
    sprintf(run_id_str, "'%zd', ", run_id);
    char device_id_str[32];
    sprintf(device_id_str, "'%zd', ", device_id);
    char version_id_str[32];
    sprintf(version_id_str, "'%zd', ", version_id);
    char gang_str[32];
    sprintf(gang_str, "'%zd', ", gang);
    char worker_str[32];
    sprintf(worker_str, "'%zd', ", worker);

    char * params_str = malloc(32 * acc_tuner->data_params->num_params * sizeof(char));
    params_str[0] = '\0';
    for (j = 0; j < acc_tuner->data_params->num_params; j++) {
      char tmp[32];
      switch (acc_tuner->data_params->type_params[j]) {
        case e_sqlite_int:
          sprintf(tmp, "'%zd', ", *(size_t *)(run_entries + i * entry_size + field_offsets[6 + j]));
          break;
        default: assert(0);
      }
      strcat(params_str, tmp);
    }

    char gflops_str[32];
    sprintf(gflops_str, "'%f'", gflops);

    query_length += strlen(run_id_str) + strlen(device_id_str) + strlen(version_id_str) + strlen(gang_str) + strlen(worker_str) + strlen(params_str) + strlen(gflops_str) + 38;
    char * new_query = realloc(query, query_length * sizeof(char));
    if (query == NULL) new_query[0] = '\0';
    query = new_query;
    num_query++;
    strcat(query, "INSERT INTO Performances VALUES ( ");
    strcat(query, run_id_str);
    strcat(query, device_id_str);
    strcat(query, version_id_str);
    strcat(query, gang_str);
    strcat(query, worker_str);
    strcat(query, params_str);
    strcat(query, gflops_str);
    strcat(query, " );");
/*
    if (num_query > 1000) {
      char * err_msg;
      int status;
      #pragma omp critical
      {
        status = sqlite3_exec (acc_tuner->versions_db, query, NULL, 0, &err_msg);
      }
      assert(status == SQLITE_OK);
      query_length = 0;
      free(query);
      query = NULL;
      num_query = 0;
    }
*/
    free(params_str);
  }
  if (num_query > 0) {
    char * err_msg;
    int status;
    #pragma omp critical
    {
      status = sqlite3_exec (acc_tuner->versions_db, query, NULL, 0, &err_msg);
    }
    assert(status == SQLITE_OK);
    query_length = 0;
    free(query);
    query = NULL;
    num_query = 0;
  }
  }

  acc_sqlite_clean_run_table_read(acc_tuner->num_devices, field_names, field_types, field_sizes, field_offsets);
  free(run_entries);
}

int main(int argc, char ** argv) {
  size_t i, j, k;
  size_t exp;

  if (argc != 2) {
    printf("usage: %s version_db\n", argv[0]);
    exit(-1);
  }

  sqlite3 * versions_db = acc_sqlite_open(argv[1], 1, 1);

  char * devices_name[1] = {argv[2]};

  // Pre-open profile DB as in-memory DB
  acc_profiler = acc_profiler_build_profiler();
  acc_profiling_get_db_file_names();
  acc_profiler->db_file = acc_sqlite_open(acc_profiler->db_file_name, 1, 1);

  acc_profiling_init();

  assert(acc_sqlite_table_exists(acc_profiler->db_file, "Runs"));
  assert(acc_sqlite_table_exists(acc_profiler->db_file, "Events"));

  struct acc_tuner_data_params_desc_t_ * data_params = acc_tuning_build_data_params(3, "n", e_sqlite_int);

  acc_tuning_init(1, devices_name, data_params, versions_db);

  acc_tuner_generate_performance_table(&eval_vectadd_performance);

  acc_sqlite_save(versions_db);

  acc_tuner_generate_performance_per_version_table();
  acc_tuner_generate_performance_per_param_table();

  acc_sqlite_save(versions_db);

//acc_tuner_generate_performance_per_version_per_param_table();

  acc_profiling_exit();

  return 0;
}
