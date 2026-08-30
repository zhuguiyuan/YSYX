#include "dbg.h"
#include "lcthw/list.h"
#include "minunit.h"
#include <assert.h>
#include <bits/time.h>
#include <lcthw/list_algos.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define def_copy_fn(T)                                                         \
  void *T##_copy(void *src) {                                                  \
    if (src == NULL) {                                                         \
      return NULL;                                                             \
    }                                                                          \
    int *dst = malloc(sizeof(T));                                              \
    if (dst == NULL) {                                                         \
      return NULL;                                                             \
    }                                                                          \
    *dst = *(int *)src;                                                        \
    return dst;                                                                \
  }

def_copy_fn(int);

int int_cmp(const void *fst, const void *snd) {
  int x = *(int *)fst;
  int y = *(int *)snd;
  return x < y ? -1 : x == y ? 0 : 1;
}

int is_sorted(List *list) {
  LIST_FOREACH(list, first, next, cur) {
    if (cur->next && (*(int *)cur->value > *(int *)cur->next->value)) {
      debug("%d %d", *(int *)cur->value, *(int *)cur->next->value);
      return 0;
    }
  }
  return 1;
}

List *generate_random_int_list(int length) {
  List *list = NULL;
  int *item = NULL;
  int rc = 0;
  list = List_create(int_copy, free);
  check_mem(list);
  for (int i = 0; i < length; ++i) {
    item = malloc(sizeof(int));
    check_mem(item);
    *item = rand();
    rc = List_push(list, item);
    check(rc == 0, "generate_random_int_list: failed to create list");
    item = NULL;
  }
  return list;

error:
  if (list) {
    List_destroy(list);
  }
  if (item) {
    free(item);
  }
  return NULL;
}

typedef struct stat_t {
  const char *test_name;
  int input_scale;
  double time_cost;
} Stat;

Stat *Stat_create(const char *test_name, int scale, double cost) {
  Stat *result = calloc(1, sizeof(Stat));
  check_mem(result);
  result->test_name = test_name;
  result->input_scale = scale;
  result->time_cost = cost;
  return result;
error:
  return NULL;
}

def_copy_fn(Stat);

void print_stat(const Stat *s) {
  printf("%s %d %lf\n", s->test_name, s->input_scale, s->time_cost);
}

double elapsed(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

typedef char *(*bench_once_fn)(int scale);

static char *run_bench_isolated(bench_once_fn fn, int scale) {
  fflush(stdout);
  pid_t pid = fork();
  check(pid >= 0, "run_bench_isolated: fork failed");

  if (pid == 0) {
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

char *test_compare_sort() {
  struct timespec start, end;
  int scale = (int)5e4;

  List *backup_list = backup_list = generate_random_int_list(scale);
  if (backup_list == NULL) {
    return "Failed to create compare_list";
  }
  List *list_to_sort = NULL;
  Stat *result = NULL;

  // bubble_sort
  list_to_sort = List_copy(backup_list);
  if (list_to_sort == NULL) {
    return "test_compare_sort: failed to copy list";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List_bubble_sort(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!is_sorted(list_to_sort)) {
    List_destroy(list_to_sort);
    return "test_compare_sort: failed to buddle sort";
  }

  result = Stat_create("compare_bubble", scale, elapsed(start, end));
  if (result == NULL) {
    return "test_compare_sort: failed to record bubble result";
  }
  print_stat(result);
  List_destroy(list_to_sort);

  // merge_sort
  list_to_sort = List_copy(backup_list);
  if (list_to_sort == NULL) {
    return "test_compare_sort: failed to copy list";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List *new_list = List_merge_sort(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!is_sorted(new_list)) {
    List_destroy(new_list);
    List_destroy(list_to_sort);
    return "test_compare_sort: failed to merge sort";
  }

  result = Stat_create("compare_merge", scale, elapsed(start, end));
  if (result == NULL) {
    return "test_compare_sort: failed to record merge result";
  }
  print_stat(result);
  List_destroy(new_list);
  List_destroy(list_to_sort);

  // merge_sort_inplace
  list_to_sort = List_copy(backup_list);
  if (list_to_sort == NULL) {
    return "test_compare_sort: failed to copy list";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List_merge_sort_inplace(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!is_sorted(list_to_sort)) {
    List_destroy(list_to_sort);
    return "test_compare_sort: failed to merge inplace sort";
  }

  result = Stat_create("compare_merge_inplace", scale, elapsed(start, end));
  if (result == NULL) {
    return "test_compare_sort: failed to record merge inplace result";
  }
  print_stat(result);
  List_destroy(list_to_sort);

  List_destroy(backup_list);
  return NULL;
}

static char *bench_bubble_sort_once(int scale) {
  struct timespec start, end;

  List *list_to_sort = NULL;
  Stat *result = NULL;

  list_to_sort = generate_random_int_list(scale);
  if (list_to_sort == NULL) {
    return "bench_bubble_sort: failed to generate list_to_sort";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List_bubble_sort(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!is_sorted(list_to_sort)) {
    List_destroy(list_to_sort);
    return "bench_bubble_sort: failed to buddle sort";
  }

  result = Stat_create("bench_bubble", scale, elapsed(start, end));
  if (result == NULL) {
    return "bench_bubble_sort: failed to record bubble result";
  }
  print_stat(result);
  List_destroy(list_to_sort);

  return NULL;
}

char *bench_bubble_sort(void) {
  for (int i = 5000; i <= 50000; i += 5000) {
    char *err = run_bench_isolated(bench_bubble_sort_once, i);
    if (err != NULL) {
      return err;
    }
  }

  return NULL;
}

static char *bench_merge_sort_once(int scale) {
  struct timespec start, end;

  List *list_to_sort = NULL;
  Stat *result = NULL;
  list_to_sort = generate_random_int_list(scale);
  if (list_to_sort == NULL) {
    return "bench_merge_sort: failed to generate list_to_sort";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List *list_sorted = List_merge_sort(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!list_sorted || !is_sorted(list_sorted)) {
    List_destroy(list_sorted);
    List_destroy(list_to_sort);
    return "bench_merge_sort: failed to merge sort";
  }

  result = Stat_create("bench_merge", scale, elapsed(start, end));
  if (result == NULL) {
    return "bench_merge_sort: failed to record merge result";
  }
  print_stat(result);
  List_destroy(list_sorted);
  List_destroy(list_to_sort);

  return NULL;
}

char *bench_merge_sort(void) {
  int scale = (int)1e6;
  for (int i = 5000; i <= 16 * scale; i *= 2) {
    char *err = run_bench_isolated(bench_merge_sort_once, i);
    if (err != NULL) {
      return err;
    }
  }

  return NULL;
}

static char *bench_merge_sort_inplace_once(int scale) {
  struct timespec start, end;

  List *list_to_sort = NULL;
  Stat *result = NULL;
  list_to_sort = generate_random_int_list(scale);
  if (list_to_sort == NULL) {
    return "bench_merge_sort: failed to generate list_to_sort";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List_merge_sort_inplace(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!is_sorted(list_to_sort)) {
    List_destroy(list_to_sort);
    return "bench_merge_sort: failed to merge sort";
  }

  result = Stat_create("bench_merge_inplace", scale, elapsed(start, end));
  if (result == NULL) {
    return "bench_merge_sort: failed to record merge result";
  }
  print_stat(result);
  List_destroy(list_to_sort);

  return NULL;
}

char *bench_merge_sort_inplace(void) {
  int scale = (int)1e6;
  for (int i = 5000; i <= 32 * scale; i *= 2) {
    char *err = run_bench_isolated(bench_merge_sort_inplace_once, i);
    if (err != NULL) {
      return err;
    }
  }

  return NULL;
}

static char *bench_merge_sort_check_once(int scale) {
  struct timespec start, end;

  List *list_to_sort = NULL;
  Stat *result = NULL;

  list_to_sort = generate_random_int_list(scale);
  if (list_to_sort == NULL) {
    return "bench_merge_sort: failed to generate list_to_sort";
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  List *list_sorted = List_merge_sort(list_to_sort, int_cmp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!list_sorted || !is_sorted(list_sorted)) {
    List_destroy(list_sorted);
    List_destroy(list_to_sort);
    return "bench_merge_sort: failed to merge sort";
  }

  result = Stat_create("bench_merge", scale, elapsed(start, end));
  if (result == NULL) {
    return "bench_merge_sort: failed to record merge result";
  }
  print_stat(result);
  List_destroy(list_sorted);
  List_destroy(list_to_sort);

  return NULL;
}

char *bench_merge_sort_check(void) {
  for (int i = 5120000; i <= 10240000; i *= 2) {
    char *err = run_bench_isolated(bench_merge_sort_check_once, i);
    if (err != NULL) {
      return err;
    }
  }

  return NULL;
}

char *all_tests(void) {
  mu_suite_start();
  srand(time(NULL));
  mu_run_test(test_compare_sort);
  mu_run_test(bench_merge_sort_inplace);
  mu_run_test(bench_merge_sort);
  mu_run_test(bench_bubble_sort);
  // mu_run_test(bench_merge_sort_check);
  return NULL;
}

RUN_TESTS(all_tests);
