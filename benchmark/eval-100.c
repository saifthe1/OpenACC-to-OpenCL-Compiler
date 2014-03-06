
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

  if (argc != 15) {
    printf("usage: %s db_file comment #gang gang[0] #worker worker[0] #vector vector[0] #sizes size[0]\n", argv[0]);
    exit(-1);
  }

  unsigned g, w, v, s;

  unsigned gang_config_num = atoi(argv[3]);
  unsigned * gang_configs = malloc(gang_config_num * sizeof(unsigned));
  gang_configs[0] = atoi(argv[4]);
  for (g = 1; g < gang_config_num; g++)
    gang_configs[g] = gang_configs[g - 1] * 2;

  unsigned worker_config_num = atoi(argv[5]);
  unsigned * worker_configs = malloc(worker_config_num * sizeof(unsigned));
  worker_configs[0] = atoi(argv[6]);
  for (w = 1; w < worker_config_num; w++)
    worker_configs[w] = worker_configs[w - 1] * 2;

  unsigned vector_config_num = atoi(argv[7]);
  unsigned * vector_configs = malloc(vector_config_num * sizeof(unsigned));
  vector_configs[0] = atoi(argv[8]);
  for (v = 1; v < vector_config_num; v++)
    vector_configs[v] = vector_configs[v - 1] * 2;

  unsigned size_config_num = atoi(argv[9]);;
  unsigned long * size_configs = malloc(size_config_num * sizeof(unsigned long));
  size_configs[0] = atoi(argv[10]);
  for (s = 1; s < size_config_num; s++)
    size_configs[s] = size_configs[s-1] * 2;

  if (sqlite3_open(argv[1], &db)) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);
    return(1);
  }

  acc_set_device_type(acc_device_any);

  sprintf(
    sql_cmd,
    "insert into experiments values(\"test-%u\", \"%s\", %u, %u, %u, %u, DATETIME(), \"%s\");",
    test_id, acc_device_name[acc_get_device_type()],
    tiles[0], tiles[1], tiles[2], tiles[3], argv[2]
  );

  if (sqlite3_exec(db, sql_cmd, callback, 0, &err_msg) != SQLITE_OK) {
    fprintf(stderr, "SQL query: %s\n", sql_cmd);
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    exit(-1);
  }

  sqlite3_int64 experiment_id = sqlite3_last_insert_rowid(db);

  printf(
    "experiment=%u, kernel=\"test-%u\", device=\"%s\", t0=%u, t1=%u, t2=%u, t3=%u, comment=\"%s\"\n",
    experiment_id, test_id, acc_device_name[acc_get_device_type()],
    tiles[0], tiles[1], tiles[2], tiles[3], argv[2]
  );

  long base_min_size = 1;
  if (tiles[0] != 0) min_size *= tiles[0];
  if (tiles[1] != 0) min_size *= tiles[1];
  if (tiles[2] != 0) min_size *= tiles[2];
  if (tiles[3] != 0) min_size *= tiles[3];

  int fake_argc = 5;
  char * fake_argv[5];
  fake_argv[4] = malloc(20 * sizeof(char));
  for (s = 0; s < size_config_num; s++) {
    for (g = 0; g < gang_config_num; g++) {
      for (w = 0; w < worker_config_num; w++) {
        for (v = 0; v < vector_config_num; v++) {
          long min_size = base_min_size * gang_configs[g] * worker_configs[w] * vector_configs[v];
          if (size_configs[s] >= min_size) {
            sprintf(fake_argv[4], "%lu", size_configs[s]);

            acc_timer_t data_timer = acc_timer_build();
            acc_timer_t comp_timer = acc_timer_build();

            launch(fake_argc, fake_argv, gang_configs[g], worker_configs[w], vector_configs[v], data_timer, comp_timer);

            printf(
              "size=%lu, gang=%u, worker=%u, vector=%u, t0=%u, t1=%u, t2=%u, t3=%u, comp=%lu, data=%lu\n",
              size_configs[s], gang_configs[g], worker_configs[w], vector_configs[v],
              tiles[0], tiles[1], tiles[2], tiles[3],
              comp_timer->delta, data_timer->delta
            );

            sprintf(
              sql_cmd,
              "insert into runs values(%u, %lu, %u, %u, %u, %lu, %lu, DATETIME());",
              experiment_id,
              size_configs[s], gang_configs[g], worker_configs[w], vector_configs[v],
              comp_timer->delta, data_timer->delta
            );

            if (sqlite3_exec(db, sql_cmd, callback, 0, &err_msg) != SQLITE_OK) {
              fprintf(stderr, "SQL query: %s\n", sql_cmd);
              fprintf(stderr, "SQL error: %s\n", err_msg);
              sqlite3_free(err_msg);
            }
          }
        }
      }
    }
  }

  sqlite3_close(db);

  return 0;
}

