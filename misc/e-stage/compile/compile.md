# 编译

## 了解编译的过程

clang 的编译流程大致分为：
- 驱动程序：clang 实际上是控制其他工具进行的一个小驱动器，一般不直接和驱动交互
- 预处理：处理宏、#include、预处理指令，输出通常是 .i/.ii/.mi/.mii
- 语法与语义分析：生成 AST，检查类型和语法错误
- 代码生成与优化：把 AST 变成 LLVM IR，再生成汇编或机器码
- 汇编：生成 .o 目标文件
- 链接：生成可执行文件或动态库

还内置了 Clang Static Analyzer，用来做静态分析找 bug。

`clang -fsyntax-only` 只做预处理、解析和语义分析；`-emit-llvm` 输出 LLVM 格式。
`-Xclang -dump-tokens` 是传递给 clang `-dump-tokens` 参数，类似还有 `-Xanalyzer`、`-Xassember`、`-Xlinker`、`-Xpreprocessor`。
