#define _GNU_SOURCE
#include "dbg.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int end_with(const char *str, const char *suffix) {
  unsigned long str_len = strlen(str);
  unsigned long suffix_len = strlen(suffix);
  if (suffix_len > str_len) {
    return 0;
  }
  return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

int end_with_tests(const struct dirent *item) {
  return item->d_type == DT_REG && end_with(item->d_name, "_tests");
}

int main(void) {
  int retval = 0;
  puts("Running unit tests:");

  const char *valgrind = getenv("VALGRIND");
  if (valgrind == NULL) {
    valgrind = "";
  }

  struct dirent **namelist = NULL;
  int n = scandir("tests", &namelist, end_with_tests, alphasort);
  check(n != -1, "main: failed to scan_dir");
  if (n == 0) {
    puts("No test found");
    goto cleanup;
  }

  for (int i = 0; i < n; ++i) {
    char *cmd_buffer = NULL;
    int rc = asprintf(&cmd_buffer, "%s ./tests/%s 2>> tests/tests.log",
                      valgrind, namelist[i]->d_name);
    check(rc != -1, "failed to build cmd_buffer");

    if (system(cmd_buffer) == 0) {
      printf("tests/%s PASS\n", namelist[i]->d_name);
    } else {
      printf("ERROR in test tests/%s: here's tests/tests.log\n",
             namelist[i]->d_name);
      puts("------");
      system("tail tests/tests.log");
      retval = 1;
    }

    free(cmd_buffer);
    if (retval == 1) {
      break;
    }
  }

cleanup:
  if (namelist != NULL) {
    for (int i = 0; i < n; ++i) {
      free(namelist[i]);
    }
    free(namelist);
  }
  return retval;

error:
  retval = -1;
  goto cleanup;
}