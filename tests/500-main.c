/*!
 * \addtogroup grp_libopenacc_tests
 * @{
 *
 */

#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "OpenACC/openacc.h"
#include "OpenACC/utils/timer.h"

extern void launch(
  int argc, char ** argv,
  unsigned long num_gang_0, unsigned long num_worker_0, unsigned long vector_length_0,
  unsigned long num_gang_1, unsigned long num_worker_1, unsigned long vector_length_1,
  acc_timer_t data_timer, acc_timer_t comp_timer
);

void read_args(
  int argc, char ** argv,
  unsigned long * num_gang_0,
  unsigned long * num_gang_1,
  unsigned long * num_worker_0,
  unsigned long * num_worker_1,
  unsigned long * vector_length_0,
  unsigned long * vector_length_1
) {
  assert(argc >= 4);
  *num_gang_0 = atoi(argv[1]);
  *num_gang_1 = atoi(argv[2]);
  *num_worker_0 = atoi(argv[3]);
  *num_worker_1 = atoi(argv[4]);
  *vector_length_0 = atoi(argv[5]);
  *vector_length_1 = atoi(argv[6]);
}

int main(int argc, char ** argv) {
  unsigned long num_gang_0;
  unsigned long num_gang_1;
  unsigned long num_worker_0;
  unsigned long num_worker_1;
  unsigned long vector_length_0;
  unsigned long vector_length_1;

  acc_timer_t data_timer = acc_timer_build();
  acc_timer_t comp_timer = acc_timer_build();

  read_args(argc, argv, &num_gang_0, &num_worker_0, &vector_length_0, &num_gang_1, &num_worker_1, &vector_length_1);

  launch(argc, argv, num_gang_0, num_worker_0, vector_length_0, num_gang_1, num_worker_1, vector_length_1, data_timer, comp_timer);

  // CSV format : computation time, computation + communication time
  printf("%d,%d", comp_timer->delta, data_timer->delta);

  return 0;
}

/*! @} */

