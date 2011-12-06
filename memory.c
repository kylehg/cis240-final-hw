#include <stdio.h>
#include <stdlib.h>

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

#define D 1

#define MEM_LEN 65536
#define REG_LEN 8

unsigned short mem[MEM_LEN];
unsigned short reg[REG_LEN];
unsigned short pc;
unsigned short psr;
unsigned short last_addr;

/** @returns 0 if executed properly, 1 if a data-versus-code error, 2 if a
    os-versus-user error. */
int mem_store(char type, unsigned short addr, unsigned short word){
  // OS?
  //  if (perm == 'o' && (0x8000 > addr || addr < 0xA000)) return 2;
  // Return false if attempt to load in incorrect place
  if (type == 'c' && addr > 0x3FFF) return 1;
  if (type == 'd' && (addr < 0x4000 || addr > 0x7FFF)) return 1;

  mem[addr] = word;
  return 0;
}

void do_br(int nzp, short imm9) {
  char n = ((nzp & 0x4) == 0x4) ? 'n' : ' ';
  char z = ((nzp & 0x2) == 0x2) ? 'z' : ' ';
  char p = ((nzp & 0x1) == 0x1) ? 'p' : ' ';
  printf("BR%c%c%c 0x%4x \n", n, z, p, imm9);

}

void do_add(int rd, int rs, int rt) {
  printf("ADD R%d, R%d, R%d \n", rd, rs, rt);

  reg[rd] = reg[rs] + reg[rt];
  // SET NZPREG
}
void do_mul(int rd, int rs, int rt) {
  printf("MUL R%d, R%d, R%d \n", rd, rs, rt);

  reg[rd] = reg[rs] * reg[rt];
  // SET NZPREG
}
void do_sub(int rd, int rs, int rt) {
  printf("SUB R%d, R%d, R%d \n", rd, rs, rt);

  reg[rd] = reg[rs] - reg[rt];
  // SET NZPREG
}
void do_div(int rd, int rs, int rt) {
  printf("DIV R%d, R%d, R%d \n", rd, rs, rt);

  reg[rd] = reg[rs] / reg[rt];
  // SET NZPREG
}
void do_addi(int rd, int rs, short imm5) {
  printf("ADD R%d, R%d, #%d \n", rd, rs, imm5);

  reg[rd] = reg[rs] + imm5;
  // SET NZPREG
}

void do_cmp(int rs, int rt) {
  printf("CMP R%d, R%d \n", rs, rt);
}
void do_cmpu(int rs, int rt) {
  printf("CMPU R%d, R%d \n", rs, rt);
}
void do_cmpi(int rs, short imm7) {
  printf("CMPI R%d, #%d \n", rs, imm7);
}
void do_cmpiu(int rs, unsigned short uimm7) {
  printf("CMPIU R%d, #%d \n", rs, uimm7);
}

void do_jsr(short imm11) {
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
void do_andi(int rd, int rs, short imm5) {
  printf("AND R%d, #%d \n", rd, imm5);
}

void do_ldr(int rd, int rs, short imm6) {
  printf("LDR R%d, R%d, #%d \n", rd, rs, imm6);
}
void do_str(int rt, int rs, short imm6) {
  printf("STR R%d, R%d, #%d \n", rt, rs, imm6);
}

void do_rti() {
  printf("RTI \n");
}

void do_const(int rd, short imm9) {
  printf("CONST R%d, 0x%x \n", rd, imm9);
}

void do_sll(int rd, int rs, unsigned short uimm4) {
  printf("SLL R%d, R%d, #%d \n", rd, rs, uimm4);
}
void do_sra(int rd, int rs, unsigned short uimm4) {
  printf("SRA R%d, R%d, #%d \n", rd, rs, uimm4);
}
void do_srl(int rd, int rs, unsigned short uimm4) {
  printf("SRL R%d, R%d, #%d \n", rd, rs, uimm4);
}
void do_mod(int rd, int rs, int rt) {
  printf("MOD R%d, R%d, R%d \n", rd, rs, rt);
}

void do_jmpr(int rs) {
  printf("JMPR R%d \n", rs);
}
void do_jmp(short imm11) {
  printf("JMP 0x%x \n", imm11);
}

void do_hiconst(int rd, unsigned short uimm8) {
  printf("HICONST R%d, 0x%x \n", rd, uimm8);
}

void do_trap(unsigned short uimm8) {
  printf("HICONST 0x%x \n", uimm8);
}

short sext(short n, unsigned short len) {
  if (len > 15) {
    fprintf(stderr, "sext(0x%4x, %d): Cannot have length >15 \n", n, len);
    exit(1);
  }
  short a = n << (16 - len);
  short b = a >> (16 - len);
  return b;
}

unsigned short parse_instruction(unsigned short word) {
  switch (I_OP(word)) {

    // BRxxx
  case 0x0: 
    if (I_11_9(word) != 0) 
      do_br(I_11_9(word), sext(I_8_0(word), 9)); 
    break;

  case 0x1: // ARITH
    switch (I_5_3(word)) {
    case 0x0: do_add(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x1: do_mul(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x2: do_sub(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x3: do_div(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    default:
      if (I_5(word)) 
        do_addi(I_11_9(word), I_8_6(word), sext(I_4_0(word), 5));
      else printf("ARITH ERROR"); //TODO: Instruction error handling
    }
    break;

  case 0x2: // LOGIC
    switch(I_8_7(word)) {
    case 0x0: do_cmp(I_11_9(word), I_2_0(word)); break;
    case 0x1: do_cmpu(I_11_9(word), I_2_0(word)); break;
    case 0x2: do_cmpi(I_11_9(word), sext(I_6_0(word), 7)); break;
    case 0x3: do_cmpiu(I_11_9(word), I_6_0(word)); break;
    default: printf("CMP ERROR"); //TODO: Instruction erro handling
    }
    break;

  case 0x4: // JSR
    if (I_11(word) == 0) 
      do_jsrr(I_8_6(word));
    else do_jsr(I_10_0(word));
    break;

  case 0x5: // LOGIC
    switch (I_5_3(word)) {
    case 0x0: do_and(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x1: do_not(I_11_9(word), I_8_6(word)); break;
    case 0x2: do_or(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x3: do_xor(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    default:
      if (I_5(word)) 
        do_andi(I_11_9(word), I_8_6(word), sext(I_4_0(word), 5));
      else printf("LOGIC ERROR"); //TODO: Instruction error handling
    }
    break;

    // LDR & STR
  case 0x6: do_ldr(I_11_9(word), I_8_6(word), sext(I_5_0(word), 6)); break;
  case 0x7: do_str(I_11_9(word), I_8_6(word), sext(I_5_0(word), 6)); break;

    // RTI
  case 0x8: do_rti(); break;

    //CONST
  case 0x9: do_const(I_11_9(word), sext(I_8_0(word), 9)); break;

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
    if (I_11(word) == 0) 
      do_jmpr(I_8_6(word));
    else do_jmp(sext(I_10_0(word), 11));
    break;

    // HICONST
  case 0xD: do_hiconst(I_11_9(word), I_7_0(word)); break;

    // TRAP
  case 0xF: do_trap(I_7_0(word)); break;

  default: printf("OPCODE ERROR"); //TODO Instruction error handling
  }

  return I_OP(word);

}


/**
 * Print the state of the machine, passing over long stretches of nothingness
 */
void print_lc4_state(FILE *f) {
  int r, m, is_nop_sequence;

  fputs("\n>>> REGISTERS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", f);

  for (r = 0; r < REG_LEN; r++)
    fprintf(f, "R%d: %x | ", r, reg[r]);

  fprintf(f, "\nPC: %d | PSR: 0x%4X \n", pc, psr);



  fputs("\n>>> MEMORY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", f);

  for (m = 0; m < MEM_LEN; m++) {

    // Only print if there's content, or if it's the first 0 of a NOP sequence.
    if (mem[m] != 0 || !is_nop_sequence) {
      is_nop_sequence = 0;
      fprintf(f, "MEM[%4x]: ", m);

      // In data areas, write hex values, in code areas, instructions
      if ((0x3FFF < m && m < 0x8000) || (0x9FFF < m)) {
        fprintf(f, "0x%X \n", mem[m]);
      } else {
        parse_instruction(mem[m]);
      }

      // Transition to a NOP sequence if it's a 0
      if (mem[m] == 0) {
        is_nop_sequence = 1;
        fputs("...\n", f);
      }
    }
  }

  fputs("\n>>> EOLC4 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", f);
}
