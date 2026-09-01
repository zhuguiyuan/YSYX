#include <stdio.h>

int main(void) {
  // printf(""); // warning
  printf("%s", ""); // ok
  printf("% hhd, %-10hd, %+10d\n", 256, 256, 256);
  printf("%#hho, %#ho, %#o\n", 256, 256, 256);
  printf("%#hhx, %#hx, %#x\n", 256, 256, 256);
  puts("");

  int x = 2048;
  printf("%p\n", &x);
  printf("%Lf \n", (long double)x);
  printf("%Le \n", (long double)x);
  printf("%Lg \n", (long double)x);
  int n = 0;
  printf("hello%n%d\n", &n, n);
  printf("%d\n", n);
  printf("%m\n");
  return 0;
}