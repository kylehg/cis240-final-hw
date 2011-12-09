/**
 * trace.c: The main logic.
 *
 * @author Kyle Hardgrave <kyleh>
 * @package trace
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

// Fix endian
#define REVERSE(x) (((x & 0x00ff) << 8) + (x >> 8))



/**
 * Read a 2-byte word from a file to a buffer.
 * @param buffer Pointer to the 2-byte short to store the word.
 * @param f Pointer to the file to read from.
 * @return 1 if the read was successful, else 0
 */
int read_word(unsigned short *buffer, FILE *f) {
  int size;
  size = fread(buffer, 2, 1, f);
  *buffer = REVERSE(*buffer);
  return 1 == size;
}



/**
 * The main logic - read the headers and do stuff with them.
 */
int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, 
            "usage: trace output_file last_pc file1.obj [file2.obj [...]] \n");
  }

  FILE *output_file, *input_file;
  char type;
  unsigned short word, addr, length, line, file_index, letter, f, last_pc;
  int size;

  output_file = fopen(argv[1], "w");
  if (output_file == 0) {
    fprintf(stderr, "Could not open the output file. \n");
    exit(1);
  }
 
  last_pc = (unsigned int) atoi(argv[2]);

  // For each input file
  for (f = 3; f < argc; f++) {
    input_file = fopen(argv[f], "r");
    if (input_file == 0) {
      fprintf(stderr, "Could not open input file '%s'. \n", argv[f]);
    }

    while (!feof(input_file)) {
      // Read the header
      size = read_word(&word, input_file);

      switch (word) {

      case 0xcade: //Code
      case 0xdada: //Data
        type = (word == 0xcade) ? 'c' : 'd';
        read_word(&addr, input_file);
        read_word(&length, input_file);

	// Read through the code/data contents, storing them in memory.
        while (length > 0) {
          read_word(&word, input_file);
          mem_store(addr, word);
          addr++;
          length--;
        }
        break;

      case 0xc3b7: //Symbol
        read_word(&addr, input_file);
        read_word(&length, input_file);

	// Read through the symbol, doing nothing with them.
        while (length > 0) {
          fread(&letter, 1, 1, input_file);
          length--;
        }
        break;

      case 0xf17e: //File name
        read_word(&length, input_file);

	// Read through the file name, doing nothing with them.
        while (length > 0) {
          fread(&letter, 1, 1, input_file);
          length--;
        }
        break;

      case 0x715e: //Line number
        read_word(&addr, input_file);
        read_word(&line, input_file);
        read_word(&file_index, input_file);
        break;

      default:
        printf("HEADER READ ERROR\n");
      }

      }

    run_lc4(last_pc, output_file);

    fclose(input_file);
  }
  fclose(output_file);
  //  print_lc4_state(stdout);
  return 0;

}
