#include <Vtop.h>
#include <cassert>
#include <cstdio>
#include <memory>
#include <verilated_fst_c.h>

constexpr auto SIM_ITERS = 1000;

struct SimulationState {
  std::unique_ptr<VerilatedContext> ctx;
  std::unique_ptr<Vtop> top;
  std::unique_ptr<VerilatedFstC> tfp;

  SimulationState(bool trace = true, const char *trace_file = "wave.fst") {
    ctx = std::make_unique<VerilatedContext>();
    top = std::make_unique<Vtop>(ctx.get());
    if (trace) { // Enable the fst waveform
      Verilated::traceEverOn(true);
      tfp = std::make_unique<VerilatedFstC>();
      top->trace(tfp.get(), 99);
      tfp->open(trace_file);
    }
  }

  ~SimulationState() {
    if (tfp) {
      top->final();
      tfp->close();
    }
  }

  void time_inc(uint64_t t) { ctx->timeInc(t); }
  void trace_wave() {
    if (tfp)
      tfp->dump(ctx->time());
  }
};

int main(void) {
  SimulationState state{};
  auto &top = *state.top;

  for (int i = 0; i < SIM_ITERS; ++i) {
    state.time_inc(1);
    int a = rand() & 1;
    int b = rand() & 1;
    top.a = a;
    top.b = b;
    top.eval();
    state.trace_wave();
    if (top.f != (a ^ b)) {
      printf("Error: a = %d, b = %d, f = %d\n", a, b, top.f);
      assert(false);
    }
  }
  printf("All test passed!\n");
  return 0;
}