/**
 * memory.c: Handles the memory management.
 *
 * @author: Kyle Hardgrave <kyleh>
 * @package: trace
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

#define REVERSE(x) (((x & 0x00ff) << 8) + (x >> 8))
#define D 1

int read_word(unsigned short *buffer, FILE *f) {
  int size;
  size = fread(buffer, 2, 1, f);
  *buffer = REVERSE(*buffer);
  return 1 == size;
}

int main(int argc, char *argv[]) {
  // TODO: Check for arg length;

  FILE *output_file, *input_file;
  unsigned short word, addr, length, line, file_index, letter, f;
  char type;
  int size;

  // For each input file
  for (f = 2; f < argc; f++) {
    input_file = fopen(argv[2], "r");

    do {
      // Read the header
      size = read_word(&word, input_file);
      printf("%4x \n", word);
      switch (word) {

      case 0xcade: //Code
      case 0xdada: //Data
        type = (word == 0xcade) ? 'c' : 'd';
        read_word(&addr, input_file);
        read_word(&length, input_file);

        while (length > 0) {
          read_word(&word, input_file);
          parse_instruction(word);
          mem_store(type, addr, word);
          length--;
        }
        break;

      case 0xc3b7: //Symbol
        type = 's';
        read_word(&addr, input_file);
        read_word(&length, input_file);
        printf("C3B7: <%4x> %d \n", addr, length);
        while (length > 0) {
          fread(&word, 1, 1, input_file);
          printf("...%2x \n", word);
          length--;
        }
        break;

      case 0xf17e: //File name
        type = 'f';
        read_word(&length, input_file);
        printf("F17E: %d \n", length);
        while (length > 0) {
          fread(&word, 1, 1, input_file);
          printf("...%2x \n", word);
          length--;
        }
        break;

      case 0x715e: //Line number
        type = 'l';
        read_word(&addr, input_file);
        read_word(&line, input_file);
        read_word(&file_index, input_file);
        printf("715E: <0x%4x> %d %d \n", addr, line, file_index);
        break;

      default:
        printf("HEADER READ ERROR\n");
      }

    } while (!feof(input_file));


    fclose(input_file);
  }

  output_file = fopen(argv[1], "w");
  print_lc4_state(stdout);
  return 0;

}
