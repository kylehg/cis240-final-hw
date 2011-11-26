/**
 * linkedlist.c: The code implementing the linked list structure.
 *
 * @author Kyle Hardgrave <kyleh>
 * @package shopper
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "linkedlist.h"


/**
 * lookup(): Lookup and return a list_item.
 *
 * @param head The pointer to the first list_item in the list.
 * @param name The name of the list_item to look up.
 * @return A pointer to the list_item, or NULL if not found.
 */
list_item *lookup(list_item *head, char *name) {

  list_item *current = head;

  while (current != NULL) {
    if (strcmp(name, current->name) == 0)
      return current;
    current = current->next;
  }

  return NULL;
}

/**
 * update(): Update the give list_item count.
 *
 * Note that it is the job of this function to check if the item exists and
 * either update it or add to it accordingly. If an element is then at 0, it
 * should delete it.
 *
 * @param head The pointer to the first list_item in the list.
 * @param name The name of the list_item to look up.
 * @param num The value by which to increment or decrement the list_item.
 * @return A pointer to the new list head (which may be the same as the old).
 */
list_item *update(list_item *head, char *name, int number) {
  
  list_item *item = lookup(head, name);
  int original_numer;

  /*
   * If the requested item does not already exist, create it.
   */
  if (item == NULL) {

    // Alocate and check the item struct itself.
    item = malloc(sizeof(*item));    
    if (item == NULL) {
      printf("update(): malloc() failed to allocate a new list element.\n");
      exit(1);
    }

    // Allocate and check the name.
    item->name = malloc(strlen(name) + 1); // +1 to leave a space for null
    if (item->name == NULL) {
      printf("update(): malloc() failed to allocate a new element name.\n");
      exit(1);
    }
    strcpy(item->name, name);

    // The number doesn't need heap allocation
    item->number = number;

    // Add to the beginning of the list, cause it's easy
    item->prev = NULL;
    item->next = head;

    return item;
  }


  /*
   * If the item does exist, update it. 
   * (If the new value would make the item number negative, just zero it.)
   */
  printf("%s: %d (old), %d (update), ", item->name, item->number, number);
  item->number = (item->number + number < 0) ? 0 : item->number + number;
  printf("%d (new)\n", item->number);
  

  /*
   * If the item is now at zero, delete it.
   */
  if (item->number == 0)
    head = delete(head, item);

  return head;
}

/**
 * delete(): Delete a list_item
 *
 * @param head The first element in the list.
 * @param item The list_item to delete.
 * @return A pointer to the new head of the list, which may not have changed.
 */
list_item *delete(list_item *head, list_item *item) {
  list_item *prev_item, *next_item;

  if (head == NULL || item == NULL)
    return head;

  prev_item = item->prev;
  next_item = item->next;

  if (prev_item)
    prev_item->next = item->next;
     
  if (next_item)
    next_item->prev = item->prev;

  free(item->name);
  free(item);

  if (item == head)
    return next_item;
  else
    return head;  
}


void print_list(list_item *head) {
  list_item *item = head;

  while (item) {
    printf("%s: %d\n", item->name, item->number);
    item = item->next;
  }
}
