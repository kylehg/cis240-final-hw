#include <stdio.h>
#include <stdlib.h>

#define D 1

#define MEM_LEN 65536
#define REG_LEN 8

unsigned short mem[MEM_LEN];
unsigned short reg[REG_LEN];

/** @returns 0 if executed properly, 1 if a data-versus-code error, 2 if a
    os-versus-user error. */
int mem_store(char perm, char type, unsigned short addr, unsigned short word){
  // OS?
  if (perm = 'o' && (0x8000 > addr || addr < 0xA000)) return 2;
  // Return false if attempt to load in incorrect place
  if (type == 'c' && addr > 0x3FFF) return 1;
  if (type == 'd' && (addr < 0x4000 || addr > 0x7FFF)) return 0;

  mem[addr] = word;
}

void do_add(int rd, int rs, int rt) {
  printf("ADD R%d, R%d, R%d \n", rd, rs, rt);
}
void do_mul(int rd, int rs, int rt) {
  printf("MUL R%d, R%d, R%d \n", rd, rs, rt);
}
void do_sub(int rd, int rs, int rt) {
  printf("SUB R%d, R%d, R%d \n", rd, rs, rt);
}
void do_div(int rd, int rs, int rt) {
  printf("DIV R%d, R%d, R%d \n", rd, rs, rt);
}
void do_addi(int rd, int rs, int imm5) {
  printf("ADD R%d, R%d, #%d \n");
}

void do_cmp(int rs, int rt) {
  printf("CMP R%d, R%d \n", rs, rt);
}
void do_cmpu(int rs, int rt) {
  printf("CMPU R%d, R%d \n", rs, rt);
}
void do_cmpi(int rs, int imm7) {
  printf("CMPI R%d, #%d \n", rs, imm7);
}
void do_cmpiu(int rs, int uimm7) {
  printf("CMPIU R%d, #%d \n", rs, imm7);
}

void do_jsr(int imm11) {
  printf("JSR 0x%x \n", imm11);
}
void do_jsrr(int rs) {
  printf("JSRR R%d \n", rs);
}

void do_and(int rd, int rs, int rt) {
  printf("AND R%d, R%d, R%d \n", rd, rs, rt);
}
void do_not(int rd, int rs) {
  printf("NOT R%d, R%d \n", rd, rs);
}
void do_or(int rd, int rs, int rt) {
  printf("OR R%d, R%d, R%d \n", rd, rs, rt);
}
void do_xor(int rd, int rs, int rt) {
  printf("XOR R%d, R%d, R%d \n", rd, rs, rt);
}
void do_andi(int rd, int rs, int imm5) {
  printf("AND R%d, #%d \n", rd, imm5);
}

void do_ldr(int rd, int rs, int imm6) {
  printf("LDR R%d, R%d, #%d \n", rd, rs, imm6);
}
void do_str(int rt, int rs, int imm6) {
  printf("STR R%d, R%d, #%d \n", rd, rs, imm6);
}

void do_rti() {
  printf("RTI \n");
}

void do_const(int rd, int imm9) {
  printf("CONST R%d, 0x%x \n", rd, imm9);
}

void do_sll(int rd, int rs, int uimm4) {
  printf("SLL R%d, R%d, #%d \n", rd, rs, uimm4);
}
void do_sra(int rd, int rs, int uimm4) {
  printf("SRA R%d, R%d, #%d \n", rd, rs, uimm4);
}
void do_srl(int rd, int rs, int uimm4) {
  printf("SRL R%d, R%d, #%d \n", rd, rs, uimm4);
}
void do_mod(int rd, int rs, int rt) {
  printf("MOD R%d, R%d, R%d \n", rd, rs, rt);
}

void do_jmpr(int rs) {
  printf("JMPR R%d \n", rs);
}
void do_jmp(int imm11) {
  printf("JMP 0x%x \n", imm11);
}

void do_hiconst(int rd, int uimm8) {
  printf("HICONST R%d, 0x%x \n", rd, uimm8);
}

void do_trap(int uimm8) {
  printf("HICONST 0x%x \n", uimm8);
}



void print_lc4_state(int reg_len, int mem_len, FILE *f) {
  int r, m, is_nop_sequence;
  reg_len = REG_LEN
  mem_len = MEM_LEMP

  fputs("\n>>> REGISTER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);

  for (r = 0; r < reg_len; r++)
    fprintf(f, "R%d: %x | ", r, reg[r]);

  fputs("\n\n>>> MEMORY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<<<\n", f);

  for (m = 0; m < mem_len; m++) {

    // Not in a NOP sequence:
    if (!is_nop_sequence) {

      fprintf(f, "MEM[%4x]: ", m, parse_instruction(mem[m]));

      // Transition into NOP sequence
      if (mem[m] == 0) {
        is_nop_sequence = 1;
        fputs("...\n", f);
      }
    }

    // In a NOP sequence, transition to an OP sequence
    else if (mem[m] != 0) {
      is_nop_sequence = 0;
      fprintf(f, "MEM[%4x]: ", m, parse_instruction(mem[m]));
    }
  }

  fputs("\n>>> EOF <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<", f);
  fputs("<<<<<<<<<<<<<<<<<\n", f);
}
