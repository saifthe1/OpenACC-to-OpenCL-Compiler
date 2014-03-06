
#include <stdio.h>

#include "sqlite3.h"

#include "OpenACC/openacc.h"
#include "OpenACC/utils/timer.h"
#include "OpenACC/internal/kernel.h"

#include <assert.h>

extern unsigned tiles[4];
extern unsigned test_id;

extern void launch(
  int argc, char ** argv,
  unsigned long num_gang,
  unsigned long num_worker,
  unsigned long vector_length,
  acc_timer_t data_timer, acc_timer_t comp_timer
);

static int callback(void * not_used, int argc, char ** argv, char ** az_col_name) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", az_col_name[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
 
int main(int argc, char **argv) {
  sqlite3 * db;
  char sql_cmd[256];
  char * err_msg;
  int rc;

  if (argc != 7) {
    printf("usage: %s db_file comment gang worker vector size\n", argv[0]);
    exit(-1);
  }

  if (sqlite3_open(argv[1], &db)) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);
    return(1);
  }

  unsigned gang = atoi(argv[3]);
  unsigned worker = atoi(argv[4]);
  unsigned vector = atoi(argv[5]);
  unsigned size = atoi(argv[6]);

  acc_set_device_type(acc_device_any);

  long base_min_size = 1;
  if (tiles[0] != 0) base_min_size *= tiles[0];
  if (tiles[1] != 0) base_min_size *= tiles[1];
  if (tiles[2] != 0) base_min_size *= tiles[2];
  if (tiles[3] != 0) base_min_size *= tiles[3];

  int fake_argc = 5;
  char * fake_argv[5];
  fake_argv[4] = malloc(20 * sizeof(char));

  long min_size = base_min_size * gang * worker * vector;

  if (size >= min_size) {
    sprintf(fake_argv[4], "%lu", size);

    acc_timer_t data_timer = acc_timer_build();
    acc_timer_t comp_timer = acc_timer_build();

    launch(fake_argc, fake_argv, gang, worker, vector, data_timer, comp_timer);

    sprintf(
      sql_cmd,
      "insert into runs values(%lu, %u, %u, %u, %u, %u, %u, %u, %lu, %lu, DATETIME());",
      size, gang, worker, vector, tiles[0], tiles[1], tiles[2], tiles[3]
      comp_timer->delta, data_timer->delta
    );

    if (sqlite3_exec(db, sql_cmd, callback, 0, &err_msg) != SQLITE_OK) {
      fprintf(stderr, "SQL query: %s\n", sql_cmd);
      fprintf(stderr, "SQL error: %s\n", err_msg);
      sqlite3_free(err_msg);
    }
  }

  sqlite3_close(db);

  return 0;
}

