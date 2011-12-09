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
  if (argc < 4) {
    fprintf(stderr, 
            "usage: trace output_file last_pc file1.obj [file2.obj [...]] \n");
  }

  FILE *output_file, *input_file;
  char type;
  unsigned short word, addr, length, line, file_index, letter, f, last_pc;
  int size;

  output_file = fopen(argv[1], "r");
  last_pc = (unsigned int) atoi(argv[2]);

  // For each input file
  for (f = 3; f < argc; f++) {
    input_file = fopen(argv[3], "r");

    while (!feof(input_file)) {
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
          mem_store(addr, word);
          addr++;
          length--;
        }
        break;

      case 0xc3b7: //Symbol
        type = 's';
        read_word(&addr, input_file);
        read_word(&length, input_file);
        printf("C3B7: <%4x> %d \n", addr, length);
        while (length > 0) {
          fread(&letter, 1, 1, input_file);
          printf("...%2x \n", letter);
          length--;
        }
        break;

      case 0xf17e: //File name
        type = 'f';
        read_word(&length, input_file);
        printf("F17E: %d \n", length);
        while (length > 0) {
          fread(&letter, 1, 1, input_file);
          printf("...%2x \n", letter);
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

      }

    run_lc4(last_pc);

    fclose(input_file);
  }

  output_file = fopen(argv[1], "w");
  print_lc4_state(stdout);
  return 0;

}
