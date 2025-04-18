#include <Vtop.h>
#include <nvboard.h>

void nvboard_bind_all_pins(Vtop *top);

int main() {
  Vtop top{};

  nvboard_bind_all_pins(&top);
  nvboard_init();

  auto single_cycle = [&]() {
    top.clk = 0;
    top.eval();
    top.clk = 1;
    top.eval();
  };

  while (1) {
    single_cycle();
    nvboard_update();
  }
}
