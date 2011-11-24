/**
 * linkedlist.h - Defines the linked list structure used for the shopping list
 *
 * @author Kyle Hardgrave <kyleh>
 * @package shopper
 */

typedef struct list_item_def {
  char *name;
  int number;
  struct list_item_def *prev, *next;
} list_item;

list_item *lookup(list_item *head, char *name);
list_item *update(list_item *head, char *name, int number);
void print_list(list_item *head);
