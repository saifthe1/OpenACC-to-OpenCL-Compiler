
#include <stdio.h>

#include "OpenACC/openacc.h"
#include "OpenACC/utils/timer.h"

#include <assert.h>

extern void kernel_501(
  unsigned long n, float * a, float offset,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
);

void read_params(
  int argc, char ** argv,
  unsigned long * n,
  float * offset
) {
  assert(argc == 8 || argc == 9);

  *n = atoi(argv[7]);

  if (argc == 9)
    *offset = atof(argv[8]);
  else
    *offset = 10.58436;
}

void init_datas(unsigned long n, float ** a) {
  *a = (float*)malloc(n * sizeof(float));
  unsigned long i;
  for (i = 0; i < n; i++)
    (*a)[i] = rand();
}

void launch(
  int argc, char ** argv,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  unsigned long n;
  float offset;

  read_params(argc, argv, &n, &offset);

  float * a;

  init_datas(n, &a);

  kernel_501(n, a, offset, num_gang_0, num_worker_0, vector_length_0, num_gang_1, num_worker_1, vector_length_1, data_timer, comp_timer);

  acc_timer_delta(data_timer);
  acc_timer_delta(comp_timer);

  free(a);
}

