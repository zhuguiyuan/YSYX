#include "lcthw/list.h"
#include <dbg.h>
#include <lcthw/list_algos.h>
#include <stdlib.h>
#include <string.h>

static inline void ListNode_swap(ListNode *a, ListNode *b) {
  void *t = a->value;
  a->value = b->value;
  b->value = t;
}

int List_bubble_sort(List *list, List_compare cmp) {
  int swapped;
  do {
    swapped = 1;
    LIST_FOREACH(list, first, next, curr) {
      if (curr->next && cmp(curr->value, curr->next->value) > 0) {
        ListNode_swap(curr, curr->next);
        swapped = 0;
      }
    }
  } while (!swapped);

  return 0;
}

static inline List *List_merge(List *left, List *right, List_compare cmp) {
  List *result = NULL;
  void *val = NULL;
  int rc = 0;

  result = List_create(left->copy_fn, left->free_fn);
  check_mem(result);

  while (List_count(left) > 0 || List_count(right) > 0) {
    if (List_count(left) > 0 && List_count(right) > 0) {
      if (cmp(List_first(left), List_first(right)) <= 0) {
        val = List_shift(left);
      } else {
        val = List_shift(right);
      }
      rc = List_push(result, val);
      check(rc == 0, "List_merge: failed to List_push");
      val = NULL;
    } else if (List_count(left) > 0) {
      val = List_shift(left);
      rc = List_push(result, val);
      check(rc == 0, "List_merge: failed to List_push");
      val = NULL;
    } else if (List_count(right) > 0) {
      val = List_shift(right);
      rc = List_push(result, val);
      check(rc == 0, "List_merge: failed to List_push");
      val = NULL;
    }
  }

  return result;

error:
  if (val && left->free_fn) {
    left->free_fn(val);
  }
  if (result) {
    List_destroy(result);
  }
  return NULL;
}

List *List_merge_sort(List *list, List_compare cmp) {
  if (cmp == NULL || list == NULL || List_count(list) <= 1) {
    return list;
  }

  List *left = NULL;
  List *right = NULL;
  void *val = NULL;
  List *sort_left = NULL;
  List *sort_right = NULL;
  int rc = 0;

  left = List_create(list->copy_fn, list->free_fn);
  check_mem(left);
  right = List_create(list->copy_fn, list->free_fn);
  check_mem(right);

  int middle = List_count(list) / 2;

  LIST_FOREACH(list, first, next, cur) {
    if (middle > 0) {
      val = ListNode_value_copy(list->copy_fn, cur);
      check_mem(val);
      rc = List_push(left, val);
      check(rc == 0, "List_merge_sort: Failed to List_push");
      val = NULL;
    } else {
      val = ListNode_value_copy(list->copy_fn, cur);
      check_mem(val);
      rc = List_push(right, val);
      check(rc == 0, "List_merge_sort: Failed to List_push");
      val = NULL;
    }

    middle--;
  }

  sort_left = List_merge_sort(left, cmp);
  check_mem(sort_left);
  if (sort_left != left) {
    List_destroy(left);
  }
  left = NULL;

  sort_right = List_merge_sort(right, cmp);
  check_mem(sort_right);
  if (sort_right != right) {
    List_destroy(right);
  }
  right = NULL;

  List *result = List_merge(sort_left, sort_right, cmp);
  check_mem(result);
  List_destroy(sort_left);
  List_destroy(sort_right);
  return result;

error:
  if (val && list->free_fn) {
    list->free_fn(val);
  }
  List_destroy(left);
  List_destroy(right);
  List_destroy(sort_left);
  List_destroy(sort_right);
  return NULL;
}

static inline int is_sorted(List *list, List_compare cmp) {
  LIST_FOREACH(list, first, next, curr) {
    if (curr->prev != NULL && cmp(curr->prev->value, curr->value) > 0) {
      return 0;
    }
  }
  return 1;
}

static inline List **List_split_inplace(List *list, ListNode *split_point) {
  List **result = calloc(2, sizeof(List *));
  check_mem(result);

  result[0] = list;
  result[1] = List_create(list->copy_fn, list->free_fn);
  check_mem(result[1]);

  ListNode *orig_last = list->last;
  int orig_count = list->count;

  int left_count = 0;
  ListNode *left_last = NULL;
  LIST_FOREACH(list, first, next, curr) {
    if (curr == split_point)
      break;
    ++left_count;
    left_last = curr;
  }
  // 第一个点就是 split，左侧为空
  if (left_last == NULL) {
    list->first = NULL;
    list->last = NULL;
    list->count = 0;
  } else {
    left_last->next = NULL;
    split_point->prev = NULL;
    list->last = left_last;
    list->count = left_count;
  }

  result[1]->first = split_point;
  result[1]->last = orig_last;
  result[1]->count = orig_count - left_count;
  return result;

error:
  if (result) {
    List_destroy(result[1]);
    free(result);
  }
  return NULL;
}

static inline List *List_merge_inplace(List *left, List *right,
                                       List_compare cmp) {
  if (List_count(left) == 0) {
    *left = *right;
    free(right);
    return left;
  }
  if (right->count == 0) {
    free(right);
    return left;
  }

  ListNode dummy;
  ListNode *tail = &dummy;
  dummy.next = NULL;

  ListNode *a = left->first;
  ListNode *b = right->first;
  while (a && b) {
    if (cmp(a->value, b->value) <= 0) {
      tail->next = a;
      a->prev = tail;
      tail = a;
      a = a->next;
    } else {
      tail->next = b;
      b->prev = tail;
      tail = b;
      b = b->next;
    }
  }
  if (a) {
    tail->next = a;
    a->prev = tail;
    tail = left->last;
  } else if (b) {
    tail->next = b;
    b->prev = tail;
    tail = right->last;
  }
  left->first = dummy.next;
  left->first->prev = NULL;
  left->last = tail;
  left->count = left->count + right->count;

  free(right);
  return left;
}

int List_merge_sort_inplace(List *list, List_compare cmp) {
  if (cmp == NULL || list == NULL || List_count(list) <= 1) {
    return 0;
  }

  ListNode *mid = list->first;
  int middle = List_count(list) / 2;
  while (middle-- > 0) {
    mid = mid->next;
  }

  List **split = NULL;
  List *left = NULL;
  List *right = NULL;
  int rc = 0;

  split = List_split_inplace(list, mid);
  check_mem(split);
  left = split[0];
  right = split[1];

  rc = List_merge_sort_inplace(left, cmp);
  check(rc == 0, "List_merge_sort_inplace: failed to sort left");
  invariant_debug(is_sorted(left, cmp), "invariant check left sorted");

  rc = List_merge_sort_inplace(right, cmp);
  check(rc == 0, "List_merge_sort_inplace: failed to sort right");
  invariant_debug(is_sorted(right, cmp), "invariant check right sorted");

  List_merge_inplace(left, right, cmp);
  invariant_debug(is_sorted(list, cmp), "invariant check result sorted");

  free(split);
  return 0;

error:
  free(split);
  return -1;
}
