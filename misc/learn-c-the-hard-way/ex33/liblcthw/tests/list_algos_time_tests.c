#include "dbg.h"
#include "minunit.h"
#include <lcthw/list_algos.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void *int_copy(void *src) {
  if (src == NULL) {
    return NULL;
  }

  int *dst = malloc(sizeof(int));
  if (dst != NULL) {
    *dst = *(int *)src;
  }
  return dst;
}

static int int_cmp(const void *fst, const void *snd) {
  int x = *(int *)fst;
  int y = *(int *)snd;
  return x < y ? -1 : x == y ? 0 : 1;
}

static int is_sorted(List *list) {
  LIST_FOREACH(list, first, next, cur) {
    if (cur->next && (*(int *)cur->value > *(int *)cur->next->value)) {
      return 0;
    }
  }
  return 1;
}

static List *generate_random_int_list(int length) {
  List *list = NULL;
  int *item = NULL;

  list = List_create(int_copy, free);
  check_mem(list);
  for (int i = 0; i < length; ++i) {
    item = malloc(sizeof(int));
    check_mem(item);
    *item = rand();
    check(List_push(list, item) == 0,
          "generate_random_int_list: failed to create list");
    item = NULL;
  }
  return list;

error:
  List_destroy(list);
  free(item);
  return NULL;
}

static double elapsed(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

static char *run_sort_once(List_sort sort, const char *name, int scale) {
  List *list = generate_random_int_list(scale);
  if (list == NULL) {
    return "Failed to generate list to sort";
  }

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  int rc = sort(list, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);

  if (rc != 0 || !is_sorted(list)) {
    List_destroy(list);
    return "Failed to sort list";
  }

  printf("%s %d %lf\n", name, scale, elapsed(start, end));
  List_destroy(list);
  return NULL;
}

typedef char *(*bench_once_fn)(int scale);

static char *run_bench_isolated(bench_once_fn fn, int scale) {
  fflush(stdout);
  pid_t pid = fork();
  check(pid >= 0, "run_bench_isolated: fork failed");

  if (pid == 0) {
    srand(0x9e3779b9u ^ (unsigned int)scale);
    char *err = fn(scale);
    if (err != NULL) {
      fprintf(stderr, "%s\n", err);
      _exit(1);
    }
    fflush(stdout);
    _exit(0);
  }

  int status = 0;
  check(waitpid(pid, &status, 0) >= 0, "run_bench_isolated: waitpid failed");
  check(WIFEXITED(status) && WEXITSTATUS(status) == 0,
        "run_bench_isolated: child benchmark failed");
  return NULL;

error:
  return "run_bench_isolated: failed";
}

static char *run_sort_range(bench_once_fn fn, int start, int limit,
                            int multiplier, int increment) {
  for (int scale = start; scale <= limit;
       scale = multiplier > 1 ? scale * multiplier : scale + increment) {
    char *err = run_bench_isolated(fn, scale);
    if (err != NULL) {
      return err;
    }
  }
  return NULL;
}

static char *test_compare_sort(void) {
  struct {
    List_sort sort;
    const char *name;
  } sorts[] = {{List_bubble_sort, "compare_bubble"},
               {List_merge_sort, "compare_merge"},
               {List_merge_sort_inplace, "compare_merge_inplace"},
               {List_merge_sort_bottom_up, "compare_merge_bottom_up"}};

  srand(0x9e3779b9u ^ 50000u);
  List *backup = generate_random_int_list(50000);
  if (backup == NULL) {
    return "Failed to create comparison input";
  }

  for (size_t i = 0; i < sizeof(sorts) / sizeof(sorts[0]); ++i) {
    List *list = List_copy(backup);
    if (list == NULL) {
      List_destroy(backup);
      return "Failed to copy comparison input";
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int rc = sorts[i].sort(list, int_cmp);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (rc != 0 || !is_sorted(list)) {
      List_destroy(list);
      List_destroy(backup);
      return "Comparison sort failed";
    }

    printf("%s %d %lf\n", sorts[i].name, 50000, elapsed(start, end));
    List_destroy(list);
  }

  List_destroy(backup);
  return NULL;
}

static char *bench_bubble_sort_once(int scale) {
  return run_sort_once(List_bubble_sort, "bench_bubble", scale);
}

static char *bench_merge_sort_once(int scale) {
  return run_sort_once(List_merge_sort, "bench_merge", scale);
}

static char *bench_merge_sort_inplace_once(int scale) {
  return run_sort_once(List_merge_sort_inplace, "bench_merge_inplace", scale);
}

static char *bench_merge_sort_bottom_up_once(int scale) {
  return run_sort_once(List_merge_sort_bottom_up, "bench_merge_bottom_up",
                       scale);
}

static char *bench_bubble_sort(void) {
  return run_sort_range(bench_bubble_sort_once, 5000, 50000, 1, 5000);
}

static char *bench_merge_sort(void) {
  return run_sort_range(bench_merge_sort_once, 5000, 16000000, 2, 0);
}

static char *bench_merge_sort_inplace(void) {
  return run_sort_range(bench_merge_sort_inplace_once, 5000, 32000000, 2, 0);
}

static char *bench_merge_sort_bottom_up(void) {
  return run_sort_range(bench_merge_sort_bottom_up_once, 5000, 32000000, 2, 0);
}

static char *all_tests(void) {
  mu_suite_start();

  mu_run_test(test_compare_sort);
  mu_run_test(bench_merge_sort_bottom_up);
  mu_run_test(bench_merge_sort_inplace);
  mu_run_test(bench_merge_sort);
  mu_run_test(bench_bubble_sort);

  return NULL;
}

RUN_TESTS(all_tests);
