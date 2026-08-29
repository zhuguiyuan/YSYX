#include "dbg.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 100
char *gets(char *s);
typedef enum EyeColor {
  BLUE_EYES,
  GREEN_EYES,
  BROWN_EYES,
  BLACK_EYES,
  OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {"Blue", "Green", "Brown", "Black", "Other"};

typedef struct Person {
  int age;
  char first_name[MAX_DATA];
  char last_name[MAX_DATA];
  EyeColor eyes;
  float income;
} Person;

int read_file(const char *filename, int n, char buffer[]) {
  FILE *old_stdin = stdin;
  stdin = fopen(filename, "r");
  check(stdin != NULL, "read_file: failed to open %s", filename);
  int i, rc;
  for (i = 0; i < n - 1; ++i) {
    rc = scanf("%c", buffer + i);
    if (rc == EOF) {
      break;
    } else if (rc != 1) {
      sentinel("read_fle: failed to read %s ad %d", filename, i);
    }
  }
  buffer[i] = '\0';

  fclose(stdin);
  stdin = old_stdin;
  return 0;

error:
  if (stdin != NULL) {
    fclose(stdin);
  }
  stdin = old_stdin;
  return 1;
}

int main(int argc, char *argv[]) {
  Person you = {.age = 0};
  int i = 0;
  char *in = NULL;

  printf("What's your First Name? ");
  int rc = scanf("%99s", you.first_name);
  check(rc != 0, "Failed to read first name.");

  printf("What's your Last Name? ");
  rc = scanf("%99s", you.last_name);
  check(rc != 0, "Failed to read last name.");

  // consume white spaces until '\n' or non-whitespace
  int ch = 0;
  while (ch = getchar(), ch != EOF && isspace(ch) && ch != '\n') {
    // do nothing
  }

  char input_buffer[MAX_DATA] = {0};

  printf("How old are you? ");
  in = fgets(input_buffer, MAX_DATA - 1, stdin);
  check(in != NULL, "You have to enter a number.") you.age = atoi(input_buffer);

  printf("What color are your eyes:\n");
  for (i = 0; i <= OTHER_EYES; i++) {
    printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
  }
  printf("> ");

  int eyes = -1;
  in = fgets(input_buffer, MAX_DATA - 1, stdin);
  check(in != NULL, "You have to enter a number.");
  eyes = atoi(input_buffer);

  you.eyes = eyes - 1;
  check(you.eyes <= OTHER_EYES && you.eyes >= 0,
        "Do it right, that's not an option.");

  printf("How much do you make an hour? ");
  in = fgets(input_buffer, MAX_DATA - 1, stdin);
  check(in != NULL, "Enter a floating point number.");
  you.income = atof(input_buffer);

  printf("----- RESULTS -----\n");

  printf("First Name: %s\n", you.first_name);
  printf("Last Name: %s\n", you.last_name);
  printf("Age: %d\n", you.age);
  printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
  printf("Income: %f\n", you.income);

  char file_buffer[30];
  memset(file_buffer, 0x10, sizeof(file_buffer));
  const char *filename = "ex24p.c";
  read_file(filename, sizeof(file_buffer), file_buffer);
  printf("----- %s -----\n", filename);
  printf("%s\n", file_buffer);

  return 0;
error:

  return -1;
}