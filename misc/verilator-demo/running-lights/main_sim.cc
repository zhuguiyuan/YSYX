#include "Vtop.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <verilated_fst_c.h>

struct SimulationState {
  const uint64_t MAX_SIM_CYCLE = 50000000;
  VerilatedContext ctx;
  Vtop top;

  SimulationState() : ctx{}, top{&ctx} {}

  void single_cycle(bool inc_time = true) {
    top.clk = 0;
    top.eval();
    if (inc_time) {
      ctx.timeInc(1);
    }
    top.clk = 1;
    top.eval();
    if (inc_time) {
      ctx.timeInc(1);
    }
  }

  void reset(int cycle) {
    top.rst = 1;
    while (cycle-- > 0)
      single_cycle(false);
    top.rst = 0;
  };

  bool is_running() { return ctx.time() < MAX_SIM_CYCLE; }
};

int main(int argc, char **argv) {
  SimulationState state{};
  state.reset(10);
  auto &curr_state = state.top.led;
  int old_state = curr_state;
  printf("Reset done at time %lu\n", state.ctx.time());
  while (state.is_running()) {
    state.single_cycle();

    if (curr_state != old_state) {
      printf("Led changed from %#04x to %#04x at time %lu\n", old_state,
             curr_state, state.ctx.time());
      old_state = curr_state;
    }
  }
}
