#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include "Vtop.h"
#include <verilated_fst_c.h>

const uint64_t MAX_SIM_CYCLE = 50000000;

int main(int argc, char **argv) {
  VerilatedContext contextp{};
  contextp.commandArgs(argc, argv);
  Vtop top{&contextp};

  auto single_cycle = [&](bool inc_time = true) {
    top.clk = 0;
    top.eval();
    if (inc_time)
      contextp.timeInc(1);
    top.clk = 1;
    top.eval();
    if (inc_time)
      contextp.timeInc(1);
  };

  auto reset = [&](int n) {
    top.rst = 1;
    while (n-- > 0)
      single_cycle(false);
    top.rst = 0;
  };

  reset(10);
  int old_state = top.led;
  printf("Reset done at time %llu\n", contextp.time());
  while (contextp.time() < MAX_SIM_CYCLE) {
    single_cycle();

    if (top.led != old_state) {
      printf("Led changed from %#04x to %#04x at time %llu\n", old_state,
             top.led, contextp.time());
      old_state = top.led;
    }
  }
}
