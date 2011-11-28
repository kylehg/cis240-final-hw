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

int mem[MEM_LEN];
int reg[REG_LEN];


/**
 * print_lc4_state(): Prints the LC4 memory and register state to a flat file.
 *
 * When confronted with a series of NOPs in memory, it prints the first and 
 * then a set of ellipses until a new value arrises. NOTE: change the first 
 * line to change the output file. 
 */
void print_lc4_state() {

  FILE *f = fopen("test.txt", "w");
  int r, m, is_nop_sequence;
  
  fputs("\n>>> REGISTER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);

  for (r = 0; r < REG_LEN; r++)
    fprintf(f, "R%d: %x\n", r, reg[r]);

  fputs("\n>>> MEMORY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);

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



int main() {

  mem[0] = 4000;
  mem[1] = 2000;
  mem[42] = 92;
  mem[512] = 4096;

  print_lc4_state();

}
