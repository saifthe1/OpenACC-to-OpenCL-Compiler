
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
  char * err_msg = 0;
  int rc;

  assert(argc == 2);

  rc = sqlite3_open(argv[1], &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);
    return(1);
  }

  rc = sqlite3_exec(db, "select * from experiments;", callback, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }

  rc = sqlite3_exec(db, "select * from runs;", callback, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }

  sqlite3_close(db);

  return 0;
}
