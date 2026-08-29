#include "dbg.h"
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef int (*lib_function)(const char *data, size_t length);

int main(int argc, char *argv[]) {
  int rc = 0;
  void *lib = NULL;
  check(argc == 4, "USAGE: ex29 libex29.so function data");

  char *lib_file = argv[1];
  char *func_to_run = argv[2];
  char *data = argv[3];

  lib = dlopen(lib_file, RTLD_NOW);
  check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

  lib_function func = dlsym(lib, func_to_run);
  check(func != NULL, "Did not find %s function in the library %s: %s",
        func_to_run, lib_file, dlerror());

  rc = func(data, strlen(data));
  check(rc == 0, "Function %s return %d for data: %s", func_to_run, rc, data);

  rc = dlclose(lib);
  check(rc == 0, "Failed to close %s", lib_file);

  return 0;

error:
  if (lib != NULL) {
    if (dlclose(lib) != 0) {
      log_err("Failed to close %s: %s", lib_file, dlerror());
    }
  }
  return 1;
}
