#include <stdio.h>
#include <stdlib.h>

#define D 1

#define MEM_LEN 65536
#define REG_LEN 8

unsigned short mem[MEM_LEN];
unsigned short reg[REG_LEN];

void do_br(int imm9, int nzp) {
  printf("BR%c%c%c %4x \n", ((nzp & 0x4) == 0x4 ? 'n' : ' '), 
         ((nzp & 0x2) == 0x2 ? 'z' : ' '), ((nzp & 0x1) == 0x1 ? 'p' : ' '),
         imm9);
}

void do_add(int rd, int rs, int rt) {
  
}
void do_mul(int rd, int rs, int rt);
void do_sub(int rd, int rs, int rt);
void do_div(int rd, int rs, int rt);
void do_addi(int rd, int rs, int imm5);

void do_cmp(int rs, int rt);
void do_cmpu(int rs, int rt);
void do_cmpi(int rs, int imm7);
void do_cmpiu(int rs, int uimm7);

void do_jsr(int imm11);
void do_jsrr(int rs);

void do_and(int rd, int rs, int rt);
void do_not(int rd, int rs);
void do_or(int rd, int rs, int rt);
void do_xor(int rd, int rs, int rt);
void do_andi(int rd, int rs, int imm5);

void do_ldr(int rd, int rs, int imm6);
void do_str(int rt, int rs, int imm6);

void do_rti();

void do_const(int rd, int imm9);
void do_sll(int rd, int rs, int uimm4);
void do_sra(int rd, int rs, int uimm4);
void do_srl(int rd, int rs, int uimm4);
void do_mod(rd, rs, rt);

void do_jmpr(int rs);
void do_jmp(int imm11);
void do_hiconst(int rd, int uimm8);
void do_trap(int uimm8);



void print_lc4_state(unsigned short *reg, unsigned short *mem,
                     int reg_len, int mem_len, FILE *f) {
  int r, m, is_nop_sequence;

  fputs("\n>>> REGISTER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);

  for (r = 0; r < reg_len; r++)
    fprintf(f, "R%d: %x | ", r, reg[r]);

  fputs("\n\n>>> MEMORY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<<<\n", f);

  for (m = 0; m < mem_len; m++) {

    // Not in a NOP sequence:
    if (!is_nop_sequence) {

      fprintf(f, "MEM[%4x]:    %x\n", m, mem[m]);

      // Transition into NOP sequence
      if (mem[m] == 0) {
        is_nop_sequence = 1;
        fputs("...\n", f);
      }
    }

    // In a NOP sequence, transition to an OP sequence
    else if (mem[m] != 0) {
      is_nop_sequence = 0;
      fprintf(f, "MEM[%4x]:    %x\n", m, mem[m]);
    }
  }

  fputs("\n>>> EOF <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);
}
