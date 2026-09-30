#include <Vtop.h>
#include <nvboard.h>

void nvboard_bind_all_pins(Vtop *top);

int main() {
  auto top = Vtop{};

  nvboard_bind_all_pins(&top);
  nvboard_init();

  while (1) {
    nvboard_update();
    top.eval();
  }
}
