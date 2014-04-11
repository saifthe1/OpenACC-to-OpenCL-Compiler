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
#include "OpenACC/internal/loop.h"

#include "OpenACC/utils/profiling.h"

#include "sqlite3.h"

#include <math.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

#ifndef PRINT_DB_ACCESS
# define PRINT_DB_ACCESS 1
#endif

#ifndef PRINT_INFO
# define PRINT_INFO 0
#endif

typedef struct acc_kernel_t_ * acc_kernel_t;
typedef struct acc_region_t_ * acc_region_t;

extern struct acc_kernel_desc_t_ kernel_desc_0_0;
extern struct acc_region_desc_t_ region_desc_0;

struct gang_worker_pair_t {
  size_t gang;
  size_t worker;
};

void init_data(size_t n, size_t m, size_t p, float *** a, float *** b, float *** c);

void free_data(float ** a, float ** b, float ** c);

void init_args(
  int argc, char ** argv, const size_t max_exp, size_t ** power_of_2,
  size_t * n_exp_min, size_t * n_exp_max, size_t * m_exp_min, size_t * m_exp_max, size_t * p_exp_min, size_t * p_exp_max,
  size_t * gang_exp_min, size_t * gang_exp_max, size_t * worker_exp_min, size_t * worker_exp_max
);

void init_loops(
  struct acc_loop_t_ ** loop_i, struct acc_loop_t_ ** loop_j,
  struct acc_loop_t_ ** gang_loop, struct acc_loop_t_ ** worker_loop
);

void determine_gw_pairs(
  size_t * gw_pairs_count,
  struct gang_worker_pair_t ** gw_pairs,
  size_t n, size_t m, size_t p,
  struct acc_loop_t_ * loop_i, struct acc_loop_t_ * loop_j,
  struct acc_loop_t_ * gang_loop, struct acc_loop_t_ * worker_loop,
  size_t * power_of_2,
  size_t gang_exp_min, size_t gang_exp_max,
  size_t worker_exp_min, size_t worker_exp_max
);

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

int main(int argc, char ** argv) {
  const size_t max_exp = 20;
  size_t * power_of_2;
  size_t n_exp_min;
  size_t n_exp_max;
  size_t m_exp_min;
  size_t m_exp_max;
  size_t p_exp_min;
  size_t p_exp_max;
  size_t gang_exp_min;
  size_t gang_exp_max;
  size_t worker_exp_min;
  size_t worker_exp_max;

  init_args(
    argc, argv, max_exp, &power_of_2,
    &n_exp_min, &n_exp_max, &m_exp_min, &m_exp_max, &p_exp_min, &p_exp_max,
    &gang_exp_min, &gang_exp_max, &worker_exp_min, &worker_exp_max
  );

  // Dertermine which loop is used for gang/worker
  struct acc_loop_t_ * loop_i;
  struct acc_loop_t_ * loop_j;
  struct acc_loop_t_ * gang_loop;
  struct acc_loop_t_ * worker_loop;
  init_loops(&loop_i, &loop_j, &gang_loop, &worker_loop);

  float ** a, ** b, ** c;
  size_t n_exp, m_exp, p_exp;
  for (n_exp = n_exp_min; n_exp <= n_exp_max; n_exp++) {
    for (m_exp = m_exp_min; m_exp <= m_exp_max; m_exp++) {
      for (p_exp = p_exp_min; p_exp <= p_exp_max; p_exp++) {
        size_t n = power_of_2[n_exp];
        size_t m = power_of_2[m_exp];
        size_t p = power_of_2[p_exp];

        // Determine the combinaison of gang/worker that can be used
        size_t gw_pairs_count;
        struct gang_worker_pair_t * gw_pairs;
        determine_gw_pairs(
          &gw_pairs_count, &gw_pairs,
          n, m, p,
          loop_i, loop_j,
          gang_loop, worker_loop,
          power_of_2,
          gang_exp_min, gang_exp_max,
          worker_exp_min, worker_exp_max
        );

        init_data(n, m, p, &a, &b, &c);

        // Iterate over the combinaison of gang/worker
        size_t i;
        for (i = 0; i < gw_pairs_count; i++) {
          size_t num_gang = gw_pairs[i].gang;
          size_t num_worker = gw_pairs[i].worker;
          size_t vector_length = 1;

          char * run_desc = (char *)malloc(1024 * sizeof(char));
          sprintf(run_desc, " '%zd' , '%s' , '%zd' , '%zd' , '%zd' , '%zd' , '%zd' ",
                            kernel_desc_0_0.version_by_devices[0], acc_device_name[acc_runtime.curr_device_type],
                            num_gang, num_worker, n, m, p
                  );
          acc_profiling_new_run(run_desc);

#if PRINT_DB_ACCESS
          printf("[db]     Insert in Runs table: \"%s\"\n", run_desc);
#endif

          kernel_109(n, m, p, a, b, c, num_gang, num_worker, vector_length);

          free(run_desc);
        }

        free_data(a, b, c);

        free(gw_pairs);
      }
    }
  }

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

void init_args(
  int argc, char ** argv,
  const size_t max_exp,
  size_t ** power_of_2,
  size_t * n_exp_min,
  size_t * n_exp_max,
  size_t * m_exp_min,
  size_t * m_exp_max,
  size_t * p_exp_min,
  size_t * p_exp_max,
  size_t * gang_exp_min,
  size_t * gang_exp_max,
  size_t * worker_exp_min,
  size_t * worker_exp_max
) {
  // Precompute power of 2
  size_t i;
  *power_of_2 = (size_t *)malloc((max_exp + 1) * sizeof(size_t));
  (*power_of_2)[0] = 1;
  for (i = 1; i <= max_exp; i++)
    (*power_of_2)[i] = (*power_of_2)[i-1] * 2;

  // Initialize OpenACC (for profiling)
  acc_init_once();

  // Set fields for Runs database
  char * experiment_desc = " version_id INT, acc_device_type CHAR(40), gang INT, worker INT, n INT, m INT, p INT ";
  acc_profiling_set_experiment(experiment_desc);


#if PRINT_DB_ACCESS
  printf("[db]     Runs table are: %s\n", experiment_desc);
#endif

  // Check usage
  if (argc != 12) {
    printf("usage: %s n_exp_min n_exp_max m_exp_min m_exp_max p_exp_min p_exp_max gang_exp_min gang_exp_max worker_exp_min worker_exp_max version_id\n", argv[0]);
    exit(-1);
  }

  // Get size ranges form arg
  *n_exp_min = atoi(argv[1]);
    assert(*n_exp_min <= max_exp);
  *n_exp_max = atoi(argv[2]);
    assert(*n_exp_max <= max_exp);
  *m_exp_min = atoi(argv[3]);
    assert(*m_exp_min <= max_exp);
  *m_exp_max = atoi(argv[4]);
    assert(*m_exp_max <= max_exp);
  *p_exp_min = atoi(argv[5]);
    assert(*p_exp_min <= max_exp);
  *p_exp_max = atoi(argv[6]);
    assert(*p_exp_max <= max_exp);

  // Get number of gangs/workers ranges form arg
  *gang_exp_min = atoi(argv[7]);
    assert(*gang_exp_min <= max_exp);
  *gang_exp_max = atoi(argv[8]);
    assert(*gang_exp_max <= max_exp);
  *worker_exp_min = atoi(argv[9]);
    assert(*worker_exp_min <= max_exp);
  *worker_exp_max = atoi(argv[10]);
    assert(*worker_exp_max <= max_exp);

  // Get the kernel version to be used from args
  size_t * version_by_devices = (size_t *)malloc(1 * sizeof(size_t));
  version_by_devices[0] = atoi(argv[11]);
  kernel_desc_0_0.version_by_devices = version_by_devices;

  assert(version_by_devices[0] < kernel_desc_0_0.num_versions);
  assert(kernel_desc_0_0.versions[version_by_devices[0]] != NULL);

  // Set compilation flags for OpenCL codes
  region_desc_0.num_options = 2;
  region_desc_0.options = malloc(region_desc_0.num_options * sizeof(char *));
  size_t ocl_comp_opt_cnt = 0;

  // Add debug flag
//region_desc_0.options[ocl_comp_opt_cnt++] = "-g";
  region_desc_0.options[ocl_comp_opt_cnt++] = "";

  // Enable the requested version of the kernel
  region_desc_0.options[ocl_comp_opt_cnt] = malloc((10 + strlen(kernel_desc_0_0.name) + strlen(kernel_desc_0_0.versions[version_by_devices[0]]->suffix)) * sizeof(char));
  sprintf(region_desc_0.options[ocl_comp_opt_cnt], "-DENABLE_%s%s", kernel_desc_0_0.name, kernel_desc_0_0.versions[version_by_devices[0]]->suffix);
  ocl_comp_opt_cnt++;
}

void init_loops(
  struct acc_loop_t_ ** loop_i, struct acc_loop_t_ ** loop_j,
  struct acc_loop_t_ ** gang_loop, struct acc_loop_t_ ** worker_loop
) {
  *loop_i = &(kernel_desc_0_0.versions[kernel_desc_0_0.version_by_devices[0]]->loops[0]);
  *loop_j = &(kernel_desc_0_0.versions[kernel_desc_0_0.version_by_devices[0]]->loops[1]);

  *gang_loop = NULL;
  *worker_loop = NULL;

  if ((*loop_i)->num_iterations[e_gang] == 0) {
    assert((*loop_j)->num_iterations[e_gang] == 1);
    *gang_loop = *loop_i;
  }
  if ((*loop_i)->num_iterations[e_worker] == 0) {
    assert((*loop_j)->num_iterations[e_worker] == 1);
    *worker_loop = *loop_i;
  }
  if ((*loop_j)->num_iterations[e_gang] == 0) {
    assert((*loop_i)->num_iterations[e_gang] == 1);
    *gang_loop = *loop_j;
  }
  if ((*loop_j)->num_iterations[e_worker] == 0) {
    assert((*loop_i)->num_iterations[e_worker] == 1);
    *worker_loop = *loop_j;
  }

  assert(*gang_loop != NULL && *worker_loop != NULL);

#if PRINT_INFO
  if (*gang_loop == *worker_loop) {
    printf("[info]   gang_loop = worker_loop = %s : ( %zd , %zd , %zd , %zd , %zd )\n",
               gang_loop == loop_i ? "loop_i" : "loop_j",
               gang_loop->num_iterations[e_tile_0],
               gang_loop->num_iterations[e_gang],
               gang_loop->num_iterations[e_tile_1],
               gang_loop->num_iterations[e_worker],
               gang_loop->num_iterations[e_tile_2]
          );
  }
  else {
    printf("[info]   gang_loop   = %s : ( %zd , %zd , %zd )\n",
               gang_loop == loop_i ? "loop_i" : "loop_j",
               gang_loop->num_iterations[e_tile_0],
               gang_loop->num_iterations[e_gang],
               gang_loop->num_iterations[e_tile_1]
          );
    printf("[info]   worker_loop = %s : ( %zd , %zd , %zd )\n",
               worker_loop == loop_i ? "loop_i" : "loop_j",
               worker_loop->num_iterations[e_tile_1],
               worker_loop->num_iterations[e_worker],
               worker_loop->num_iterations[e_tile_2]
          );
  }
#endif
}

void determine_gw_pairs(
  size_t * gw_pairs_count,
  struct gang_worker_pair_t ** gw_pairs,
  size_t n, size_t m, size_t p,
  struct acc_loop_t_ * loop_i, struct acc_loop_t_ * loop_j,
  struct acc_loop_t_ * gang_loop, struct acc_loop_t_ * worker_loop,
  size_t * power_of_2,
  size_t gang_exp_min, size_t gang_exp_max,
  size_t worker_exp_min, size_t worker_exp_max
) {
  // Determine the combinaison of gang/worker that can be used
  *gw_pairs_count = 0;
  size_t gw_pairs_size = 42;
  *gw_pairs = malloc(gw_pairs_size * sizeof(struct gang_worker_pair_t));

  if (gang_loop == worker_loop) {
    size_t num_static_iteration = (gang_loop->num_iterations[e_tile_0] == 0 ? 1 : gang_loop->num_iterations[e_tile_0]) *
                                  (gang_loop->num_iterations[e_tile_1] == 0 ? 1 : gang_loop->num_iterations[e_tile_1]) *
                                  (gang_loop->num_iterations[e_tile_2] == 0 ? 1 : gang_loop->num_iterations[e_tile_2]);
    size_t total_iteration = 0;
    if (gang_loop == loop_i) total_iteration = n;
    if (gang_loop == loop_j) total_iteration = m;
    assert(total_iteration != 0);
    int loop_has_dynamic_tile = gang_loop->num_iterations[e_tile_0] == 0 ||
                                gang_loop->num_iterations[e_tile_1] == 0 ||
                                gang_loop->num_iterations[e_tile_2] == 0;

#if PRINT_INFO
    printf("[info]     For n=%zd, m=%zd, p=%zd\n", n, m, p);
    printf("[info]       num_static_iteration = %zd, total_iteration = %zd, loop_has_dynamic_tile = %d\n", num_static_iteration, total_iteration, loop_has_dynamic_tile);
#endif

    size_t g_exp, w_exp;
    for (g_exp = gang_exp_min; g_exp <= gang_exp_max; g_exp++) {
      for (w_exp = worker_exp_min; w_exp <= worker_exp_max; w_exp++) {
        if (
             (loop_has_dynamic_tile && power_of_2[g_exp] * power_of_2[w_exp] * num_static_iteration <= total_iteration) ||
             power_of_2[g_exp] * power_of_2[w_exp] * num_static_iteration == total_iteration
        ) {
          assert(*gw_pairs_count < gw_pairs_size);
          (*gw_pairs)[*gw_pairs_count].gang   = power_of_2[g_exp];
          (*gw_pairs)[*gw_pairs_count].worker = power_of_2[w_exp];
          (*gw_pairs_count)++;
          if (*gw_pairs_count == gw_pairs_size) {
            gw_pairs_size *= 2;
            *gw_pairs = realloc(*gw_pairs, gw_pairs_size * sizeof(struct gang_worker_pair_t));
          }
        }
      }
    }
  }
  else {
    size_t num_static_iteration_gang = (gang_loop->num_iterations[e_tile_0] == 0 ? 1 : gang_loop->num_iterations[e_tile_0]) *
                                       (gang_loop->num_iterations[e_tile_1] == 0 ? 1 : gang_loop->num_iterations[e_tile_1]);
    size_t total_iteration_gang = 0;
    if (gang_loop == loop_i) total_iteration_gang = n;
    if (gang_loop == loop_j) total_iteration_gang = m;
    assert(total_iteration_gang != 0);
    int gang_loop_has_dynamic_tile = gang_loop->num_iterations[e_tile_0] == 0 || gang_loop->num_iterations[e_tile_1] == 0;

    size_t num_static_iteration_worker = (worker_loop->num_iterations[e_tile_1] == 0 ? 1 : worker_loop->num_iterations[e_tile_1]) *
                                         (worker_loop->num_iterations[e_tile_2] == 0 ? 1 : worker_loop->num_iterations[e_tile_2]);
    size_t total_iteration_worker = 0;
    if (worker_loop == loop_i) total_iteration_worker = n;
    if (worker_loop == loop_j) total_iteration_worker = m;
    assert(total_iteration_worker != 0);
    int worker_loop_has_dynamic_tile = worker_loop->num_iterations[e_tile_1] == 0 || worker_loop->num_iterations[e_tile_2] == 0;

#if PRINT_INFO
    printf("[info]   num_static_iteration_gang   = %zd, total_iteration_gang   = %zd, gang_loop_has_dynamic_tile   = %d\n",
               num_static_iteration_gang, total_iteration_gang, gang_loop_has_dynamic_tile
          );
    printf("[info]   num_static_iteration_worker = %zd, total_iteration_worker = %zd, worker_loop_has_dynamic_tile = %d\n",
               num_static_iteration_worker, total_iteration_worker, worker_loop_has_dynamic_tile
          );
#endif

    size_t g_exp, w_exp;
    for (g_exp = gang_exp_min; g_exp <= gang_exp_max; g_exp++) {
      for (w_exp = worker_exp_min; w_exp <= worker_exp_max; w_exp++) {
        if ( (
               (gang_loop_has_dynamic_tile && power_of_2[g_exp] * num_static_iteration_gang <= total_iteration_gang) ||
               power_of_2[g_exp] * num_static_iteration_gang == total_iteration_gang
             ) && ( 
               (worker_loop_has_dynamic_tile && power_of_2[w_exp] * num_static_iteration_worker <= total_iteration_worker) ||
               power_of_2[w_exp] * num_static_iteration_worker == total_iteration_worker
             )
        ) {
          assert(*gw_pairs_count < gw_pairs_size);
          (*gw_pairs)[*gw_pairs_count].gang   = power_of_2[g_exp];
          (*gw_pairs)[*gw_pairs_count].worker = power_of_2[w_exp];
          (*gw_pairs_count)++;
          if (*gw_pairs_count == gw_pairs_size) {
            gw_pairs_size *= 2;
            *gw_pairs = realloc(*gw_pairs, gw_pairs_size * sizeof(struct gang_worker_pair_t));
          }
        }
      }
    }
  }
}

/*! @} */

