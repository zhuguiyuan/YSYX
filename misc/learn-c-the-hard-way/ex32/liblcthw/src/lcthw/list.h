#ifndef lcthw_list_h
#define lcthw_list_h

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
  struct ListNode *next;
  struct ListNode *prev;
  void *value;
} ListNode;

typedef void (*List_free_fn)(void *);
typedef void *(*List_copy_fn)(void *);

typedef struct List {
  int count;
  ListNode *first;
  ListNode *last;
  /** When copy_fn == NULL, copy will be shallow */
  List_copy_fn copy_fn;
  /** When copy_fn == NULL, lifecycle will not be managed */
  List_free_fn free_fn;
} List;

List *List_create(List_copy_fn copy_fn, List_free_fn free_fn);
/** free all the nodes and value fields and the list */
void List_destroy(List *list);
/** free all the nodes and value fields */
void List_clear(List *list);

#define List_count(A) (assert((A) != NULL), (A)->count)
#define List_first(A)                                                          \
  (assert((A) != NULL), (A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A)                                                           \
  (assert((A) != NULL), (A)->last != NULL ? (A)->last->value : NULL)

/** push back */
int List_push(List *list, void *value);
/** pop back */
void *List_pop(List *list);

/** push front */
int List_unshift(List *list, void *value);
/** pop front */
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);

List *List_copy(List *src);

List *List_concat(List *first, List *second);

List **List_split(List *list, ListNode *split_point);

#define LIST_FOREACH(L, S, M, V)                                               \
  ListNode *_node = NULL;                                                      \
  ListNode *V = NULL;                                                          \
  for (V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif // lcthw_list_h
