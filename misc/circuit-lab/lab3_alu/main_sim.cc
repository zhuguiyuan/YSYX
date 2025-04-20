#include <fmt/core.h>

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>
#include <random>
#include <stdexcept>

#include "Vtop.h"

enum class Op { ADD, SUB, NOTA, AND, OR, XOR, CMP, EQ };

struct InTransaction {
  int a;
  int b;
  Op op;

  std::string to_string() const {
    return fmt::format("a: {:04b}, b: {:04b}, op: {:03b}", a & 0xF, b & 0xF,
                       static_cast<int>(op));
  }
};

struct OutTransaction {
  int result;
  int z_flag;
  int c_flag;
  int o_flag;

  std::string to_string() const {
    return fmt::format("r: {:04b}, zf: {}, cf: {}, of: {}", result & 0xF,
                       z_flag, c_flag, o_flag);
  }
};

struct Scoreboard {
  using Checker =
      std::function<void(const InTransaction &, const OutTransaction &)>;
  using ExpectationMaker = std::function<OutTransaction(const InTransaction &)>;

  enum CheckFlag {
    RESULT = 1 << 0,
    Z_FLAG = 1 << 1,
    C_FLAG = 1 << 2,
    OF_FLAG = 1 << 3
  };

  std::map<Op, Checker> checkers{};

  Checker create_checker(const char *op_name, ExpectationMaker get_expectations,
                         int check_flags = RESULT | Z_FLAG | C_FLAG | OF_FLAG) {
    return [=](const InTransaction &in, const OutTransaction &out) {
      const OutTransaction exp_out = get_expectations(in);

      std::string error_msg = fmt::format(
          "{} Failed with input {{{}}}\n[Expected] {}\n[Actual  ] {}\n",
          op_name, in.to_string(), exp_out.to_string(), out.to_string());

      auto check = [&](auto expected, auto actual, const char *name,
                       CheckFlag flag) {
        if (!((check_flags & flag) && expected != actual)) return false;
        return true;
      };

      bool failed = false;
      failed |= check(exp_out.result, out.result, "Result", RESULT);
      failed |= check(exp_out.z_flag, out.z_flag, "Z-Flag", Z_FLAG);
      failed |= check(exp_out.c_flag, out.c_flag, "C-Flag", C_FLAG);
      failed |= check(exp_out.o_flag, out.o_flag, "O-Flag", OF_FLAG);

      if (failed) {
        throw std::runtime_error(error_msg);
      }
    };
  }

  Scoreboard() {
    checkers[Op::ADD] = create_checker("ADD", [](const auto &in) {
      const int sum = in.a + in.b;
      const int res = sum & 0xF;
      const bool of =
          ((in.a & 0x8) == (in.b & 0x8)) && ((in.a & 0x8) != (res & 0x8));
      return OutTransaction{res, res == 0, (sum >> 4) & 1, of};
    });

    checkers[Op::SUB] = create_checker("SUB", [](const auto &in) {
      const int diff = in.a - in.b;
      const int res = diff & 0xF;
      const bool of =
          ((in.a & 0x8) != (in.b & 0x8)) && ((in.a & 0x8) != (res & 0x8));
      return OutTransaction{res, res == 0, in.a >= in.b, of};
    });

    checkers[Op::NOTA] = create_checker(
        "NOTA",
        [](const auto &in) {
          const int res = (~in.a) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::AND] = create_checker(
        "AND",
        [](const auto &in) {
          const int res = (in.a & in.b) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::OR] = create_checker(
        "OR",
        [](const auto &in) {
          const int res = (in.a | in.b) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::XOR] = create_checker(
        "XOR",
        [](const auto &in) {
          const int res = (in.a ^ in.b) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::CMP] = create_checker(
        "CMP",
        [](const auto &in) {
          int8_t a_sign_ext = in.a & 0x8 ? (0xF0 | in.a) : in.a;
          int8_t b_sign_ext = in.b & 0x8 ? (0xF0 | in.b) : in.b;
          const int res = a_sign_ext < b_sign_ext;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::EQ] = create_checker(
        "EQ",
        [](const auto &in) {
          const bool eq = in.a == in.b;
          return OutTransaction{eq ? 1 : 0, !eq, 0, 0};
        },
        RESULT | Z_FLAG);
  }

  void verify(const InTransaction &in, const OutTransaction &out) {
    checkers.at(in.op)(in, out);
  }
};

InTransaction rand_trans() {
  static std::mt19937 gen(std::random_device{}());
  static std::uniform_int_distribution<> val(0, 15), op(0, 7);
  return {val(gen), val(gen), static_cast<Op>(op(gen))};
}

int main(int argc, char **argv) {
  VerilatedContext ctx;
  ctx.commandArgs(argc, argv);
  Vtop top{&ctx};

  Scoreboard scoreboard;

  try {
    for (int i = 0; i < 500000; ++i) {
      const auto in = rand_trans();
      top.a = in.a & 0xF;
      top.b = in.b & 0xF;
      top.op = static_cast<int>(in.op);

      top.eval();

      const OutTransaction out{top.r, top.zf, top.cf, top.of};
      scoreboard.verify(in, out);

      ctx.timeInc(1);
    }
    printf("Simulation completed successfully\n");
    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    fprintf(stderr, "\nERROR: %s\n", e.what());
    return EXIT_FAILURE;
  }
}