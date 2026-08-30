#include "minunit.h"
#include <assert.h>
#include <lcthw/list.h>
#include <stdlib.h>
#include <string.h>

static char *test1 = "test1 data";
static char *test2 = "test2 data";
static char *test3 = "test3 data";

static char *heap_str(const char *str) {
  char *copy = malloc(strlen(str) + 1);
  mu_assert(copy != NULL, "malloc failed in heap_str");
  strcpy(copy, str);
  return copy;
}

static void *copy_string(void *value) {
  if (value == NULL)
    return NULL;
  char *str = (char *)value;
  char *new_str = malloc(strlen(str) + 1);
  mu_assert(new_str != NULL, "malloc failed in copy_string");
  strcpy(new_str, str);
  return new_str;
}

static void free_string(void *value) { free(value); }

char *test_create_destroy() {
  List *list = List_create(NULL, NULL);
  mu_assert(list != NULL, "Failed to create list.");
  List_destroy(list);
  return NULL;
}

// 测试基本操作（push/pop/unshift/shift/remove）
char *test_basic_operations() {
  List *list = List_create(NULL, NULL);
  mu_assert(list != NULL, "Failed to create list.");

  // push / pop
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);
  mu_assert(List_count(list) == 3, "Wrong count after push.");
  mu_assert(List_last(list) == test3, "Wrong last after push.");

  char *val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");
  mu_assert(List_count(list) == 0, "Wrong count after pop.");

  // unshift / shift
  List_unshift(list, test1);
  List_unshift(list, test2);
  List_unshift(list, test3);
  mu_assert(List_count(list) == 3, "Wrong count after unshift.");
  mu_assert(List_first(list) == test3, "Wrong first after unshift.");

  val = List_shift(list);
  mu_assert(val == test3, "Wrong value on shift.");
  val = List_shift(list);
  mu_assert(val == test2, "Wrong value on shift.");
  val = List_shift(list);
  mu_assert(val == test1, "Wrong value on shift.");
  mu_assert(List_count(list) == 0, "Wrong count after shift.");

  // remove middle
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);
  val = List_remove(list, list->first->next);
  mu_assert(val == test2, "Wrong removed element.");
  mu_assert(List_count(list) == 2, "Wrong count after remove.");
  mu_assert(List_first(list) == test1, "Wrong first after remove.");
  mu_assert(List_last(list) == test3, "Wrong last after remove.");

  List_destroy(list);
  return NULL;
}

// 测试浅拷贝
char *test_copy_shallow() {
  List *src = List_create(NULL, NULL);
  List_push(src, test1);
  List_push(src, test2);
  List_push(src, test3);

  List *copy = List_copy(src);
  mu_assert(copy != NULL, "List_copy returned NULL");
  mu_assert(List_count(copy) == 3, "Wrong count in copied list");

  // 浅拷贝应共享相同的指针
  mu_assert(List_first(copy) == test1, "Wrong first value in shallow copy");
  mu_assert(List_last(copy) == test3, "Wrong last value in shallow copy");

  // 修改原链表不应影响副本（节点结构独立，但数据指针相同）
  List_pop(src);
  mu_assert(List_count(src) == 2, "Wrong count in source after pop");
  mu_assert(List_count(copy) == 3, "Copy count changed unexpectedly");

  List_destroy(copy);
  List_destroy(src);
  return NULL;
}

// 测试深拷贝
char *test_copy_deep() {
  List *src = List_create(copy_string, free_string);
  const char *heap_test1 = heap_str(test1);
  const char *heap_test2 = heap_str(test2);
  const char *heap_test3 = heap_str(test3);
  List_push(src, (void *)heap_test1);
  List_push(src, (void *)heap_test2);
  List_push(src, (void *)heap_test3);

  List *copy = List_copy(src);
  mu_assert(copy != NULL, "List_copy returned NULL");
  mu_assert(List_count(copy) == 3, "Wrong count in copied list");

  // 深拷贝后数据指针应不同，但内容相同
  char *val1 = List_first(copy);
  char *val3 = List_last(copy);
  mu_assert(val1 != heap_test1, "Deep copy should have different pointer");
  mu_assert(strcmp(val1, heap_test1) == 0, "Deep copy content mismatch");
  mu_assert(val3 != heap_test3, "Deep copy should have different pointer");
  mu_assert(strcmp(val3, heap_test3) == 0, "Deep copy content mismatch");

  // 释放原链表不应影响副本
  List_destroy(src);
  mu_assert(strcmp(val1, test1) == 0,
            "Copy data corrupted after source destroy");
  mu_assert(strcmp(val3, test3) == 0,
            "Copy data corrupted after source destroy");

  List_destroy(copy);
  return NULL;
}

// 测试拼接
char *test_concat() {
  List *list1 = List_create(NULL, NULL);
  List_push(list1, test1);
  List_push(list1, test2);

  List *list2 = List_create(NULL, NULL);
  List_push(list2, test3);

  List *concat = List_concat(list1, list2);
  mu_assert(concat != NULL, "List_concat returned NULL");
  mu_assert(List_count(concat) == 3, "Wrong count after concat");
  mu_assert(List_first(concat) == test1, "Wrong first element");
  mu_assert(List_last(concat) == test3, "Wrong last element");

  // 验证顺序
  ListNode *node = concat->first;
  mu_assert(node->value == test1, "First node wrong");
  node = node->next;
  mu_assert(node->value == test2, "Second node wrong");
  node = node->next;
  mu_assert(node->value == test3, "Third node wrong");

  List_destroy(concat);
  List_destroy(list1);
  List_destroy(list2);
  return NULL;
}

// 测试深拷贝拼接
char *test_concat_deep() {
  List *list1 = List_create(copy_string, free_string);
  List_push(list1, heap_str(test1));
  List_push(list1, heap_str(test2));

  List *list2 = List_create(copy_string, free_string);
  List_push(list2, heap_str(test3));

  List *concat = List_concat(list1, list2);
  mu_assert(concat != NULL, "List_concat returned NULL");
  mu_assert(List_count(concat) == 3, "Wrong count after deep concat");

  ListNode *node = concat->first;
  mu_assert(strcmp((char *)node->value, test1) == 0, "First content wrong");
  mu_assert(node->value != test1, "First pointer should be different");
  node = node->next;
  mu_assert(strcmp((char *)node->value, test2) == 0, "Second content wrong");
  mu_assert(node->value != test2, "Second pointer should be different");
  node = node->next;
  mu_assert(strcmp((char *)node->value, test3) == 0, "Third content wrong");
  mu_assert(node->value != test3, "Third pointer should be different");

  List_destroy(concat);
  List_destroy(list1);
  List_destroy(list2);
  return NULL;
}

// 测试分割
char *test_split() {
  List *src = List_create(NULL, NULL);
  List_push(src, test1);
  List_push(src, test2);
  List_push(src, test3);

  // 在第二个节点处分割（即 test2）
  ListNode *split_node = src->first->next;
  List **parts = List_split(src, split_node);
  mu_assert(parts != NULL, "List_split returned NULL");
  mu_assert(parts[0] != NULL && parts[1] != NULL, "Split lists are NULL");

  // 第一部分应包含 test1
  mu_assert(List_count(parts[0]) == 1, "First part count wrong");
  mu_assert(List_first(parts[0]) == test1, "First part content wrong");

  // 第二部分应包含 test2 和 test3（取决于 split 语义：split_point
  // 属于第二部分）
  mu_assert(List_count(parts[1]) == 2, "Second part count wrong");
  mu_assert(List_first(parts[1]) == test2, "Second part first element wrong");
  mu_assert(List_last(parts[1]) == test3, "Second part last element wrong");

  // 原链表保持不变
  mu_assert(List_count(src) == 3, "Source list modified during split");

  // 清理
  List_destroy(parts[0]);
  List_destroy(parts[1]);
  free(parts);
  List_destroy(src);
  return NULL;
}

// 测试深拷贝分割
char *test_split_deep() {
  List *src = List_create(copy_string, free_string);
  List_push(src, heap_str(test1));
  List_push(src, heap_str(test2));
  List_push(src, heap_str(test3));

  ListNode *split_node = src->first->next;
  List **parts = List_split(src, split_node);
  mu_assert(parts != NULL, "List_split returned NULL");

  char *val = List_first(parts[0]);
  mu_assert(strcmp(val, test1) == 0, "First part content wrong");
  mu_assert(val != test1, "First part pointer should be different");

  val = List_first(parts[1]);
  mu_assert(strcmp(val, test2) == 0, "Second part first content wrong");
  mu_assert(val != test2, "Second part first pointer should be different");
  val = List_last(parts[1]);
  mu_assert(strcmp(val, test3) == 0, "Second part last content wrong");
  mu_assert(val != test3, "Second part last pointer should be different");

  List_destroy(parts[0]);
  List_destroy(parts[1]);
  free(parts);
  List_destroy(src);
  return NULL;
}

// 测试空链表和边界情况
char *test_edge_cases() {
  // 空链表拷贝
  List *empty = List_create(NULL, NULL);
  List *copy = List_copy(empty);
  mu_assert(copy != NULL, "Copy of empty list should not be NULL");
  mu_assert(List_count(copy) == 0, "Copy of empty list should be empty");
  List_destroy(copy);
  List_destroy(empty);

  // 拼接空链表
  List *list1 = List_create(NULL, NULL);
  List_push(list1, test1);
  List *empty2 = List_create(NULL, NULL);
  List *concat = List_concat(list1, empty2);
  mu_assert(concat != NULL, "Concat with empty failed");
  mu_assert(List_count(concat) == 1,
            "Concat with empty should have one element");
  mu_assert(List_first(concat) == test1, "Concat with empty wrong element");
  List_destroy(concat);
  List_destroy(list1);
  List_destroy(empty2);

  // 分割点不在链表中（可选：根据当前实现，允许这种情况，则第二部分为空）
  List *list2 = List_create(NULL, NULL);
  List_push(list2, test1);
  List_push(list2, test2);
  // 创建一个不属于 list2 的节点（仅用于分割测试）
  ListNode *fake_node = calloc(1, sizeof(ListNode));
  List **parts = List_split(list2, fake_node);
  mu_assert(parts != NULL, "Split with invalid point failed");
  mu_assert(List_count(parts[0]) == 2,
            "First part should contain all elements");
  mu_assert(List_count(parts[1]) == 0, "Second part should be empty");
  List_destroy(parts[0]);
  List_destroy(parts[1]);
  free(parts);
  free(fake_node);
  List_destroy(list2);

  return NULL;
}

// 所有测试
char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create_destroy);
  mu_run_test(test_basic_operations);
  mu_run_test(test_copy_shallow);
  mu_run_test(test_copy_deep);
  mu_run_test(test_concat);
  mu_run_test(test_concat_deep);
  mu_run_test(test_split);
  mu_run_test(test_split_deep);
  mu_run_test(test_edge_cases);

  return NULL;
}

RUN_TESTS(all_tests);