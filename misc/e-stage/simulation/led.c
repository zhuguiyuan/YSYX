#include <stdint.h>
#include <stdio.h>

#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)
#define BITMASK(bits) ((1ull << (bits)) - 1)
// similar to x[hi:lo] in verilog
#define BITS(x, hi, lo) (((x) >> (lo)) & BITMASK((hi) - (lo) + 1))
#define DEF_WIRE(name, w) uint64_t name : w
#define DEF_REG(name, w)                                                       \
  uint64_t name : w;                                                           \
  uint64_t CONCAT(name, _next) : w;                                            \
  uint64_t CONCAT(name, _update) : 1
#define EVAL(c, name, val)                                                     \
  do {                                                                         \
    c->CONCAT(name, _next) = (val);                                            \
    c->CONCAT(name, _update) = 1;                                              \
  } while (0)
#define UPDATE(c, name)                                                        \
  do {                                                                         \
    if (c->CONCAT(name, _update)) {                                            \
      c->name = c->CONCAT(name, _next);                                        \
    }                                                                          \
  } while (0)

typedef struct {
  DEF_WIRE(clk, 1);
  DEF_WIRE(rst, 1);
  DEF_REG(led, 16);
  DEF_REG(count, 32);
} Circuit;
static Circuit circuit;

static void cycle(Circuit *c) {
  c->led_update = 0;
  c->count_update = 0;
  if (c->rst) {
    EVAL(c, led, 1);
    EVAL(c, count, 0);
  } else {
    if (c->count == 0) {
      EVAL(c, led, (BITS(c->led, 14, 0) << 1) | BITS(c->led, 15, 15));
    }
    EVAL(c, count, c->count >= 5000000 ? 0 : c->count + 1);
  }
  UPDATE(c, led);
  UPDATE(c, count);
}

static void reset(Circuit *c) {
  c->rst = 1;
  cycle(c);
  c->rst = 0;
}

static void display(Circuit *c) {
  static uint16_t last_led = 0;
  if (last_led != c->led) { // only update display when c->led changes
    for (int i = 0; i < 16; i++) {
      putchar(BITS(c->led, i, i) ? 'o' : '.');
    }
    putchar('\r');
    fflush(stdout);
    last_led = c->led;
  }
}

int main() {
  reset(&circuit);
  while (1) {
    cycle(&circuit);
    display(&circuit);
  }
  return 0;
}
