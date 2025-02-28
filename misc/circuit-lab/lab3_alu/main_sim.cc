#include "Vtop.h"
#include <array>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <functional>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>

enum class Op : int { ADD, SUB, NOTA, AND, OR, XOR, CMP, EQ };

template <int width> std::string to_binary(int n) {
  const unsigned mask = (1U << width) - 1;
  return std::bitset<width>(static_cast<unsigned>(n) & mask).to_string();
}

struct InTransaction {
  int a;
  int b;
  Op op;

  std::string str() const {
    char buf[64];
    snprintf(buf, sizeof(buf), "a: %s, b: %s, op: %s", to_binary<4>(a).c_str(),
             to_binary<4>(b).c_str(),
             to_binary<3>(static_cast<int>(op)).c_str());
    return buf;
  }
};

struct OutTransaction {
  int result;
  int z_flag;
  int c_flag;
  int of_flag;

  std::string str() const {
    char buf[64];
    snprintf(buf, sizeof(buf), "res: %s, z: %s, c: %s, of: %s",
             to_binary<4>(result).c_str(), to_binary<1>(z_flag).c_str(),
             to_binary<1>(c_flag).c_str(), to_binary<1>(of_flag).c_str());
    return buf;
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
      OutTransaction exp_out = get_expectations(in);

      std::stringstream ss;
      ss << op_name << " Check Failed\nInput: " << in.str()
         << "\nExpected: " << exp_out.str() << "\nActual: " << out.str()
         << "\n";

      bool failed = false;
      auto check = [&](auto expected, auto actual, const char *name,
                       CheckFlag flag) {
        if (!((check_flags & flag) && expected != actual))
          return false;

        ss << "  " << name << " FAIL: expected " << expected << " got "
           << actual << "\n";
        return true;
      };

      failed |= check(exp_out.result, out.result, "Result", RESULT);
      failed |= check(exp_out.z_flag, out.z_flag, "Z-Flag", Z_FLAG);
      failed |= check(exp_out.c_flag, out.c_flag, "C-Flag", C_FLAG);
      failed |= check(exp_out.of_flag, out.of_flag, "OF-Flag", OF_FLAG);

      if (failed)
        throw std::runtime_error(ss.str());
    };
  }

  Scoreboard() {
    checkers[Op::ADD] = create_checker("ADD", [](auto &in) {
      const int sum = in.a + in.b;
      const int res = sum & 0xF;
      const bool of =
          ((in.a & 0x8) == (in.b & 0x8)) && ((in.a & 0x8) != (res & 0x8));
      return OutTransaction{res, res == 0, (sum >> 4) & 1, of};
    });

    checkers[Op::SUB] = create_checker("SUB", [](auto &in) {
      const int diff = in.a - in.b;
      const int res = diff & 0xF;
      const bool of =
          ((in.a & 0x8) != (in.b & 0x8)) && ((in.a & 0x8) != (res & 0x8));
      return OutTransaction{res, res == 0, in.a >= in.b, of};
    });

    checkers[Op::NOTA] = create_checker(
        "NOTA",
        [](auto &in) {
          const int res = (~in.a) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::AND] = create_checker(
        "AND",
        [](auto &in) {
          const int res = (in.a & in.b) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::OR] = create_checker(
        "OR",
        [](auto &in) {
          const int res = (in.a | in.b) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::XOR] = create_checker(
        "XOR",
        [](auto &in) {
          const int res = (in.a ^ in.b) & 0xF;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::CMP] = create_checker(
        "CMP",
        [](auto &in) {
          int8_t a_sign_ext = in.a & 0x8 ? (0xF0 | in.a) : in.a;
          int8_t b_sign_ext = in.b & 0x8 ? (0xF0 | in.b) : in.b;
          const int res = a_sign_ext < b_sign_ext;
          return OutTransaction{res, res == 0, 0, 0};
        },
        RESULT | Z_FLAG);

    checkers[Op::EQ] = create_checker(
        "EQ",
        [](auto &in) {
          const bool eq = in.a == in.b;
          return OutTransaction{eq ? 1 : 0, !eq, 0, 0};
        },
        RESULT | Z_FLAG);
  }

  void verify(const InTransaction &in, const OutTransaction &out) {
    checkers[in.op](in, out);
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
      const InTransaction in = rand_trans();
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
