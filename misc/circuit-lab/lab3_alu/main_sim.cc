#include "Vtop.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

enum class AluOpCode { ADD, SUB, NOTA, AND, OR, XOR, CMP, EQ };

struct AluInputTrans {
  AluOpCode op;
  uint8_t a;
  uint8_t b;

  std::string to_string() const {
    char buf[64];
    snprintf(buf, sizeof(buf), "op: %03b a: %04b b: %04b", static_cast<int>(op),
             a & 0xF, b & 0xF);
    return std::string(buf);
  }

  static AluInputTrans random() {
    AluOpCode op = AluOpCode{rand() & 0x7};
    uint8_t a = rand() & 0xF;
    uint8_t b = rand() & 0xF;
    return {op, a, b};
  }
};

struct AluOutTrans {
  uint8_t result;
  uint8_t z_flag;
  uint8_t c_flag;
  uint8_t o_flag;

  std::string to_string() const {
    char buf[128];
    snprintf(buf, sizeof(buf), "r: %04b, zf: %d, cf: %d, of: %d", result & 0xF,
             z_flag, c_flag, o_flag);
    return std::string(buf);
  }
};

struct ScoreBoard {
  bool equal_under_flags(AluOutTrans lhs, AluOutTrans rhs, bool check_result,
                         bool check_z_flag, bool check_c_flags,
                         bool check_o_flag) {
    if (check_result && lhs.result != rhs.result)
      return false;
    if (check_z_flag && lhs.z_flag != rhs.z_flag)
      return false;
    if (check_c_flags && lhs.c_flag != rhs.c_flag)
      return false;
    if (check_o_flag && lhs.o_flag != rhs.o_flag)
      return false;
    return true;
  }

  AluOutTrans golden_eval(AluInputTrans trans_in) {
    const uint8_t a = trans_in.a & 0xF;
    const uint8_t b = trans_in.b & 0xF;

    switch (trans_in.op) {
    case AluOpCode::ADD: {
      const int sum = a + b;
      const uint8_t res = sum & 0xF;
      const uint8_t cf = (sum >> 4) & 1;
      const bool of = ((a & 0x8) == (b & 0x8)) && ((a & 0x8) != (res & 0x8));
      return {res, res == 0, cf, of};
    }
    case AluOpCode::SUB: {
      const int diff = a - b;
      const uint8_t res = diff & 0xF;
      const bool of = ((a & 0x8) != (b & 0x8)) && ((a & 0x8) != (res & 0x8));
      return {res, res == 0, a >= b, of}; // cf 为 1 表示无借位
    }
    case AluOpCode::NOTA: {
      const uint8_t res = ~a & 0xF;
      return {res, res == 0, 0, 0};
    }
    case AluOpCode::AND: {
      const uint8_t res = a & b;
      return {res, res == 0, 0, 0};
    }
    case AluOpCode::OR: {
      const uint8_t res = a | b;
      return {res, res == 0, 0, 0};
    }
    case AluOpCode::XOR: {
      const uint8_t res = a ^ b;
      return {res, res == 0, 0, 0};
    }
    case AluOpCode::CMP: {
      const int a_signed = (a & 0x8) ? a - 16 : a; // 按 4 位有符号数比较
      const int b_signed = (b & 0x8) ? b - 16 : b;
      const uint8_t res = a_signed < b_signed;
      return {res, res == 0, 0, 0};
    }
    case AluOpCode::EQ: {
      const bool eq = a == b;
      return {eq, !eq, 0, 0};
    }
    }
    return {};
  }

  bool check_equal(AluInputTrans simu_in, AluOutTrans simu_out) {
    auto model_out = golden_eval(simu_in);

    // 只有加减法才会用到进位和溢出标志
    bool check_cf_of = false;
    switch (simu_in.op) {
    case AluOpCode::ADD:
    case AluOpCode::SUB:
      check_cf_of = true;
      break;
    case AluOpCode::NOTA:
    case AluOpCode::AND:
    case AluOpCode::OR:
    case AluOpCode::XOR:
    case AluOpCode::CMP:
    case AluOpCode::EQ:
      break;
    }

    if (equal_under_flags(model_out, simu_out, true, true, check_cf_of,
                          check_cf_of)) {
      return true;
    }

    printf("Failed with input {%s}\n[Expected] %s\n[Actual  ] %s\n",
           simu_in.to_string().c_str(), model_out.to_string().c_str(),
           simu_out.to_string().c_str());
    return false;
  }
};

struct SimulationState {
  VerilatedContext ctx;
  Vtop top;

  SimulationState(int argc, char **argv) : ctx{}, top{&ctx} {
    ctx.commandArgs(argc, argv);
  }

  AluOutTrans eval(const AluInputTrans &in) {
    top.a = in.a & 0xF;
    top.b = in.b & 0xF;
    top.op = static_cast<int>(in.op);
    top.eval();
    ctx.timeInc(1);
    return {top.r, top.zf, top.cf, top.of};
  }
};

int main(int argc, char **argv) {
  srand(time(NULL));
  SimulationState state{argc, argv};
  ScoreBoard board{};

  for (int i = 0; i < 500000; ++i) {
    auto simu_in = AluInputTrans::random();
    auto simu_out = state.eval(simu_in);
    if (!board.check_equal(simu_in, simu_out)) {
      return EXIT_FAILURE;
    }
  }
  printf("Simulation completed successfully\n");
  return EXIT_SUCCESS;
}
