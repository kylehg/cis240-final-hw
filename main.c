/**
 * trace.c: The main logic for reading and parsing the file.
 *
 * @author Kyle Hardgrave <kyleh>
 * @package trace
 */

#include <stdio.h>
#include <stdlib.h>
#include "lc4.h"

#define I_OP(i)   (i >> 12) // For opcodes
#define I_2_0(i)  (i & 0x7) //For Rt
#define I_8_6(i)  ((i >> 6) & 0x7) // For Rs
#define I_11_9(i) ((i >> 9) & 0x7) // For Rd, and NZP codes

#define I_3_0(i)  (i &   0xF) // For IMM4 (AND w/ 15)
#define I_4_0(i)  (i &  0x1F) // For IMM5 (AND w/ 31)
#define I_5_0(i)  (i &  0x3F) // For IMM6 (AND w/ 63)
#define I_6_0(i)  (i &  0x7F) // FOR IMM7 (AND w/ 127)
#define I_7_0(i)  (i &  0xFF) // FOR IMM8 (AND w/ 255)
#define I_8_0(i)  (i & 0x1FF) // FOR IMM9 (AND w/ 511)
#define I_10_0(i) (i & 0x7FF) // FOR IMM11 (AND w/ 2047)

#define I_5(i)   ((i >>  5) & 0x1) // For 1-bit secondary opcodes
#define I_5_4(i) ((i >>  4) & 0x3) // For 2-bit secondary opcodes
#define I_5_3(i) ((i >>  3) & 0x7) // For 3-bit secondary opcodes
#define I_8_7(i) ((i >>  7) & 0x3) // For 2-bit secondary opcodes a I[8:7]
#define I_11(i)  ((i >> 11) & 0x1) // For 1-bit secondary opcodes in I[11]

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

    fclose(input_file);
  }

  run_lc4(last_pc, output_file);
  fclose(output_file);
  //  print_lc4_state(stdout);
  return 0;

}
