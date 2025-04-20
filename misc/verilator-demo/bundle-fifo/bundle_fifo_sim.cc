#include <Vtop.h>
#include <cassert>
#include <cstdio>
#include <memory>
#include <verilated_fst_c.h>

constexpr auto SIM_ITERS = 1000;

int main(void) {
  auto contextp = std::make_unique<VerilatedContext>();
  // Top don't take the ownership of contextp raw ptr
  auto top = std::make_unique<Vtop>(contextp.get());

  // Enable the fst waveform
  Verilated::traceEverOn(true);
  auto tfp = std::make_unique<VerilatedFstC>();
  top->trace(tfp.get(), 99);
  tfp->open("wave.fst");

  // reset the model
  auto reset = [&]() {
    top->rst = 0;
    top->clk = 1;
    top->eval();
    top->enq_valid = 0;
    top->enq_data = 0;
    top->deq_ready = 0;
    tfp->dump(contextp->time());
  };

  auto cycle = [&](std::function<void()> driver) {
    // tick
    contextp->timeInc(1);
    top->clk = 0;
    top->eval();
    tfp->dump(contextp->time());
    // tock
    contextp->timeInc(1);
    top->clk = 1;
    top->eval();
    driver();
    tfp->dump(contextp->time());
  };

  auto driver = [&]() {
    top->enq_data = rand() & 1;
    top->enq_valid = rand() & 1;
    top->deq_ready = rand() & 1;
  };

  reset();
  for (int i = 0; i < SIM_ITERS; ++i) {
    cycle(driver);
  }

  return 0;
}