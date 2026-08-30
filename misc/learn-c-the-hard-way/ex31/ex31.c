#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int i = 1, y;

  while (i < 100) {
    usleep(3000);
    i = (i + 1) % 100;
    y = 100 / i;
    printf("%d\n", y);
  }

  return 0;
}