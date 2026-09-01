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
  int rc = 0;

  result = List_create(left->copy_fn, left->free_fn);
  check_mem(result);

  while (List_count(left) > 0 || List_count(right) > 0) {
    if (List_count(left) > 0 && List_count(right) > 0) {
      if (cmp(List_first(left), List_first(right)) <= 0) {
        rc = List_push(result, List_first(left));
        check(rc == 0, "List_merge: failed to List_push");
        List_shift(left);
      } else {
        rc = List_push(result, List_first(right));
        check(rc == 0, "List_merge: failed to List_push");
        List_shift(right);
      }
    } else if (List_count(left) > 0) {
      rc = List_push(result, List_first(left));
      check(rc == 0, "List_merge: failed to List_push");
      List_shift(left);
    } else if (List_count(right) > 0) {
      rc = List_push(result, List_first(right));
      check(rc == 0, "List_merge: failed to List_push");
      List_shift(right);
    }
  }

  return result;

error:
  if (result) {
    List_destroy(result);
  }
  return NULL;
}

int List_merge_sort(List *list, List_compare cmp) {
  if (cmp == NULL || list == NULL || List_count(list) <= 1) {
    return 0;
  }

  List *left = NULL;
  List *right = NULL;
  List *merge = NULL;
  int rc = 0;

  left = List_create(list->copy_fn, list->free_fn);
  check_mem(left);
  right = List_create(list->copy_fn, list->free_fn);
  check_mem(right);

  int middle = List_count(list) / 2;

  while (List_count(list) != 0) {
    void *valuep = List_first(list);
    if (middle > 0) {
      rc = List_push(left, valuep);
      check(rc == 0, "List_merge_sort: Failed to List_push");
    } else {
      rc = List_push(right, valuep);
      check(rc == 0, "List_merge_sort: Failed to List_push");
    }
    List_shift(list);
    middle--;
  }

  rc = List_merge_sort(left, cmp);
  check(rc == 0, "List_merge_sort: Failed to sort left");

  rc = List_merge_sort(right, cmp);
  check(rc == 0, "List_merge_sort: Failed to sort right");

  merge = List_merge(left, right, cmp);
  check_mem(merge);

  *list = *merge;
  free(merge);
  List_destroy(left);
  List_destroy(right);

  return 0;

error:
  List_destroy(merge);
  List_destroy(left);
  List_destroy(right);
  return -1;
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
    *right = (List){0};
    return left;
  }
  if (right->count == 0) {
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
  *right = (List){0};

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
  invariant_debug(List_count(right) == 0,
                  "after List_merge_inplace, right must be empty.");
  free(right);
  invariant_debug(is_sorted(list, cmp), "invariant check result sorted");

  free(split);
  return 0;

error:
  free(split);
  return -1;
}

int List_insert_sorted(List *list, void *value, List_compare cmp) {
  ListNode *new_node = NULL;
  check(list != NULL && value != NULL && cmp != NULL,
        "List_insert_sorted: invalid arguments");

  ListNode *after = NULL;
  LIST_FOREACH(list, first, next, curr) {
    if (cmp(value, curr->value) >= 0) {
      after = curr;
    } else {
      break;
    }
  }
  if (after == NULL) {
    int rc = List_unshift(list, value);
    check(rc == 0, "List_insert_sorted: failed to unshift value");
  } else if (after->next == NULL) {
    int rc = List_push(list, value);
    check(rc == 0, "List_insert_sorted: failed to push value");
  } else {
    new_node = malloc(sizeof(ListNode));
    check_mem(new_node);
    new_node->value = value;
    new_node->prev = after;
    new_node->next = after->next;
    after->next->prev = new_node;
    after->next = new_node;
    list->count++;
  }
  return 0;

error:
  if (new_node)
    free(new_node);
  return -1;
}

int List_merge_sort_bottom_up(List *list, List_compare cmp) {
  List *merge_queue[32] = {NULL};
  List *carry = NULL;
  int i = 0;
  int rc = 0;

  if (cmp == NULL || list == NULL || List_count(list) <= 1) {
    return 0;
  }

  carry = List_create(list->copy_fn, list->free_fn);
  check_mem(carry);
  for (i = 0; i < 32; ++i) {
    merge_queue[i] = List_create(list->copy_fn, list->free_fn);
    check_mem(merge_queue[i]);
  }

  while (List_count(list) != 0) {
    rc = List_unshift(carry, List_first(list));
    check(rc == 0, "List_merge_sort_bottom_up: failed to unshift valuep");
    List_shift(list);

    for (i = 0; i < 32 && List_count(merge_queue[i]) != 0; ++i) {
      List_merge_inplace(merge_queue[i], carry, cmp);
      invariant_debug(List_count(carry) == 0,
                      "after List_merge_inplace, right list must be empty");

      List *tmp = carry;
      carry = merge_queue[i];
      merge_queue[i] = tmp;
    }

    if (i == 32) {
      i -= 1;
    }
    List_merge_inplace(merge_queue[i], carry, cmp);
  }

  for (i = 0; i < 32; ++i) {
    List_merge_inplace(list, merge_queue[i], cmp);
    free(merge_queue[i]);
  }
  free(carry);
  return 0;

error:
  for (int i = 0; i < 32; ++i) {
    List_destroy(merge_queue[i]);
  }
  List_destroy(carry);
  return -1;
}
