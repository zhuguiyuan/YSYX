#include "dbg.h"
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef int (*lib_function)(const char *data, size_t length);

int test(const char *lib_file, const char *func_to_run, const char *data) {
  int rc = 0;

  void *lib = dlopen(lib_file, RTLD_LAZY);
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

int main(void) {
  const char *lib_file = "build/libex29.so";
  check(test(lib_file, "print_a_message", "hello there") == 0, "");
  check(test(lib_file, "uppercase", "hello there") == 0, "");
  check(test(lib_file, "lowercase", "HELLO tHeRe") == 0, "");
  check(test(lib_file, "fail_on_purpose", "i fail") == 1, "");
  check(test(lib_file, "fail_on_purpose", "") == 1, "");
  check(test(lib_file, "adfasfasdf", "adfasfasdf") == 1, "");
  check(test("build/libex.so", "adfasfasdf", "adfasfasdf") == 1, "");
  return 0;

error:
  return 1;
}
