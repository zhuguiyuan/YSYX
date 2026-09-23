#include "state.h"
#include "dbg.h"
#include "inst.h"
#include <am.h>
#include <glib.h>
#include <klib-macros.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// mem_space
typedef struct mem_space {
  uint32_t *screen;
  uint32_t screen_height_pixel;
  uint32_t screen_width_pixel;

  uint32_t *memory;
  uint32_t memory_size_byte;
} mem_space_t;

mem_space_t *mem_space_new(void) {
  mem_space_t *m = g_new0(mem_space_t, 1);
  m->screen = g_new0(uint32_t, 256 * 256);
  m->screen_height_pixel = 256;
  m->screen_width_pixel = 256;

  m->memory = g_new0(uint32_t, (1 << 20));
  m->memory_size_byte = sizeof(uint32_t) * (1 << 20);
  return m;
}

void mem_space_free(mem_space_t *m) {
  if (m != NULL) {
    g_free(m->screen);
    g_free(m->memory);
    g_free(m);
  }
}

static inline bool mem_space_in_screen(const mem_space_t *m, uint32_t addr) {
  uint32_t screen_size =
      sizeof(uint32_t) * m->screen_width_pixel * m->screen_height_pixel;
  return 0x20000000 <= addr && addr < 0x20000000 + screen_size;
}

static inline bool mem_space_in_memory(const mem_space_t *m, uint32_t addr) {
  return addr < m->memory_size_byte;
}

uint32_t mem_space_read(mem_space_t *m, uint32_t addr) {
  if (mem_space_in_memory(m, addr)) {
    return m->memory[addr >> 2];
  } else if (mem_space_in_screen(m, addr)) {
    sentinel("addr 0x%08x reading screen space is not supported", addr);
  } else {
    sentinel("addr 0x%08x out of memory space", addr);
  }
error:
  halt(EXIT_FAILURE);
}

void mem_space_write(mem_space_t *m, uint32_t addr, uint32_t value,
                     uint32_t mask) {
  if (mem_space_in_memory(m, addr)) {
    uint32_t rev_mask = ~mask;
    uint32_t old_value = m->memory[addr >> 2];
    uint32_t new_value = (old_value & rev_mask) | (value & mask);
    m->memory[addr >> 2] = new_value;
  } else if (mem_space_in_screen(m, addr)) {
    m->screen[(addr >> 2) & 0xFFFF] = value;
    io_write(AM_GPU_FBDRAW, 0, 0, m->screen, m->screen_width_pixel,
             m->screen_height_pixel, false);
  } else {
    sentinel("addr 0x%08x out of memory space", addr);
  }
  return;
error:
  halt(EXIT_FAILURE);
}

// state
state_t *state_new(void) {
  state_t *s = g_new0(state_t, 1);
  s->mem = mem_space_new();
  return s;
}

void state_free(state_t *s) {
  if (s != NULL) {
    mem_space_free(s->mem);
    g_free(s);
  }
}

void print_state(state_t *s) {
  uint32_t binary = mem_space_read(s->mem, s->pc);
  inst_t inst = inst_decode_from_binary(binary);
  g_autofree char *str = inst_asprint(&inst);

  printf("> PC = 0x%08x, INST = 0x%08x, STR = %s\n", s->pc, binary, str);
  for (int i = 0; i < 32; i += 4) {
    for (int j = 0; j < 4; ++j) {
      printf("%4s = %10u (0x%08x)%s", reg_name(i + j), s->registers[i + j],
             s->registers[i + j], j == 3 ? "\n" : ", ");
    }
  }
}

bool inst_cycle(state_t *s) {
  check((s->pc & 0b11) == 0, "misaligned pc: 0x%08x", s->pc);
  check(mem_space_in_memory(s->mem, s->pc), "pc 0x%08x out of memory", s->pc);

  uint32_t binary = mem_space_read(s->mem, s->pc);
  inst_t inst = inst_decode_from_binary(binary);

  uint32_t *regs = s->registers;
  uint32_t pc_next = s->pc + 4;
  switch (inst.kind) {
  case INST_ADD:
    regs[inst.rd] = regs[inst.rs1] + regs[inst.rs2];
    break;
  case INST_ADDI:
    regs[inst.rd] = regs[inst.rs1] + inst.imm;
    break;
  case INST_LUI:
    regs[inst.rd] = inst.imm;
    break;
  case INST_LW: {
    uint32_t addr = regs[inst.rs1] + inst.imm;
    check((addr & 0b11) == 0, "misaligned lw address: 0x%08x", addr);
    regs[inst.rd] = mem_space_read(s->mem, addr);
    break;
  }
  case INST_LBU: {
    uint32_t addr = regs[inst.rs1] + inst.imm;
    uint32_t mem_word = mem_space_read(s->mem, addr);
    regs[inst.rd] = ((uint8_t *)&mem_word)[addr & 0b11];
    break;
  }
  case INST_SW: {
    uint32_t addr = regs[inst.rs1] + inst.imm;
    check((addr & 0b11) == 0, "misaligned sw address: 0x%08x", addr);
    mem_space_write(s->mem, addr, regs[inst.rs2], 0xFFFFFFFF);
    break;
  }
  case INST_SB: {
    uint32_t addr = regs[inst.rs1] + inst.imm;
    uint32_t mask = 0xFF << ((addr & 0b11) * 8);
    uint32_t value = (regs[inst.rs2] & 0xFF) << ((addr & 0b11) * 8);
    mem_space_write(s->mem, addr, value, mask);
    break;
  }
  case INST_JALR: {
    uint32_t target = (regs[inst.rs1] + inst.imm) & ~1u;
    check((target & 0b11) == 0, "misaligned jalr target: 0x%08x", target);
    regs[inst.rd] = s->pc + 4;
    pc_next = target;
    break;
  }
  case INST_EBREAK:
    return false;
  }
  regs[0] = 0; /* x0 恒为 0 */
  s->pc = pc_next;
  return true;
error:
  halt(EXIT_FAILURE);
}

G_DEFINE_AUTOPTR_CLEANUP_FUNC(FILE, fclose)
bool load_program(state_t *s, const char *filename) {
  g_autoptr(FILE) file = fopen(filename, "rb");
  check(file != NULL, "failed to open %s", filename);

  uint8_t buf[4];
  uint32_t addr = 0;
  while (fread(buf, 1, sizeof(buf), file) == sizeof(buf)) {
    check(mem_space_in_memory(s->mem, addr), "program too large for memory");
    /* 小端序打包成 32 位指令字 */
    uint32_t value = (uint32_t)buf[0] | (uint32_t)buf[1] << 8 |
                     (uint32_t)buf[2] << 16 | (uint32_t)buf[3] << 24;
    mem_space_write(s->mem, addr, value, 0xFFFFFFFF);
    addr += 4;
  }
  check(feof(file), "failed to read %s", filename);

  s->pc = 0;
  return true;
error:
  return false;
}
