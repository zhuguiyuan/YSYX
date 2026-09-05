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

## 程序优化

类似 `f()` 这样的函数调用表达式，因为不确定是否有副作用，一般是没有办法优化的。
除非编译器可以确定或者我们手动标准这是一个 pure function。
如果是类似 `f2(x)` 这样在循环中的函数调用表达式也是同样的道理。
可能会选择先 inline 再优化提取循环不变代码之类的。
inline 优化就是如例子 `f1(x, 3)` 中的那样子。
但是，这里并不能对 f1 本身来做死代码消除。
因为 `f1` 的链接性是外部，在语义上是需要被保留，以供后面链接的时候被其他编译单元使用的。

在添加了 -O1 优化之后，`a.ll` 的 main 函数被优化成了一条 printf 调用，计算被完全常量展开了。
但是在加了 volatile 之后，对 `x` 和 `y` 的 load 和 store 都被加上了 volatile，并且还有 lifetime 标注。

具体的优化查询如下：

```bash
$ gcc -Q --help=optimizers -O1
The following options control optimizations:
  -O<number>                  
  -Ofast                      
  -Og                         
  -Os                         
  -Oz                         
  -faggressive-loop-optimizations       [enabled]
  -falign-functions                     [disabled]
  -falign-functions=          
  -falign-jumps                         [disabled]
  -falign-jumps=              
  -falign-labels                        [disabled]
  -falign-labels=             
  -falign-loops                         [disabled]
  -falign-loops=              
  -fallocation-dce                      [enabled]
  -fallow-store-data-races              [disabled]
  -fassociative-math                    [disabled]
  -fasynchronous-unwind-tables          [enabled]
  -fauto-inc-dec                        [enabled]
  -fbit-tests                           [enabled]
  -fbranch-count-reg                    [enabled]
  -fbranch-probabilities                [disabled]
  -fcaller-saves                        [disabled]
  -fcode-hoisting                       [disabled]
  -fcombine-stack-adjustments           [enabled]
  -fcompare-elim                        [enabled]
  -fconserve-stack                      [disabled]
  -fcprop-registers                     [enabled]
  -fcrossjumping                        [disabled]
  -fcse-follow-jumps                    [disabled]
  -fcx-fortran-rules                    [disabled]
  -fcx-limited-range                    [disabled]
  -fdce                                 [enabled]
  -fdefer-pop                           [enabled]
  -fdelayed-branch                      [disabled]
  -fdelete-dead-exceptions              [disabled]
  -fdelete-null-pointer-checks 
  -fdevirtualize                        [disabled]
  -fdevirtualize-speculatively          [disabled]
  -fdse                                 [enabled]
  -fearly-inlining                      [enabled]
  -fexceptions                          [available in D, Modula-2]
  -fexcess-precision=[fast|standard|16]         [default]
  -fexpensive-optimizations             [disabled]
  -ffast-math                 
  -ffinite-loops                        [disabled]
  -ffinite-math-only                    [disabled]
  -ffloat-store                         [disabled]
  -ffold-mem-offsets                    [enabled]
  -ffold-simple-inlines                 [available in C++, ObjC++]
  -fforward-propagate                   [enabled]
  -ffp-contract=[off|on|fast]           fast
  -ffp-int-builtin-inexact              [enabled]
  -ffunction-cse                        [enabled]
  -fgcse                                [disabled]
  -fgcse-after-reload                   [disabled]
  -fgcse-las                            [disabled]
  -fgcse-lm                             [enabled]
  -fgcse-sm                             [disabled]
  -fgraphite                            [disabled]
  -fgraphite-identity                   [disabled]
  -fguess-branch-probability            [enabled]
  -fhandle-exceptions                   -fexceptions
  -fhardcfr-check-exceptions  
  -fhardcfr-check-noreturn-calls=[always|no-xthrow|nothrow|never]       [default]
  -fhardcfr-check-returning-calls 
  -fhardcfr-skip-leaf                   [disabled]
  -fharden-compares                     [disabled]
  -fharden-conditional-branches         [disabled]
  -fharden-control-flow-redundancy      [disabled]
  -fhoist-adjacent-loads                [disabled]
  -fif-conversion                       [enabled]
  -fif-conversion2                      [enabled]
  -findirect-inlining                   [disabled]
  -finline                              [enabled]
  -finline-atomics                      [enabled]
  -finline-functions                    [disabled]
  -finline-functions-called-once        [enabled]
  -finline-small-functions              [disabled]
  -finline-stringops                    [disabled]
  -finline-stringops=                   [default]
  -fipa-bit-cp                          [disabled]
  -fipa-cp                              [disabled]
  -fipa-cp-clone                        [disabled]
  -fipa-icf                             [disabled]
  -fipa-icf-functions                   [disabled]
  -fipa-icf-variables                   [disabled]
  -fipa-modref                          [enabled]
  -fipa-profile                         [enabled]
  -fipa-pta                             [disabled]
  -fipa-pure-const                      [enabled]
  -fipa-ra                              [disabled]
  -fipa-reference                       [enabled]
  -fipa-reference-addressable           [enabled]
  -fipa-sra                             [disabled]
  -fipa-stack-alignment                 [enabled]
  -fipa-strict-aliasing                 [enabled]
  -fipa-vrp                             [disabled]
  -fira-algorithm=[CB|priority]         CB
  -fira-hoist-pressure                  [enabled]
  -fira-loop-pressure                   [disabled]
  -fira-region=[one|all|mixed]          one
  -fira-share-save-slots                [enabled]
  -fira-share-spill-slots               [enabled]
  -fisolate-erroneous-paths-attribute   [disabled]
  -fisolate-erroneous-paths-dereference         [disabled]
  -fivopts                              [enabled]
  -fjump-tables                         [enabled]
  -fkeep-gc-roots-live                  [disabled]
  -flifetime-dse                        [enabled]
  -flifetime-dse=<0,2>                  2
  -flimit-function-alignment            [disabled]
  -flive-patching                       -flive-patching=inline-clone
  -flive-patching=[inline-only-static|inline-clone]     [default]
  -flive-range-shrinkage                [disabled]
  -floop-interchange                    [disabled]
  -floop-nest-optimize                  [disabled]
  -floop-parallelize-all                [disabled]
  -floop-unroll-and-jam                 [disabled]
  -flra-remat                           [disabled]
  -fmath-errno                          [enabled]
  -fmin-function-alignment=             0
  -fmodulo-sched                        [disabled]
  -fmodulo-sched-allow-regmoves         [disabled]
  -fmove-loop-invariants                [enabled]
  -fmove-loop-stores                    [enabled]
  -fno-inline-stringops                 [enabled]
  -fnon-call-exceptions                 [disabled]
  -fnothrow-opt                         [available in C++, ObjC++]
  -fomit-frame-pointer                  [enabled]
  -fopenmp-target-simd-clone=           none
  -fopt-info                            [disabled]
  -foptimize-sibling-calls              [disabled]
  -foptimize-strlen                     [disabled]
  -fpack-struct                         [disabled]
  -fpack-struct=<number>      
  -fpartial-inlining                    [disabled]
  -fpatchable-function-entry= 
  -fpeel-loops                          [disabled]
  -fpeephole                            [enabled]
  -fpeephole2                           [disabled]
  -fplt                                 [enabled]
  -fpredictive-commoning                [disabled]
  -fprefetch-loop-arrays      
  -fprintf-return-value                 [enabled]
  -fprofile-partial-training            [disabled]
  -fprofile-reorder-functions           [disabled]
  -freciprocal-math                     [disabled]
  -free                                 [disabled]
  -freg-struct-return                   [enabled]
  -frename-registers                    [disabled]
  -freorder-blocks                      [enabled]
  -freorder-blocks-algorithm=[simple|stc]       simple
  -freorder-blocks-and-partition        [disabled]
  -freorder-functions                   [disabled]
  -frerun-cse-after-loop                [disabled]
  -freschedule-modulo-scheduled-loops   [disabled]
  -frounding-math                       [disabled]
  -frtti                                [available in C++, D, ObjC++]
  -fsave-optimization-record            [disabled]
  -fsched-critical-path-heuristic       [enabled]
  -fsched-dep-count-heuristic           [enabled]
  -fsched-group-heuristic               [enabled]
  -fsched-interblock                    [enabled]
  -fsched-last-insn-heuristic           [enabled]
  -fsched-pressure                      [disabled]
  -fsched-rank-heuristic                [enabled]
  -fsched-spec                          [enabled]
  -fsched-spec-insn-heuristic           [enabled]
  -fsched-spec-load                     [disabled]
  -fsched-spec-load-dangerous           [disabled]
  -fsched-stalled-insns                 [disabled]
  -fsched-stalled-insns-dep             [enabled]
  -fsched-stalled-insns-dep=<number> 
  -fsched-stalled-insns=<number> 
  -fsched2-use-superblocks              [disabled]
  -fschedule-fusion                     [enabled]
  -fschedule-insns                      [disabled]
  -fschedule-insns2                     [disabled]
  -fsection-anchors                     [disabled]
  -fsel-sched-pipelining                [disabled]
  -fsel-sched-pipelining-outer-loops    [disabled]
  -fsel-sched-reschedule-pipelined      [disabled]
  -fselective-scheduling                [disabled]
  -fselective-scheduling2               [disabled]
  -fsemantic-interposition              [enabled]
  -fshort-enums                         [enabled]
  -fshort-wchar                         [disabled]
  -fshrink-wrap                         [enabled]
  -fshrink-wrap-separate                [enabled]
  -fsignaling-nans                      [disabled]
  -fsigned-zeros                        [enabled]
  -fsimd-cost-model=[unlimited|dynamic|cheap|very-cheap]        unlimited
  -fsingle-precision-constant           [disabled]
  -fsplit-ivs-in-unroller               [enabled]
  -fsplit-loops                         [disabled]
  -fsplit-paths                         [disabled]
  -fsplit-wide-types                    [enabled]
  -fsplit-wide-types-early              [disabled]
  -fssa-backprop                        [enabled]
  -fssa-phiopt                          [enabled]
  -fstack-check=[no|generic|specific] 
  -fstack-clash-protection              [disabled]
  -fstack-protector                     [disabled]
  -fstack-protector-all                 [disabled]
  -fstack-protector-explicit            [disabled]
  -fstack-protector-strong              [disabled]
  -fstack-reuse=[all|named_vars|none]   all
  -fstdarg-opt                          [enabled]
  -fstore-merging                       [disabled]
  -fstrict-aliasing                     [disabled]
  -fstrict-enums                        [available in C++, ObjC++]
  -fstrict-volatile-bitfields 
  -fthread-jumps                        [enabled]
  -fno-threadsafe-statics               [available in C++, ObjC++]
  -ftoplevel-reorder                    [enabled]
  -ftracer                              [disabled]
  -ftrapping-math                       [enabled]
  -ftrapv                               [disabled]
  -ftree-bit-ccp                        [enabled]
  -ftree-builtin-call-dce               [enabled]
  -ftree-ccp                            [enabled]
  -ftree-ch                             [enabled]
  -ftree-coalesce-vars                  [enabled]
  -ftree-copy-prop                      [enabled]
  -ftree-cselim                         [disabled]
  -ftree-dce                            [enabled]
  -ftree-dominator-opts                 [enabled]
  -ftree-dse                            [enabled]
  -ftree-forwprop                       [enabled]
  -ftree-fre                            [enabled]
  -ftree-loop-distribute-patterns       [disabled]
  -ftree-loop-distribution              [disabled]
  -ftree-loop-if-convert      
  -ftree-loop-im                        [enabled]
  -ftree-loop-ivcanon                   [enabled]
  -ftree-loop-optimize                  [enabled]
  -ftree-loop-vectorize                 [disabled]
  -ftree-lrs                            [disabled]
  -ftree-parallelize-loops=<number>     1
  -ftree-partial-pre                    [disabled]
  -ftree-phiprop                        [enabled]
  -ftree-pre                            [disabled]
  -ftree-pta                            [enabled]
  -ftree-reassoc                        [enabled]
  -ftree-scev-cprop                     [enabled]
  -ftree-sink                           [enabled]
  -ftree-slp-vectorize                  [disabled]
  -ftree-slsr                           [enabled]
  -ftree-sra                            [enabled]
  -ftree-switch-conversion              [disabled]
  -ftree-tail-merge                     [disabled]
  -ftree-ter                            [enabled]
  -ftree-vectorize                      [disabled]
  -ftree-vrp                            [disabled]
  -ftrivial-auto-var-init=[uninitialized|pattern|zero]  uninitialized
  -funconstrained-commons               [disabled]
  -funreachable-traps                   [disabled]
  -funroll-all-loops                    [disabled]
  -funroll-completely-grow-size         [disabled]
  -funroll-loops                        [disabled]
  -funsafe-math-optimizations           [disabled]
  -funswitch-loops                      [disabled]
  -funwind-tables                       [disabled]
  -fvar-tracking                        [disabled]
  -fvar-tracking-assignments            [disabled]
  -fvar-tracking-assignments-toggle     [disabled]
  -fvar-tracking-uninit                 [disabled]
  -fvariable-expansion-in-unroller      [disabled]
  -fvect-cost-model=[unlimited|dynamic|cheap|very-cheap]        [default]
  -fversion-loops-for-strides           [disabled]
  -fvpt                                 [disabled]
  -fweb                                 [disabled]
  -fwrapv                               [disabled]
  -fwrapv-pointer                       [disabled]
  -gstatement-frontiers                 [disabled]
```

```bash
$ clang -S -emit-llvm -O1 a.c -ftime-report
===-------------------------------------------------------------------------===
                          Pass execution timing report
===-------------------------------------------------------------------------===
  Total Execution Time: 0.0009 seconds (0.0009 wall clock)

   ---User Time---   --User+System--   ---Wall Time---  --- Name ---
   0.0003 ( 38.0%)   0.0003 ( 38.0%)   0.0003 ( 38.1%)  SimplifyCFGPass
   0.0001 ( 11.4%)   0.0001 ( 11.4%)   0.0001 ( 11.3%)  PrintModulePass
   0.0001 (  7.3%)   0.0001 (  7.3%)   0.0001 (  7.5%)  SROAPass
   0.0001 (  6.8%)   0.0001 (  6.8%)   0.0001 (  6.7%)  InstCombinePass
   0.0001 (  6.6%)   0.0001 (  6.6%)   0.0001 (  6.5%)  InferFunctionAttrsPass
   0.0000 (  3.5%)   0.0000 (  3.5%)   0.0000 (  3.4%)  EarlyCSEPass
   0.0000 (  3.2%)   0.0000 (  3.2%)   0.0000 (  3.2%)  IPSCCPPass
   0.0000 (  2.7%)   0.0000 (  2.7%)   0.0000 (  2.7%)  PostOrderFunctionAttrsPass
   0.0000 (  1.8%)   0.0000 (  1.8%)   0.0000 (  1.9%)  LoopDistributePass
   0.0000 (  1.9%)   0.0000 (  1.9%)   0.0000 (  1.8%)  RequireAnalysisPass<llvm::GlobalsAA, llvm::Module, llvm::AnalysisManager<Module>>
   0.0000 (  1.7%)   0.0000 (  1.7%)   0.0000 (  1.6%)  LoopSimplifyPass
   0.0000 (  1.7%)   0.0000 (  1.7%)   0.0000 (  1.6%)  MemCpyOptPass
   0.0000 (  1.0%)   0.0000 (  1.0%)   0.0000 (  1.1%)  CGProfilePass
   0.0000 (  1.0%)   0.0000 (  1.0%)   0.0000 (  1.0%)  ReassociatePass
   0.0000 (  0.9%)   0.0000 (  0.9%)   0.0000 (  0.9%)  GlobalOptPass
   0.0000 (  0.8%)   0.0000 (  0.8%)   0.0000 (  0.7%)  BDCEPass
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.7%)  GlobalDCEPass
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.6%)  RecomputeGlobalsAAPass
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.6%)  CalledValuePropagationPass
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.6%)  TailCallElimPass
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.5%)  VectorCombinePass
   0.0000 (  0.4%)   0.0000 (  0.4%)   0.0000 (  0.5%)  Annotation2MetadataPass
   0.0000 (  0.4%)   0.0000 (  0.4%)   0.0000 (  0.4%)  ADCEPass
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.4%)  CoroEarlyPass
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.4%)  SCCPPass
   0.0000 (  0.4%)   0.0000 (  0.4%)   0.0000 (  0.3%)  DeadArgumentEliminationPass
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.3%)  AlwaysInlinerPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.3%)  CoroElidePass
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.3%)  Float2IntPass
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.3%)  AssignmentTrackingPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.3%)  InferAlignmentPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.2%)  LCSSAPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  InlinerPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  RequireAnalysisPass<llvm::ShouldNotRunFunctionPassesAnalysis, llvm::Function, llvm::AnalysisManager<Function>>
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  DivRemPairsPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  LowerExpectIntrinsicPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.2%)  LowerConstantIntrinsicsPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  RelLookupTableConverterPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.2%)  InvalidateAnalysisPass<llvm::ShouldNotRunFunctionPassesAnalysis>
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.2%)  LibCallsShrinkWrapPass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  ConstantMergePass
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.1%)  ForceFunctionAttrsPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  OpenMPOptPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  CoroCleanupPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  CoroSplitPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  InstSimplifyPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  EntryExitInstrumenterPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  LoopSinkPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  PromotePass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  InvalidateAnalysisPass<llvm::AAManager>
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  AnnotationRemarksPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  LoopUnrollPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  WarnMissedTransformationsPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  AlignmentFromAssumptionsPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  LoopLoadEliminationPass
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  LoopVectorizePass
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  InjectTLIMappings
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  ReversePostOrderFunctionAttrsPass
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  EliminateAvailableExternallyPass
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  RequireAnalysisPass<llvm::ProfileSummaryAnalysis, llvm::Module, llvm::AnalysisManager<Module>>
   0.0009 (100.0%)   0.0009 (100.0%)   0.0009 (100.0%)  Total

===-------------------------------------------------------------------------===
                        Analysis execution timing report
===-------------------------------------------------------------------------===
  Total Execution Time: 0.0004 seconds (0.0004 wall clock)

   ---User Time---   --User+System--   ---Wall Time---  --- Name ---
   0.0003 ( 75.8%)   0.0003 ( 75.8%)   0.0003 ( 76.3%)  TargetIRAnalysis
   0.0000 (  3.1%)   0.0000 (  3.1%)   0.0000 (  3.2%)  ScalarEvolutionAnalysis
   0.0000 (  2.9%)   0.0000 (  2.9%)   0.0000 (  2.7%)  LoopAnalysis
   0.0000 (  2.6%)   0.0000 (  2.6%)   0.0000 (  2.6%)  DominatorTreeAnalysis
   0.0000 (  2.4%)   0.0000 (  2.4%)   0.0000 (  2.4%)  LazyCallGraphAnalysis
   0.0000 (  2.1%)   0.0000 (  2.1%)   0.0000 (  2.2%)  MemorySSAAnalysis
   0.0000 (  1.9%)   0.0000 (  1.9%)   0.0000 (  2.0%)  GlobalsAA
   0.0000 (  1.9%)   0.0000 (  1.9%)   0.0000 (  1.6%)  AAManager
   0.0000 (  1.4%)   0.0000 (  1.4%)   0.0000 (  1.4%)  CallGraphAnalysis
   0.0000 (  1.0%)   0.0000 (  1.0%)   0.0000 (  1.1%)  PostDominatorTreeAnalysis
   0.0000 (  1.2%)   0.0000 (  1.2%)   0.0000 (  1.0%)  TargetLibraryAnalysis
   0.0000 (  0.5%)   0.0000 (  0.5%)   0.0000 (  0.6%)  DemandedBitsAnalysis
   0.0000 (  0.5%)   0.0000 (  0.5%)   0.0000 (  0.5%)  BasicAA
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.3%)  InnerAnalysisManagerProxy<CGSCCAnalysisManager, Module>
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  ProfileSummaryAnalysis
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  OptimizationRemarkEmitterAnalysis
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  LoopAccessAnalysis
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  InnerAnalysisManagerProxy<FunctionAnalysisManager, Module>
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.2%)  InlineAdvisorAnalysis
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  OuterAnalysisManagerProxy<ModuleAnalysisManager, Function>
   0.0000 (  0.5%)   0.0000 (  0.5%)   0.0000 (  0.2%)  FunctionAnalysisManagerCGSCCProxy
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  OuterAnalysisManagerProxy<ModuleAnalysisManager, LazyCallGraph::SCC, LazyCallGraph &>
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.2%)  AssumptionAnalysis
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.1%)  TypeBasedAA
   0.0000 (  0.2%)   0.0000 (  0.2%)   0.0000 (  0.1%)  ScopedNoAliasAA
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  ShouldNotRunFunctionPassesAnalysis
   0.0004 (100.0%)   0.0004 (100.0%)   0.0004 (100.0%)  Total

===-------------------------------------------------------------------------===
                         Miscellaneous Ungrouped Timers
===-------------------------------------------------------------------------===

   ---User Time---   --System Time--   --User+System--   ---Wall Time---  --- Name ---
   0.0018 ( 76.2%)   0.0000 (  0.0%)   0.0018 ( 74.9%)   0.0018 ( 72.5%)  Code Generation Time
   0.0006 ( 23.8%)   0.0000 (100.0%)   0.0006 ( 25.1%)   0.0007 ( 27.5%)  LLVM IR Generation Time
   0.0024 (100.0%)   0.0000 (100.0%)   0.0024 (100.0%)   0.0025 (100.0%)  Total

===-------------------------------------------------------------------------===
                          Clang front-end time report
===-------------------------------------------------------------------------===
  Total Execution Time: 0.0074 seconds (0.0075 wall clock)

   ---User Time---   --System Time--   --User+System--   ---Wall Time---  --- Name ---
   0.0066 (100.0%)   0.0008 (100.0%)   0.0074 (100.0%)   0.0075 (100.0%)  Clang front-end timer
   0.0066 (100.0%)   0.0008 (100.0%)   0.0074 (100.0%)   0.0075 (100.0%)  Total
```

## 目标代码生成

C 代码与 riscv 指令序列的关联在这个简单例子中还是挺直接的，可以看到构建调用栈、执行函数调用、返回等等过程。
