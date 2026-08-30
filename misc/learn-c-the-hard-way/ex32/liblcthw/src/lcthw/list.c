#include <assert.h>
#include <dbg.h>
#include <lcthw/list.h>
#include <stdlib.h>
#include <string.h>

List *List_create(List_copy_fn copy_fn, List_free_fn free_fn) {
  check((copy_fn == NULL && free_fn == NULL) ||
            (copy_fn != NULL && free_fn != NULL),
        "List_create: copy_fn and free_fn must be both NULL or both not NULL");
  List *list = calloc(1, sizeof(List));
  check_mem(list);
  list->copy_fn = copy_fn;
  list->free_fn = free_fn;
  return list;

error:
  return NULL;
}

void List_clear(List *list) {
  if (list == NULL) {
    return;
  }
  LIST_FOREACH(list, first, next, curr) {
    if (list->free_fn) {
      list->free_fn(curr->value);
    }
    if (curr->prev) {
      free(curr->prev);
    }
  }
  free(list->last);
  list->first = NULL;
  list->last = NULL;
  list->count = 0;
}

void List_destroy(List *list) {
  if (list == NULL) {
    return;
  }
  List_clear(list);
  free(list);
}

int List_push(List *list, void *value) {
  check(list, "list can't be NULL");
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;
  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }
  list->count++;
  return 0;

error:
  return -1;
}

void *List_pop(List *list) {
  check(list, "list can't be NULL");
  ListNode *node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;

error:
  return NULL;
}

int List_unshift(List *list, void *value) {
  check(list, "list can't be NULL");
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;
  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }
  list->count++;
  return 0;

error:
  return -1;
}

void *List_shift(List *list) {
  if (list == NULL) {
    return NULL;
  }
  ListNode *node = list->first;
  return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node) {
  void *result = NULL;
  check(list, "list can't be NULL");
  check(list->first && list->last, "List is empty");
  check(node, "node can't be NULL");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL,
          "Invalid list, somehow got a first that is NULL.");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
    list->last->next = NULL;
  } else {
    ListNode *after = node->next;
    ListNode *before = node->prev;
    after->prev = before;
    before->next = after;
  }
  list->count--;
  check(list->count >= 0, "List_remove: invariant count >= 0 violated");
  if (list->count > 0) {
    check(list->first != NULL,
          "List_remove: invariant if count > 0 then first != NULL violated.");
  }
  result = node->value;
  free(node);

error:
  return result;
}

static void *ListNode_value_copy(List_copy_fn copy, ListNode *node) {
  if (copy == NULL) {
    return node->value;
  }
  return copy(node->value);
}

List *List_copy(List *src) {
  List *new_list = NULL;
  void *new_value = NULL;
  check(src != NULL, "List_copy: src cannot be NULL");

  new_list = List_create(src->copy_fn, src->free_fn);
  check_mem(new_list);
  LIST_FOREACH(src, first, next, curr) {
    new_value = ListNode_value_copy(src->copy_fn, curr);
    check_mem(new_value);

    int err = List_push(new_list, new_value);
    check(err == 0, "List_copy: failed to push new_value into new_list");
    new_value = NULL;
  }
  return new_list;

error:
  if (new_value && src->free_fn) {
    src->free_fn(new_value);
  }
  if (new_list) {
    List_destroy(new_list);
  }
  return NULL;
}

List *List_concat(List *first, List *second) {
  List *new_list = NULL;
  void *new_value = NULL;
  check(first != NULL && second != NULL,
        "List_concat: first or second list cannot be NULL");
  check(first->copy_fn == second->copy_fn && first->free_fn == second->free_fn,
        "List_concat: both lists must have the same copy/free functions");
  new_list = List_copy(first);
  check_mem(new_list);
  LIST_FOREACH(second, first, next, curr) {
    new_value = ListNode_value_copy(second->copy_fn, curr);
    check_mem(new_value);

    int err = List_push(new_list, new_value);
    check(err == 0, "List_concat: failed to push new_value into new_list");
    new_value = NULL;
  }

  return new_list;

error:
  if (new_value && second && second->free_fn) {
    second->free_fn(new_value);
  }
  if (new_list) {
    List_destroy(new_list);
  }
  return NULL;
}

List **List_split(List *list, ListNode *split_point) {
  List **result = NULL;
  void *new_value = NULL;

  check(list != NULL && split_point != NULL,
        "List_split: list and split_point cannot be NULL");

  result = calloc(2, sizeof(struct List *));
  check_mem(result);
  result[0] = List_create(list->copy_fn, list->free_fn);
  check_mem(result[0]);
  result[1] = List_create(list->copy_fn, list->free_fn);
  check_mem(result[1]);

  // if split_point not in list, then the second list is empty
  int list_sel = 0;
  LIST_FOREACH(list, first, next, curr) {
    if (curr == split_point) {
      list_sel = 1;
    }
    new_value = ListNode_value_copy(list->copy_fn, curr);
    check_mem(new_value);
    int err = List_push(result[list_sel], new_value);
    check(err == 0, "List_split: failed to push into splited lists");
    new_value = NULL;
  }
  return result;

error:
  if (new_value && list && list->free_fn) {
    list->free_fn(new_value);
  }
  if (result) {
    List_destroy(result[0]);
    List_destroy(result[1]);
    free(result);
  }
  return NULL;
}