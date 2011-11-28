/**
 * memory.c: Handles the memory management.
 *
 * @author: Kyle Hardgrave <kyleh>
 * @package: trace
 */

#include <stdio.h>
#include <stdlib.h>

#define MEM_LEN 65536
#define REG_LEN 8

#define I_OP(i)   ((i) >> 12) // For opcodes
#define I_2_0(i)  ((i) & 0x7) //For Rt
#define I_8_6(i)  (((i) >> 6) & 0x7) // For Rs
#define I_11_9(i) (((i) >> 9) & 0x7) // For Rd, and NZP codes

#define I_3_0(i)  ((i) & 0xF) // For IMM4 (AND w/ 15)
#define I_4_0(i)  ((i) & 0x1F) // For IMM5 (AND w/ 31)
#define I_5_0(i)  ((i) & 0x3F) // For IMM6 (AND w/ 63)
#define I_6_0(i)  ((i) & 0x7F) // FOR IMM7 (AND w/ 127)
#define I_7_0(i)  ((i) & 0xFF) // FOR IMM8 (AND w/ 255)
#define I_8_0(i)  ((i) * 0x1FF) // FOR IMM9 (AND w/ 511)
#define I_10_0(i) ((i) * 0x7FF) // FOR IMM11 (AND w/ 2047)

#define I_5(i)    (((i) >> 5) & 0x1) // For 1-bit secondary opcodes
#define I_5_4(i)  (((i) >> 4) & 0x2) // For 2-bit secondary opcodes
#define I_5_3(i)  (((i) >> 3) & 0x7) // For 3-bit secondary opcodes
#define I_11(i)   (((i) >> 11) & 0x1) // For 1-bit secondary opcodes in I[11]



int mem[MEM_LEN];
int reg[REG_LEN];


/**
 * print_lc4_state(): Prints the LC4 memory and register state to a flat file.
 *
 * When confronted with a series of NOPs in memory, it prints the first and 
 * then a set of ellipses until a new value arrises. NOTE: change the first 
 * line to change the output file. 
 *
 * @param: *f A file opened with write privileges.
 */
void print_lc4_state(FILE *f) {

  int r, m, is_nop_sequence;
  
  fputs("\n>>> REGISTER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);

  for (r = 0; r < REG_LEN; r++)
    fprintf(f, "R%d: %x :: ", r, reg[r]);

  fputs("\n\n>>> MEMORY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<<<\n", f);

  for (m = 0; m < MEM_LEN; m++) {

    // Not in a NOP sequence:
    if (!is_nop_sequence) {
      fprintf(f, "MEM[%x]:    %x\n", m, mem[m]);

      // Transition into NOP sequence
      if (mem[m] == 0) {
        is_nop_sequence = 1;
        fputs("...\n", f);
      }      
    } 

    // In a NOP sequence, transition to an OP sequence
    else if (mem[m] != 0) {
      is_nop_sequence = 0;
      fprintf(f, "MEM[%x]:    %x\n", m, mem[m]);
    }
   
  }

  fputs("\n>>> EOF <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);


}



int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s output_file input1.obj [inptu2.obj [...]]", argv[0]);
    exit(1);
  }

  FILE *output_file, *input_file;
  int word;
  int i;

  // For each input file
  //  for (i = 2; i < argc; i++) {

    input_file = fopen(argv[i], "r");


    // Read word-by-word
    while (fread(&word, 16, 1, input_file) == 1) {
      printf ("%16x\n", word);
    }
    printf("Done in while\n");
    fclose(input_file);
    //  }
  
    //  output_file = fopen(argv[1], "w");
    //  print_lc4_state(output_file);
  return 0;

}
