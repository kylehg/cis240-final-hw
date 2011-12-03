/**
 * memory.c: Handles the memory management.
 *
 * @author: Kyle Hardgrave <kyleh>
 * @package: trace
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.c"

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
#define I_5_4(i)  ((i >> 4) & 0x3) // For 2-bit secondary opcodes
#define I_5_3(i)  ((i >> 3) & 0x7) // For 3-bit secondary opcodes
#define I_8_7(i)  ((i >> 7) & 0x3) // For 2-bit secondary opcodes a I[8:7]
#define I_11(i)   ((i >> 11) & 0x1) // For 1-bit secondary opcodes in I[11]

#define REVERSE(x) (((x & 0x00ff) << 8) + (x >> 8))

#define D 1

int read_word(unsigned short *buffer, FILE *f) {
  int size;
  size = fread(buffer, 2, 1, f);
  *buffer = REVERSE(*buffer);
  return 1 == size;
}

unsigned short parse_instruction(unsigned short word) {
  switch (I_OP(word)) {

    // BRxxx
  case 0x0: do_br(I_8_0(word), I_11_9(word)); break;

  case 0x1: // ARITH
    switch (I_5_3(word)) {
    case 0x0: do_add(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x1: do_mul(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x2: do_sub(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x3: do_div(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    default:
      if (I_5(word)) do_addi(I_11_9(word), I_8_6(word), I_4_0(word));
      else printf("ARITH ERROR"); //TODO: Instruction error handling
    }
    break;

  case 0x2: // LOGIC
    switch(I_8_7(word)) {
    case 0x0: do_cmp(I_11_9(word), I_2_0(word)); break;
    case 0x1: do_cmpu(I_11_9(word), I_2_0(word)); break;
    case 0x2: do_cmpi(I_11_9(word), I_6_0(word)); break;
    case 0x3: do_cmpiu(I_11_9(word), I_6_0(word)); break;
    default: printf("CMP ERROR"); //TODO: Instruction erro handling
    }
    break;

  case 0x4: // JSR
    if (I_11(word) == 0) do_jsrr(I_8_6(word)); 
    else do_jsr(I_10_0(word));
    break;

  case 0x5: // LOGIC
    switch (I_5_3(word)) {
    case 0x0: do_and(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x1: do_not(I_11_9(word), I_8_6(word)); break;
    case 0x2: do_or(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x3: do_xor(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    default:
      if (I_5(word)) do_andi(I_11_9(word), I_8_6(word), I_4_0(word));
      else printf("LOGIC ERROR"); //TODO: Instruction error handling
    }
    break;

    // LDR & STR
  case 0x6: do_ldr(I_11_9(word), I_8_6(word), I_5_0(word)); break;
  case 0x7: do_str(I_11_9(word), I_8_6(word), I_5_0(word)); break;

    // RTI
  case 0x8: do_rti(); break;

    //CONST
  case 0x9: do_const(I_11_9(word), I_8_0(word)); break;

    // BITSHIFT
  case 0xA:
    switch (I_5_4(word)) {
    case 0x0: do_sll(I_11_9(word), I_8_6(word), I_3_0(word)); break;
    case 0x1: do_sra(I_11_9(word), I_8_6(word), I_3_0(word)); break;
    case 0x2: do_srl(I_11_9(word), I_8_6(word), I_3_0(word)); break;
    case 0x3: do_mod(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    default: printf("BITSHIFT ERROR\n"); //TODO: Instruction error handling
    }
    break;

    // JMP & JMPR
  case 0xC:
    if (I_11(word) == 0) do_jmpr(I_8_6(word));
    else do_jmp(I_10_0(word));
    break;

    // HICONST
  case 0xD: do_hiconst(I_11_9(word), I_7_0(word)); break;

    // TRAP
  case 0xF: do_trap(I_7_0(word)); break;

  default: printf("OPCODE ERROR"); //TODO Instruction error handling
  }

  return I_OP(word);

}


void parse_words(char type, FILE *f, unsigned short addr, 
                 unsigned short length) {
}

void parse_symbol(FILE *f, int addr, int n) {}
void parse_filename(FILE *f, int addr, int n) {}
void parse_linenumber(FILE *f, int addr, int line, 
                      int file_index) {}


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
      printf("%4x %d\n", word, size);
      switch (word) {

      case 0xcade: //Code
      case 0xdada: //Data
        type = (word == 0xcade) ? 'c' : 'd'; 
        read_word(&addr, input_file);
        read_word(&length, input_file);
        while (length > 0) {
          read_word(&word, input_file);
          mem_store('u', type, addr, word);
          length--;
        }
        break;

      case 0xc3b7: //Symbol
        type = 's'; 
        read_word(&addr, input_file);
        read_word(&length, input_file);
        fread(&word, 1, length, f);
        break;

      case 0xf17e: //File name
        type = 'f'; 
        read_word(&length, input_file);
        fread(&word, 1, length, f);
        break;

      case 0x715e: //Line number
        type = 'l';
        read_word(&addr, input_file);
        read_word(&line, input_file);
        read_word(&file_index, input_file);
        break;

      default:
        printf("HEADER READ ERROR\n");
      }

    } while (!feof(input_file));


    fclose(input_file);
  }
  
  output_file = fopen(argv[1], "w");
  print_lc4_state(reg, mem, output_file);
  return 0;

}
