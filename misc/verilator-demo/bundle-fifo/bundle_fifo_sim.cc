#include <Vtop.h>
#include <cassert>
#include <cstdio>
#include <verilated_fst_c.h>

constexpr auto SIM_ITERS = 30;

struct SimulationState {
  VerilatedContext ctx;
  Vtop top;
  VerilatedFstC tfp;

  SimulationState() : ctx{}, top{&ctx}, tfp{} {
    // Enable the fst waveform
    Verilated::traceEverOn(true);
    top.trace(&tfp, 99);
    tfp.open("wave.fst");
  }

  void cycle() {
    tfp.dump(ctx.time());
    ctx.timeInc(1);
    top.clk = 0;
    top.eval();
    tfp.dump(ctx.time());
    ctx.timeInc(1);
    top.clk = 1;
    top.eval();
  }

  void randomDrive() {
    top.enq_data = rand() & 1;
    top.enq_valid = rand() & 1;
    top.deq_ready = rand() & 1;
  };

  void reset() {
    top.rst = 1;
    top.enq_valid = 0;
    top.enq_data = 0;
    top.deq_ready = 0;
    for (int i = 0; i < 10; ++i) {
      cycle();
    }
    top.rst = 0;
  }
};

int main(void) {
  SimulationState state{};
  state.reset();
  for (int i = 0; i < SIM_ITERS; ++i) {
    printf("%ld %d %d %d\t", state.ctx.time(), state.top.enq_data,
           state.top.enq_valid, state.top.enq_ready);
    printf("%ld %d %d %d\n", state.ctx.time(), state.top.deq_data,
           state.top.deq_valid, state.top.deq_ready);
    state.randomDrive();
    state.cycle();
  }
  return 0;
}
