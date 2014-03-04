
#include <stdio.h>
#include <sqlite3.h>

#include <assert.h>

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
  char * z_err_msg = 0;
  int rc;

  assert(argc == 2);

  rc = sqlite3_open(argv[1], &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);
    return(1);
  }

  const char * cmd_create_experiments_table = "create table experiments(\
                                                 kernel varchar(20),\
                                                 device varchar(50),\
                                                 tile_0 smallint,\
                                                 tile_1 smallint,\
                                                 tile_2 smallint,\
                                                 tile_3 smallint,\
                                                 date DATETIME,\
                                                 comment text\
                                               );";

  rc = sqlite3_exec(db, cmd_create_experiments_table, callback, 0, &z_err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", z_err_msg);

    sqlite3_free(z_err_msg);
  }

  const char * cmd_create_runs_table = "create table runs(\
                                          experiment int,\
                                          size bigint,\
                                          gang tinyint,\
                                          worker tinyint,\
                                          vector tinyint,\
                                          computation_time bigint,\
                                          communication_time bigint,\
                                          date DATETIME\
                                        );";

  rc = sqlite3_exec(db, cmd_create_runs_table, callback, 0, &z_err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", z_err_msg);

    sqlite3_free(z_err_msg);
  }

  sqlite3_close(db);

  return 0;
}

