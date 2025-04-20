#include <Vtop.h>
#include <nvboard.h>

void nvboard_bind_all_pins(Vtop *top);

int main() {
  auto top = Vtop{};
  auto single_cycle = [&]() {
    top.clk = 0;
    top.eval();
    top.clk = 1;
    top.eval();
  };

  auto reset = [&](int n) {
    top.rst = 1;
    while (n-- > 0) {
      single_cycle();
    }
    top.rst = 0;
  };

  nvboard_bind_all_pins(&top);
  nvboard_init();

  reset(10);
  while (1) {
    nvboard_update();
    single_cycle();
  }
}
