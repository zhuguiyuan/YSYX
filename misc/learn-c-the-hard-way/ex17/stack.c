#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct Stack;
int Stack_init(struct Stack *stack, size_t init_cap, void (*item_free)(void *));
void Stack_deinit(struct Stack *stack);
int Stack_push(struct Stack *stack, void *item);
void *Stack_pop(struct Stack *stack);
size_t Stack_size(const struct Stack *stack);

struct Stack {
  size_t cap;
  size_t size;
  void **payload;
  void (*item_free)(void *);
};

/** stack 只能是刚创建或者刚 deinit 之后的对象 */
int Stack_init(struct Stack *stack, size_t init_cap,
               void (*item_free)(void *)) {
  if (stack == NULL) {
    return -1;
  }
  stack->payload = NULL;
  stack->item_free = NULL;
  stack->cap = 0;
  stack->size = 0;

  if (init_cap < 4) {
    init_cap = 4;
  }
  stack->payload = calloc(init_cap, sizeof(*stack->payload));
  if (stack->payload == NULL) {
    return -1;
  }
  stack->item_free = item_free;
  stack->cap = init_cap;

  return 0;
}

void Stack_deinit(struct Stack *stack) {
  if (stack != NULL) {
    if (stack->item_free != NULL) {
      for (size_t i = 0; i < stack->size; ++i) {
        stack->item_free(stack->payload[i]);
      }
    }
    free(stack->payload);
    stack->payload = NULL;
    stack->item_free = NULL;
    stack->cap = 0;
    stack->size = 0;
  }
}

int Stack_push(struct Stack *stack, void *item) {
  if (stack == NULL || item == NULL) {
    return -1;
  }
  if (stack->size == stack->cap) {
    if (stack->cap > SIZE_MAX / 2) {
      return -1;
    }
    size_t new_cap = stack->cap * 2;
    void **new_payload =
        reallocarray(stack->payload, new_cap, sizeof(*stack->payload));
    if (new_payload == NULL) {
      return -1;
    }
    stack->payload = new_payload;
    stack->cap = new_cap;
  }
  stack->payload[(stack->size)++] = item;
  return 0;
}

void *Stack_pop(struct Stack *stack) {
  if (stack == NULL || stack->size == 0) {
    return NULL;
  }
  size_t index = stack->size - 1;
  void *item = stack->payload[index];
  stack->payload[index] = NULL;
  stack->size -= 1;
  return item;
}

size_t Stack_size(const struct Stack *stack) {
  if (stack == NULL) {
    return 0;
  }
  return stack->size;
}
