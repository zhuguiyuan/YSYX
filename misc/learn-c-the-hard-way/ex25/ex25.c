/** WARNING: This code is fresh and potentially isn't correct yet. */

#include "dbg.h"

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DATA 100

int read_string(char *out_string, int max_buffer) {
  int i, ch = 0;
  for (i = 0; i < max_buffer - 1; ++i) {
    ch = fgetc(stdin);
    if (ch == EOF) {
      check(!ferror(stdin), "Input error.");
      break;
    }
    out_string[i] = ch;
    if (ch == '\n') {
      ++i;
      break;
    }
  }
  out_string[i] = '\0';
  return 0;

error:
  return -1;
}

int read_int(int *out_int) {
  char *input = calloc(1, MAX_DATA);
  int rc = read_string(input, MAX_DATA);
  check(rc == 0, "Failed to read number.");

  *out_int = atoi(input);

  free(input);
  return 0;

error:
  if (input)
    free(input);
  return -1;
}

int read_scan(const char *fmt, ...) {
  int i = 0;
  int rc = 0;
  int *out_int = NULL;
  char *out_char = NULL;
  char *out_string = NULL;
  int max_buffer = 0;

  va_list argp;
  va_start(argp, fmt);

  for (i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      i++;
      switch (fmt[i]) {
      case '\0':
        sentinel("Invalid format, you ended with %%.");
        break;

      case 'd':
        out_int = va_arg(argp, int *);
        rc = read_int(out_int);
        check(rc == 0, "Failed to read int.");
        break;

      case 'c':
        out_char = va_arg(argp, char *);
        *out_char = fgetc(stdin);
        break;

      case 's':
        max_buffer = va_arg(argp, int);
        out_string = va_arg(argp, char *);
        rc = read_string(out_string, max_buffer);
        check(rc == 0, "Failed to read string.");
        break;

      default:
        sentinel("Invalid format.");
      }
    } else {
      fgetc(stdin);
    }

    check(!feof(stdin) && !ferror(stdin), "Input error.");
  }

  va_end(argp);
  return 0;

error:
  va_end(argp);
  return -1;
}

int print_int(int d) {
  int nc = 0;
  unsigned int du = 0;
  char buffer[sizeof(int) * CHAR_BIT + 1];
  if (d < 0) {
    fputc('-', stdout);
    du = 0u - (unsigned int)d;
    nc += 1;
  } else {
    du = d;
  }

  int i = 0;
  do {
    buffer[i++] = (char)('0' + du % 10);
    du /= 10;
  } while (du > 0);
  while (i > 0) {
    fputc(buffer[--i], stdout);
    ++nc;
  }

  return nc;
}

int print_fmt(const char *fmt, ...) {
  int nc = 0;
  int rc;
  va_list argp;
  va_start(argp, fmt);

  for (int i = 0; fmt[i] != '\0'; ++i) {
    if (fmt[i] == '%') {
      i++;
      switch (fmt[i]) {
      case '\0':
        sentinel("Invalid format, you ended with %%.");
        break;

      case 'd':
        rc = print_int(va_arg(argp, int));
        check(rc != EOF, "print_fmt: failed to print int");
        nc += rc;
        break;

      case 'c':
        rc = fputc((char)va_arg(argp, int), stdout);
        check(rc != EOF, "print_fmt: failed to print char");
        nc += rc;
        break;

      case 's':
        rc = fputs(va_arg(argp, const char *), stdout);
        check(rc != EOF, "print_fmt: failed to print string");
        nc += rc;
        break;

      default:
        sentinel("Invalid format.");
      }

    } else {
      rc = fputc(fmt[i], stdout);
      check(rc != EOF, "print_fmt: failed to print");
      nc += rc;
    }
  }

  va_end(argp);
  return nc;

error:
  return EOF;
}

int main(int argc, char *argv[]) {
  char first_name[MAX_DATA] = {0};
  char initial = ' ';
  char last_name[MAX_DATA] = {0};
  int age = 0;

  print_fmt("What's your first name? ");
  int rc = read_scan("%s", MAX_DATA, first_name);
  check(rc == 0, "Failed first name.");

  print_fmt("What's your initial? ");
  rc = read_scan("%c\n", &initial);
  check(rc == 0, "Failed initial.");

  print_fmt("What's your last name? ");
  rc = read_scan("%s", MAX_DATA, last_name);
  check(rc == 0, "Failed last name.");

  print_fmt("How old are you? ");
  rc = read_scan("%d", &age);

  print_fmt("---- RESULTS ----\n");
  print_fmt("First Name: %s", first_name);
  print_fmt("Initial: '%c'\n", initial);
  print_fmt("Last Name: %s", last_name);
  print_fmt("Age: %d\n", age);

  return 0;
error:
  return -1;
}