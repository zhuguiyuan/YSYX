#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

typedef int (*List_compare)(const void *a, const void *b);
typedef int (*List_sort)(List *list, List_compare cmp);

int List_bubble_sort(List *list, List_compare cmp);

int List_merge_sort(List *list, List_compare cmp);

int List_merge_sort_inplace(List *list, List_compare cmp);

int List_insert_sorted(List *list, void *value, List_compare cmp);

int List_merge_sort_bottom_up(List *list, List_compare cmp);

#endif
