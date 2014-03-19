
#include <stdio.h>

#include "OpenACC/openacc.h"
#include "OpenACC/utils/timer.h"

#include <assert.h>

extern void kernel_509(
  int n, int m, int p,
  float ** a, float ** b, float ** c,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
);

void read_params(
  int argc, char ** argv,
  int * n, int * m, int * p
) {
  assert(argc == 10);

  *n = atoi(argv[7]);
  *m = atoi(argv[8]);
  *p = atoi(argv[9]);
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

void launch(
  int argc, char ** argv,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
) {
  int n, m, p;

  int i, j;

  read_params(argc, argv, &n, &m, &p);

  float ** a;
  float ** b;
  float ** c;

  init_data(n, m, p, &a, &b, &c);

  kernel_509(n, m, p, a, b, c, num_gang_0, num_worker_0, vector_length_0, num_gang_1, num_worker_1, vector_length_1, data_timer, comp_timer);

  acc_timer_delta(data_timer);
  acc_timer_delta(comp_timer);

  free_data(n, m, p, a, b, c);
}

