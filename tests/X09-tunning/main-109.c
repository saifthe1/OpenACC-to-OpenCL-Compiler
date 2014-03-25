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

void kernel_109(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang, unsigned long num_worker, unsigned long vector_length
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

void register_experiment(
  char * experiment_db_file,
  int version_id,
  unsigned long num_gang,
  unsigned long num_worker,
  int n, int m, int p
) {
  acc_init_once(); // need to initialize OpenACC's profiling

  char * profiling_db_file_name = acc_profiling_get_db_file_name();
  char * profiling_event_table_name = acc_profiling_get_event_table_name();

  unsigned device_idx = acc_get_device_idx(acc_runtime.curr_device_type, acc_runtime.curr_device_num);

  sqlite3 * experiment_db;
  int status = sqlite3_open(experiment_db_file, &experiment_db);
  assert(status == SQLITE_OK);

  char db_query[1024];
  sprintf(db_query, "insert into experiments values ( '%d' , '%s' , '%s' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d' );",
                    version_id, profiling_db_file_name, profiling_event_table_name, device_idx,
                    num_gang, num_worker, n, m ,p
         );

  char * err_msg;
  status = sqlite3_exec (experiment_db, db_query, NULL, 0, &err_msg);
  assert(status == SQLITE_OK);
}

int main(int argc, char ** argv) {
  assert(argc == 8);

  unsigned long num_gang = atoi(argv[1]);
  unsigned long num_worker = atoi(argv[2]);
  unsigned long vector_length = 1;

  int n = atoi(argv[3]);
  int m = atoi(argv[4]);
  int p = atoi(argv[5]);

  char * experiment_db_file = argv[6];
  int version_id = atoi(argv[7]);

  register_experiment(experiment_db_file, version_id, num_gang, num_worker, n, m, p);

  int i, j;

  float ** a;
  float ** b;
  float ** c;

  init_data(n, m, p, &a, &b, &c);

  kernel_109(n, m, p, a, b, c, num_gang, num_worker, vector_length);

  free_data(n, m, p, a, b, c);

  return 0;
}

/*! @} */

