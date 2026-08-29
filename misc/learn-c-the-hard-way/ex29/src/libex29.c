#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

int print_a_message(const char *msg, size_t n) {
  printf("A STRING: ");
  fwrite(msg, 1, n, stdout);
  printf("\n");

  return 0;
}

int uppercase(const char *msg, size_t n) {
  size_t i = 0;

  for (i = 0; i < n; i++) {
    printf("%c", toupper((unsigned char)msg[i]));
  }

  printf("\n");

  return 0;
}

int lowercase(const char *msg, size_t n) {
  size_t i = 0;

  for (i = 0; i < n; i++) {
    printf("%c", tolower((unsigned char)msg[i]));
  }

  printf("\n");

  return 0;
}

int fail_on_purpose(const char *msg, size_t n) {
  (void)msg;
  (void)n;
  return 1;
}
