#include <Vtop.h>
#include <nvboard.h>

struct SimulationState {
  VerilatedContext ctx;
  Vtop top;

  SimulationState() : ctx{}, top{&ctx} {}

  void single_cycle() {
    top.clk = 0;
    top.eval();
    ctx.timeInc(1);
    top.clk = 1;
    top.eval();
    ctx.timeInc(1);
  }

  void reset(int cycle) {
    top.rst = 1;
    while (cycle-- > 0)
      single_cycle();
    top.rst = 0;
  };
};

void nvboard_bind_all_pins(Vtop *top);

int main() {
  auto state = SimulationState{};

  nvboard_bind_all_pins(&state.top);
  nvboard_init();

  state.reset(10);
  while (1) {
    nvboard_update();
    state.single_cycle();
  }
}
