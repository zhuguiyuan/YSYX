#include "dbg.h"
#include "ex22.h"
#include <stdint.h>

const char *MY_NAME = "Zed A. Shaw";

void scope_demo(int count) {
  log_info("count is: %d", count);

  if (count > 10) {
    int count = 100; // BAD! BUGS!

    log_info("count in this scope is %d", count);
  }

  log_info("count is at exit: %d", count);

  count = 3000;

  log_info("count after assign: %d", count);
}

void *frame_hack(uint64_t foo) {
  void *frame = __builtin_frame_address(0);
  uint64_t bar;
  if (foo != 0) {
    bar = foo;
  }
  log_info("frame = %p", frame);
  log_info("&bar = %p", &bar);
  log_info("bar = %#lx", bar);
  return frame;
}

int main(int argc, char *argv[]) {
  // test out THE_AGE accessors
  log_info("My name: %s, age: %d", MY_NAME, get_age());

  set_age(100);

  log_info("My age is now: %d", get_age());

  // test out THE_SIZE extern
  log_info("THE_SIZE is: %d", THE_SIZE);
  print_size();

  THE_SIZE = 9;

  log_info("THE SIZE is now: %d", THE_SIZE);
  print_size();

  // test the ratio function static
  log_info("Ratio at first: %f", update_ratio(2.0));
  log_info("Ratio again: %f", update_ratio(10.0));
  log_info("Ratio once more: %f", update_ratio(300.0));

  // test the scope demo
  int count = 4;
  scope_demo(count);
  scope_demo(count * 20);

  log_info("count after calling scope_demo: %d", count);

  uint64_t *frame = frame_hack(0x233);
  for (int i = 0; i >= -4; --i) {
    log_info("frame[%d] = %#lx (at %p)", i, frame[i], frame + i);
  }
  frame[-2] = 0x234;
  frame_hack(0);

  return 0;
}
