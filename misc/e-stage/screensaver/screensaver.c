#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdbool.h>
#include <stdint.h>

void draw(uint32_t color) {
  int height = io_read(AM_GPU_CONFIG).height;
  int width = io_read(AM_GPU_CONFIG).width;
  k_autofree uint32_t *buf = malloc(height * width * sizeof(uint32_t));
  uint32_t *curr = buf;
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      *curr++ = color;
    }
  }
  io_write(AM_GPU_FBDRAW, 0, 0, buf, width, height, false);
}

static inline uint8_t rand_color_component() {
  return rand() & 0x1 ? 0x00 : 0xFF;
}

typedef struct color {
  uint8_t r, g, b;
} color_t;

color_t color_random_pure() {
  return (color_t){
      rand_color_component(),
      rand_color_component(),
      rand_color_component(),
  };
}

color_t color_mix(color_t c0, color_t c1, int i, int k) {
  uint8_t r, g, b;
  r = (c0.r * (k - i) + c1.r * i) / k;
  g = (c0.g * (k - i) + c1.g * i) / k;
  b = (c0.b * (k - i) + c1.b * i) / k;
  return (color_t){r, g, b};
};

bool color_eq(color_t c0, color_t c1) {
  return c0.r == c1.r && c0.g == c1.g && c0.b == c1.b;
}

uint32_t color_to_code(color_t c) { return (c.r << 16) | (c.g << 8) | (c.b); }

struct {
  color_t curr;
  color_t next;
  uint32_t i;
  uint32_t k;
  uint32_t FPS;
  bool has_kbd;
  bool fast;
} state;

void state_init() {
  AM_TIMER_RTC_T time = io_read(AM_TIMER_RTC);
  state.curr = color_random_pure();
  state.next = color_random_pure();
  while (color_eq(state.curr, state.next)) {
    state.next = color_random_pure();
  }
  state.k = 255;
  state.i = 0;
  state.FPS = 60;
  state.has_kbd = io_read(AM_INPUT_CONFIG).present;
  state.fast = false;
  srand(time.second);
}

void state_update() {
  if (state.has_kbd) {
    AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
    if (ev.keycode == AM_KEY_ESCAPE)
      halt(0);
    if (ev.keycode != AM_KEY_NONE) {
      state.fast = ev.keydown ? true : false;
    }
  }
  state.i += state.fast ? 4 : 1;
  if (state.i >= state.k) {
    state.i = 0;
    state.curr = state.next;
    do {
      state.next = color_random_pure();
    } while (color_eq(state.next, state.curr));
  }
}

void state_render() {
  color_t curr_color = color_mix(state.curr, state.next, state.i, state.k);
  uint32_t color_code = color_to_code(curr_color);
  draw(color_code);
}

int main(const char *args) {
  ioe_init(); // initialization for GUI
  unsigned long last = 0;
  unsigned long fps_last = 0;
  int fps = 0;

  state_init();
  while (1) {
    unsigned long upt = io_read(AM_TIMER_UPTIME).us / 1000;
    if (upt - last > 1000 / state.FPS) {
      state_update();
      state_render();
      last = upt;
      fps++;
    }
    if (upt - fps_last > 1000) {
      // display fps every 1s
      printf("%d: FPS = %d\n", upt, fps);
      fps_last = upt;
      fps = 0;
    }
  }
  return 0;
}
