#include <stdio.h>

int main(int argc, char *argv[]) {
  // go through each string in argv

  int i = argc;
  while (i--) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  // let's make our own array of strings
  char *states[] = {"California", "Oregon", "Washington", "Texas"};

  int num_states = 4;
  i = 0; // watch for this
  while (i < num_states) {
    printf("state %d: %s\n", i, states[i]);
    i++;
  }

  int i_max = argc < num_states ? argc : num_states;

  i = 0;
  while (i < i_max) {
    states[i] = argv[i];
    i++;
  }

  i = 0; // watch for this
  while (i < i_max) {
    printf("state %d: %s\n", i, states[i]);
    i++;
  }

  return 0;
}
