#ifndef __state_h_
#define __state_h_

#include <glib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** memory and mmio space */
typedef struct mem_space mem_space_t;

mem_space_t *mem_space_new(void);

void mem_space_free(mem_space_t *m);

/** When mem read address is invalid, exit with error. */
uint32_t mem_space_read(mem_space_t *m, uint32_t addr);

/** @param mask 32-bit bit-enable mask for the write value.
  When mem write address is invalid, exit with error. */
void mem_space_write(mem_space_t *m, uint32_t addr, uint32_t value,
                     uint32_t mask);

/** minirv emulation state */
typedef struct state {
  uint32_t registers[32];
  uint32_t pc;
  mem_space_t *mem;
} state_t;

state_t *state_new(void);

void state_free(state_t *s);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(state_t, state_free)

void print_state(state_t *s);

/** @return (bool) program continue?
  When any error occurs (like access invalid address), exit with error. */
bool inst_cycle(state_t *s);

/** @return (bool) load program success?
  Program is a flat binary file and program's size must <= mem_size */
bool load_program(state_t *s, const char *filename);

#endif