#include <Vtop.h>
#include <Vtop___024root.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct GoldenModel {
  uint8_t pc;
  uint8_t regs[4];
  uint8_t mem[256];

  void print_state() {
    printf("> PC = %d, INST = 0x%08b\n", pc, mem[pc]);
    for (int i = 0; i < 4; ++i) {
      printf("R[%d] = %3d (0x%02x)\n", i, regs[i], regs[i]);
    }
  }

  void init_program() {
    pc = 0;
    memset(regs, 0, sizeof(regs));
    uint8_t prog[256] = {
        0b10000110, // li r0, 8
        0b10010010, // li r1, 2
        0b00000001, // add r0, r0, r1
        0b10010000, // li r1, 0
        0b10100000, // li r2, 0
        0b10110001, // li r3, 1
        0b00010111, // add r1, r1, r3
        0b00101001, // add r2, r2, r1
        0b11111001, // bner0 r1, -2
        0b01101000, // io led
        0b01101001, // io seg
        0b10000000, // li r0, 0
        0b11000011  // bner0 r3, 0
    };
    for (int i = 0; i < 16; ++i) {
      mem[i] = prog[i];
    }
  }

  /** @return (bool) program continue? */
  bool inst_cycle() {
    uint8_t inst = mem[pc];
    uint8_t rd = (inst >> 4) & 0b11;
    uint8_t rs1 = (inst >> 2) & 0b11;
    uint8_t rs2 = (inst >> 0) & 0b11;
    uint8_t imm = (inst >> 0) & 0b11;
    uint8_t s = (inst >> 2) & 0b11;
    uint8_t offset = (inst >> 2) & 0b1111;
    uint8_t sign_ext_offset = (int8_t)(offset << 4) >> 4;

    uint8_t new_pc = pc + 1;

    switch (inst >> 6) {
    case 0b00:
      regs[rd] = regs[rs1] + regs[rs2];
      break;
    case 0b01:
      break;
    case 0b10:
      regs[rd] = imm << (s << 1);
      break;
    case 0b11:
      if (regs[0] != regs[rs2]) {
        new_pc = pc + sign_ext_offset;
      }
      break;
    }
    if (pc == new_pc) {
      return false;
    }
    pc = new_pc;
    return true;
  }
};

struct SimulationState {
  VerilatedContext ctx;
  Vtop top;
  SimulationState(int argc, char *argv[]) : ctx{}, top{&ctx} {
    ctx.commandArgs(argc, argv);
  }

  void cycle() {
    ctx.timeInc(1);
    top.clk_i = 0;
    top.eval();
    ctx.timeInc(1);
    top.clk_i = 1;
    top.eval();
  }

  void reset() {
    top.rst_ni = 0;
    for (int i = 0; i < 10; ++i) {
      cycle();
    }
    top.rst_ni = 1;
  }
};

void check_state(GoldenModel &model, SimulationState &state) {
  // check gpr
  auto ref_regs = model.regs;
  auto &dut_regs =
      state.top.rootp->sCPU_top__DOT__u_core__DOT__u_grp__DOT__reg_file;
  for (int i = 0; i < 4; ++i) {
    if (ref_regs[i] != dut_regs[i]) {
      printf("GPR mismatch at R[%d]: ref = %3d (0x%02x), dut = %3d (0x%02x)\n",
             i, ref_regs[i], ref_regs[i], dut_regs[i], dut_regs[i]);
      exit(EXIT_FAILURE);
    }
  }
  // check pc
  uint8_t ref_pc = model.pc;
  uint8_t dut_pc =
      state.top.rootp->sCPU_top__DOT__u_core__DOT__u_fetcher__DOT__pc_val;
  if (ref_pc != dut_pc) {
    printf("PC mismatch: ref = %3d (0x%02x), dut = %3d (0x%02x)\n", ref_pc,
           ref_pc, dut_pc, dut_pc);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  SimulationState state{argc, argv};
  GoldenModel model{};

  model.init_program();
  state.reset();
  bool finish = false;
  do {
    finish = !model.inst_cycle();
    state.cycle();
    check_state(model, state);
  } while (!finish);
  puts("SUCCESS!");
  model.print_state();

  return EXIT_SUCCESS;
}