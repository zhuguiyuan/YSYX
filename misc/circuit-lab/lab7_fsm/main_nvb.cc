#include <Vtop.h>
#include <nvboard.h>

void nvboard_bind_all_pins(Vtop *top);

int main() {
  Vtop top{};

  nvboard_bind_all_pins(&top);
  nvboard_init();

  auto single_cycle = [&]() {
    top.clk_i = 0;
    top.eval();
    top.clk_i = 1;
    top.eval();
  };

  while (1) {
    single_cycle();
    nvboard_update();
  }
}
