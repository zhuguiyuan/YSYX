#include <Vtop.h>
#include <nvboard.h>

void nvboard_bind_all_pins(Vtop *top);

int main() {
  VerilatedContext ctx;
  Vtop top{&ctx};

  nvboard_bind_all_pins(&top);
  nvboard_init();

  while (1) {
    top.clk_i = 0;
    top.eval();
    ctx.timeInc(1);
    top.clk_i = 1;
    top.eval();
    ctx.timeInc(1);
    nvboard_update();
  }
}
