/**
 * memory.c: Handles the memory management.
 *
 * @author: Kyle Hardgrave <kyleh>
 * @package: trace
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

#define I_OP(i)   (i >> 12) // For opcodes
#define I_2_0(i)  (i & 0x7) //For Rt
#define I_8_6(i)  ((i >> 6) & 0x7) // For Rs
#define I_11_9(i) ((i >> 9) & 0x7) // For Rd, and NZP codes

#define I_3_0(i)  (i & 0xF) // For IMM4 (AND w/ 15)
#define I_4_0(i)  (i & 0x1F) // For IMM5 (AND w/ 31)
#define I_5_0(i)  (i & 0x3F) // For IMM6 (AND w/ 63)
#define I_6_0(i)  (i & 0x7F) // FOR IMM7 (AND w/ 127)
#define I_7_0(i)  (i & 0xFF) // FOR IMM8 (AND w/ 255)
#define I_8_0(i)  (i * 0x1FF) // FOR IMM9 (AND w/ 511)
#define I_10_0(i) (i * 0x7FF) // FOR IMM11 (AND w/ 2047)

#define I_5(i)    ((i >> 5) & 0x1) // For 1-bit secondary opcodes
#define I_5_4(i)  ((i >> 4) & 0x2) // For 2-bit secondary opcodes
#define I_5_3(i)  ((i >> 3) & 0x7) // For 3-bit secondary opcodes
#define I_11(i)   ((i >> 11) & 0x1) // For 1-bit secondary opcodes in I[11]

#define REVERSE(x) (((x & 0x00ff) << 8) + (x >> 8))

#define D 1
#define MEM_LEN 65536
#define REG_LEN 8
unsigned short mem[MEM_LEN];
unsigned short reg[REG_LEN];


/**
 * read_word(): Special read function for words. Uses read_byte().
 *
 * @param: buffer The int to read the word into.
 * @param: *f The file to read the word from.
 * @return: TRUE if the read was successful, FALSE otherwise.
 */
int read_word(unsigned short *buffer, FILE *f) {
  int size;
  size = fread(buffer, 2, 1, f);
  *buffer = REVERSE(*buffer);
  return 1 == size;
}

void parse_code(FILE *f, int addr, int n) {
  int m;
  unsigned short word;
  if (D) printf("parse_code: <%4x> <%d>", addr, n);

  for (m = 0; m < n; m++) {
    read_word(&word, f);
    mem[addr + m] = word;

    switch (I_OP(word)) {
    case 0x0:
      printf("BR\n");
      break;

    case 0x1:
      printf("ARITH\n");
      /*      switch (I_5_3(word)) {
      case 0x0:
        printf("ADD\n");
        break;

      case 0x1:
        printf("MUL\n");
        break;

      case 0x2:
        printf("SUB\n");
        break;

      case 0x3:
        printf("DIV\n");
        break;

      default:
        if (I_5(word))
          printf("ADDI\n");
        else
          printf("ARITH ERROR");
          }*/
      break;

    case 0x2:
      printf("CMP\n");
      break;

    case 0x4:
      printf("JSR\n");
      break;

    case 0x5:
      printf("LOGIC\n");
      break;

    case 0x6:
      printf("LDR\n");
      break;

    case 0x7:
      printf("STR\n");
      break;

    case 0x8:
      printf("RTI\n");
      break;

    case 0x9:
      printf("CONST\n");
      break;

    case 0xA:
      printf("BITOPS\n");
      break;

    case 0xC:
      printf("JMP\n");
      break;

    case 0xD:
      printf("HICONST\n");
      break;

    case 0xF:
      printf("TRAP\n");
      break;

    default:
      printf("OPCODE ERROR");
    }

  }


}

void parse_data(FILE *f, int addr, int n) {}
void parse_symbol(FILE *f, int addr, int n) {}
void parse_filename(FILE *f, int addr, int n) {}
void parse_linenumber(FILE *f, int addr, int line, 
                      int file_index) {}


int main(int argc, char *argv[]) {
  // TODO: Check for arg length;

  FILE *output_file, *input_file;
  unsigned short word, addr, n, letter, mode, f;
  int size;

  // For each input file
  for (f = 2; f < argc; f++) {
    input_file = fopen(argv[2], "r");

    // Read word-by-word
    do {
      //      fread(&word, 2, 1, input_file);
      size = read_word(&word, input_file);
      if (D) printf("%4x %d\n", word, size);
      switch (word) {
      case 0xcade:
        if (D) printf("CODE\n");
        read_word(&addr, input_file);
        read_word(&n, input_file);
        parse_code(input_file, addr, n);
        break;
      case 0xdada:
        printf("CODE\n");
        break;
      case 0xc3b7:
        printf("SYMBOL\n");
        break;
      case 0xf17e:
        printf("FILE\n");
        break;
      case 0x715e:
        printf("LINE\n");
        break;
      default:
        printf("DEFAULT\n");
      }

    } while (!feof(input_file));


    fclose(input_file);
  }
  
  output_file = fopen(argv[1], "w");
  print_lc4_state(reg, mem, REG_LEN, MEM_LEN, output_file);
  return 0;

}
