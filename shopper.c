/**
 * main.c: Oh you know.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_LINE_LENGTH 200

int main() {

  char input[MAX_LINE_LENGTH];
  char name[MAX_LINE_LENGTH];
  char num_str[MAX_LINE_LENGTH];
  int number;
  list_item *head = NULL;

    printf("Enter command (update, print, quit):\n");
  while (1) {
    printf(">>> ");
    fgets(input, MAX_LINE_LENGTH, stdin);

    // Check for add command
    if (sscanf(input, "update %s %s", name, num_str) == 2) {
      number = atoi(num_str);
      head = update(head, name, number);
      printf("%s: %d\n", name, number);
    }

    // Check for list command
    if (input[0] == 'p') {
      print_list (head);
    }

    // check for quit command
    if (input[0] == 'q') {
      while (head) {
        head = delete(head, head);
      }

      exit(0);
    }

  }

  return 0;

}
