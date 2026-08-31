#include "minunit.h"
#include <lcthw/list_algos.h>
#include <stdio.h>
#include <string.h>

static char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5

static List *create_words(void) {
  List *words = List_create(NULL, NULL);

  for (int i = 0; i < NUM_VALUES; i++) {
    List_push(words, values[i]);
  }

  return words;
}

static int is_sorted(List *words) {
  LIST_FOREACH(words, first, next, cur) {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      return 0;
    }
  }

  return 1;
}

static int has_valid_links(List *list) {
  int count = 0;
  ListNode *prev = NULL;

  LIST_FOREACH(list, first, next, cur) {
    if (cur->prev != prev) {
      return 0;
    }
    prev = cur;
    count++;
  }

  return count == List_count(list) && prev == list->last &&
         (list->first == NULL || list->first->prev == NULL) &&
         (list->last == NULL || list->last->next == NULL);
}

static char *check_sort(List_sort sort) {
  List *words = create_words();
  mu_assert(words != NULL, "Failed to create words list.");

  int rc = sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Sort failed.");
  mu_assert(List_count(words) == NUM_VALUES, "Sort changed the list count.");
  mu_assert(is_sorted(words), "Words are not sorted.");
  mu_assert(has_valid_links(words), "Sort corrupted the list links.");

  rc = sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Sort of an already sorted list failed.");
  mu_assert(is_sorted(words), "Already sorted words are no longer sorted.");
  mu_assert(has_valid_links(words), "Repeated sort corrupted list links.");
  List_destroy(words);

  char storage[65][4];
  words = List_create(NULL, NULL);
  mu_assert(words != NULL, "Failed to create multi-level input.");
  for (int i = 64; i >= 0; --i) {
    snprintf(storage[i], sizeof(storage[i]), "%02d", i);
    mu_assert(List_push(words, storage[i]) == 0,
              "Failed to populate multi-level input.");
  }
  rc = sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Sort failed on multi-level input.");
  mu_assert(List_count(words) == 65,
            "Sort changed the multi-level input count.");
  mu_assert(is_sorted(words), "Multi-level input is not sorted.");
  mu_assert(has_valid_links(words),
            "Sort corrupted the multi-level input links.");
  List_destroy(words);

  words = List_create(NULL, NULL);
  mu_assert(words != NULL, "Failed to create empty list.");
  rc = sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Sort failed on an empty list.");
  mu_assert(List_count(words) == 0, "Sort changed an empty list.");
  mu_assert(has_valid_links(words), "Empty list links are invalid.");
  List_destroy(words);

  return NULL;
}

static char *test_all_sorts(void) {
  List_sort sorts[] = {List_bubble_sort, List_merge_sort,
                       List_merge_sort_inplace, List_merge_sort_bottom_up};

  for (size_t i = 0; i < sizeof(sorts) / sizeof(sorts[0]); ++i) {
    char *error = check_sort(sorts[i]);
    if (error != NULL) {
      return error;
    }
  }

  return NULL;
}

static char *test_insert_sorted(void) {
  List *words = List_create(NULL, NULL);
  mu_assert(words != NULL, "Failed to create list.");

  mu_assert(List_insert_sorted(words, "b", (List_compare)strcmp) == 0,
            "Failed to insert first value.");
  mu_assert(List_insert_sorted(words, "d", (List_compare)strcmp) == 0,
            "Failed to insert value at the end.");
  mu_assert(List_insert_sorted(words, "a", (List_compare)strcmp) == 0,
            "Failed to insert value at the beginning.");
  mu_assert(List_insert_sorted(words, "c", (List_compare)strcmp) == 0,
            "Failed to insert value in the middle.");

  mu_assert(List_count(words) == 4, "Insert sorted changed the list count.");
  mu_assert(strcmp(List_first(words), "a") == 0, "First value is not sorted.");
  mu_assert(strcmp(words->first->next->value, "b") == 0,
            "Second value is not sorted.");
  mu_assert(strcmp(words->first->next->next->value, "c") == 0,
            "Third value is not sorted.");
  mu_assert(strcmp(List_last(words), "d") == 0, "Last value is not sorted.");
  mu_assert(is_sorted(words), "Values are not sorted after insertion.");
  mu_assert(has_valid_links(words), "Insert sorted corrupted list links.");

  List_destroy(words);
  return NULL;
}

static char *all_tests(void) {
  mu_suite_start();

  mu_run_test(test_all_sorts);
  mu_run_test(test_insert_sorted);

  return NULL;
}

RUN_TESTS(all_tests);
