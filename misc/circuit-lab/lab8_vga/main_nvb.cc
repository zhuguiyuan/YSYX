#include <Vtop.h>
#include <nvboard.h>

void nvboard_bind_all_pins(Vtop *top);

int main() {
  Vtop top{};

  nvboard_bind_all_pins(&top);
  nvboard_init();

  while (1) {
    top.clk_i = 0;
    top.eval();
    top.clk_i = 1;
    top.eval();
    nvboard_update();
  }
}
