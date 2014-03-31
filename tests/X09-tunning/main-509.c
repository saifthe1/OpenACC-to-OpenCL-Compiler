
#include "OpenACC/openacc.h"

#include "OpenACC/private/debug.h"
#include "OpenACC/private/region.h"
#include "OpenACC/private/kernel.h"
#include "OpenACC/private/loop.h"
#include "OpenACC/private/data-env.h"
#include "OpenACC/private/memory.h"
#include "OpenACC/private/runtime.h"
#include "OpenACC/private/init.h"

#include "OpenACC/utils/timer.h"
#include "OpenACC/utils/profiling.h"

#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_desc_0_0;
extern struct acc_region_desc_t_ region_desc_0;

struct {
  acc_device_t kind;
  size_t num;
} dev_list[3] = { {acc_device_GTX_460, 0}, {acc_device_e5_2620, 0}, {acc_device_xeonphi, 0} };

unsigned portions[3];

size_t version_by_devices[3];

void kernel_509(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang[3], unsigned long num_worker[3], unsigned long vector_length[3],
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  acc_timer_start(data_timer);

  acc_push_data_environment();

  acc_region_t region_0 = acc_build_region(&region_desc_0);

    region_0->devices[0].num_gang = num_gang[0];
    region_0->devices[0].num_worker = num_worker[0];
    region_0->devices[0].vector_length = vector_length[0];

    region_0->devices[1].num_gang = num_gang[1];
    region_0->devices[1].num_worker = num_worker[1];
    region_0->devices[1].vector_length = vector_length[1];

    region_0->devices[2].num_gang = num_gang[2];
    region_0->devices[2].num_worker = num_worker[2];
    region_0->devices[2].vector_length = vector_length[2];

    region_0->distributed_data[0].ptr  = a[0];
    region_0->distributed_data[0].size = n;
    region_0->distributed_data[1].ptr  = c[0];
    region_0->distributed_data[1].size = n;
    region_0->distributed_data[2].ptr  = b[0];
    region_0->distributed_data[2].size = n;

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

  acc_timer_stop(comp_timer);

  acc_present_or_copyout_regions_(region_0, c[0], n * m *sizeof(float));
 
  acc_pop_data_environment();

  acc_timer_stop(data_timer);
}

void init_data(size_t n, size_t m, size_t p, float *** a, float *** b, float *** c);
void free_data(float ** a, float ** b, float ** c);

int main(int argc, char ** argv) {

  if (argc != 16) {
    printf("Usage: %s mun_gang[3] num_worker[3] portions[3] version_by_devices[3] n m p\n", argv[0]);
    exit(-1);
  }

  size_t num_gang[3] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3])};

  size_t num_worker[3] = {atoi(argv[4]), atoi(argv[5]), atoi(argv[6])};

  portions[0] = atoi(argv[7]);
  portions[1] = atoi(argv[8]);
  portions[2] = atoi(argv[9]);

  version_by_devices[0] = atoi(argv[10]);
  version_by_devices[1] = atoi(argv[11]);
  version_by_devices[2] = atoi(argv[12]);

  size_t n = atoi(argv[13]);
  size_t m = atoi(argv[14]);
  size_t p = atoi(argv[15]);

  size_t vector_length[3] = {1,1,1};

  // Initialize OpenACC (for profiling)
  acc_init_once();

  // Set the experiment (Configure 'Runs' table in DB)
  char * experiment_desc = "gpu_kernel_id INT, cpu_kernel_id INT, mic_kernel_id INT, "\
                           "gpu_portion INT, cpu_portion INT, mic_portion INT, "\
                           "gpu_gang INT, gpu_worker INT, "\
                           "cpu_gang INT, cpu_worker INT, "\
                           "mic_gang INT, mic_worker INT, "\
                           "n INT, m INT, p INT , "\
                           "comp_time BIGINT , data_time BIGINT ";
  acc_profiling_set_experiment(experiment_desc);

  // Add current run in 'Runs' table
  char run_desc[1024];
  sprintf(run_desc, " '%zd' , '%zd' , '%zd' , '%u' , '%u', '%u' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' , '0' , '0' ",
                    version_by_devices[0], version_by_devices[1], version_by_devices[2],
                    portions[0], portions[1], portions[2],
                    num_gang[0], num_worker[0],
                    num_gang[1], num_worker[1],
                    num_gang[2], num_worker[2],
                    n, m, p);
  acc_profiling_new_run(run_desc);

  int i, j;

  float ** a;
  float ** b;
  float ** c;

  init_data(n, m, p, &a, &b, &c);

  acc_timer_t data_timer = acc_timer_build();
  acc_timer_t comp_timer = acc_timer_build();

  kernel_509(n, m, p, a, b, c, num_gang, num_worker, vector_length, data_timer, comp_timer);

  acc_timer_delta(comp_timer);
  acc_timer_delta(data_timer);

  // Update 'Runs' table with host-side timers.
  char db_query[1024];
  sprintf(db_query, "UPDATE Runs SET comp_time='%d', data_time='%d' where rowid='%d'", comp_timer->delta, data_timer->delta, acc_profiler->run_id);
  char * err_msg;
  int status = sqlite3_exec (acc_profiler->db_file, db_query, NULL, 0, &err_msg);
  assert (status == SQLITE_OK);

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

