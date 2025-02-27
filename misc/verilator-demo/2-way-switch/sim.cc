#include <Vtop.h>
#include <cassert>
#include <cstdio>
#include <memory>
#include <verilated_fst_c.h>

constexpr auto SIM_ITERS = 1000;

int main(void) {
  // Attention the ownership here.
  auto contextp = std::make_unique<VerilatedContext>();
  auto top = std::make_unique<Vtop>(contextp.get());

  // Enable the fst waveform
  Verilated::traceEverOn(true);
  auto tfp = std::make_unique<VerilatedFstC>();
  top->trace(tfp.get(), 99);
  tfp->open("wave.fst");

  // Sim steps
  for (int i = 0; i < SIM_ITERS; ++i) {
    contextp->timeInc(1);
    int a = rand() & 1;
    int b = rand() & 1;
    top->a = a;
    top->b = b;
    top->eval();
    tfp->dump(contextp->time());
    if (top->f != (a ^ b)) {
      printf("Error: a = %d, b = %d, f = %d\n", a, b, top->f);
      assert(false);
    }
  }
  printf("All test passed!\n");
  return 0;
}