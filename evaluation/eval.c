
#include <stdio.h>
#include <sqlite3.h>

#include "OpenACC/openacc.h"
#include "OpenACC/utils/timer.h"
#include "OpenACC/internal/kernel.h"

#include <assert.h>

#include KERNEL_FILE

#include INIT_FILE

extern unsigned tiles[4];

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

  assert(argc == 3); // usage: eval db_file comment

  unsigned g, w, v, s;

  unsigned gang_config_num = 5;
  unsigned * gang_configs = malloc(gang_config_num * sizeof(unsigned));
  gang_configs[0] = 16;
  for (g = 1; g < gang_config_num; g++)
    gang_configs[g] = gang_configs[g - 1] * 2;

  unsigned worker_config_num = 6;
  unsigned * worker_configs = malloc(worker_config_num * sizeof(unsigned));
  worker_configs[0] = 32;
  for (w = 1; w < worker_config_num; w++)
    worker_configs[w] = worker_configs[w - 1] * 2;

  unsigned vector_config_num = 1; /// \todo debug vector in opencl context
  unsigned * vector_configs = malloc(vector_config_num * sizeof(unsigned));
  vector_configs[0] = 1;
  for (v = 1; v < vector_config_num; v++)
    vector_configs[v] = vector_configs[v - 1] * 2;

  unsigned size_config_num = 6;
  unsigned long * size_configs = malloc(size_config_num * sizeof(unsigned long));
  size_configs[0] = 16 * 1024 * 1024; // 1M >> g*w*v = 2^6 * 2^7 * 2^0 = 8k , it leaves 2^7 to distribute among the filing tiles
  for (s = 1; s < size_config_num; s++)
    size_configs[s] = size_configs[s-1] * 2;

  if (sqlite3_open(argv[1], &db)) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);
    return(1);
  }

  acc_set_device_type(acc_device_M2070);

  sprintf(
    sql_cmd,
    "insert into experiments values(\"%s\", \"%s\", %u, %u, %u, %u, DATETIME(), \"%s\");",
    kernel_0x00_desc.name, acc_device_name[acc_get_device_type()],
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
    "experiment=%u, kernel=\"%s\", device=\"%s\", t0=%u, t1=%u, t2=%u, t3=%u, comment=\"%s\"\n",
    experiment_id, kernel_0x00_desc.name, acc_device_name[acc_get_device_type()],
    tiles[0], tiles[1], tiles[2], tiles[3], argv[2]
  );

  int fake_argc = 5;
  char * fake_argv[5];
  fake_argv[4] = malloc(20 * sizeof(char));
  for (s = 0; s < size_config_num; s++) {
    for (g = 0; g < gang_config_num; g++) {
      for (w = 0; w < worker_config_num; w++) {
        for (v = 0; v < vector_config_num; v++) {
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

  sqlite3_close(db);

  return 0;
}

