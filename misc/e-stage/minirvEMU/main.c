#include "dbg.h"
#include "state.h"
#include <am.h>
#include <klib-macros.h>
#include <stdio.h>
#include <stdlib.h>

int main(const char *args) {
  g_autoptr(state_t) s = NULL;
  check(args && *args, "usage: mainargs=<program.bin> make run");

  s = state_new();
  check(load_program(s, args), "failed to load program %s", args);
  ioe_init();

  int cycle_cnt = 0;
  while (inst_cycle(s)) {
    cycle_cnt += 1;
  }

  print_state(s);
  printf("cycle_cnt = %d\n", cycle_cnt);

  return EXIT_SUCCESS;
error:
  return EXIT_FAILURE;
}
