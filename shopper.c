/**
 * shopper.c: The main logic.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linkedlist.h"

#define MAX_LINE_LENGTH 200

void strtoupper(char *str) {
  int i;
  for (i = 0; str[i]; i++)
    str[i] = toupper(str[i]);
}

int main(int argc, char *argv[]) {
  FILE *input_file;
  char line[MAX_LINE_LENGTH];
  char name[MAX_LINE_LENGTH];
  char num_str[MAX_LINE_LENGTH];
  int number;
  list_item *head = NULL;

  // Check for the correct number of args
  if (argc != 2) {
    printf("usage: %s filename.txt\n", argv[0]);
    exit(1);
  }

  input_file = fopen(argv[1], "r");

  // If fopen() returns a null pointer
  if (input_file == 0) {
    printf("Could not open file\n");
    exit(1);
  }

  // Read each line of the file
  while (fgets(line, MAX_LINE_LENGTH, input_file)) {

    // Check for add command
    if (sscanf(line, "%s %s", name, num_str) == 2) {
      number = atoi(num_str);
      strtoupper(name);
      head = update(head, name, number);
    } else {
      printf("...ERROR: improperly formatted line\n");
      exit(1);
    }
  }

  fclose(input_file);
  print_list(head);

  while (head) {
    head = delete(head, head);
  }

  return 0;

}
