#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  // let's make our own array of strings
  char *states[] = {"California", "Oregon", "Washington", "Texas"};

  assert(argc >= 2);
  argv[1] = states[0];
  states[0] = argv[0];

  int i = 0;

  // go through each string in argv
  // why am I skipping argv[0]?
  for (i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  int num_states = 4;
  for (i = 0; i < num_states; i++) {
    printf("state %d: %s\n", i, states[i]);
  }


  int j = 0;
  for (int(*foo)(void) = NULL; j += 1, j < 10; ++j) {
    printf("foo = %p\n", foo);
    printf("j = %d\n", j);
  }

  return 0;
}
