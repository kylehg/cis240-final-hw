#include <stdio.h>
#include <stdlib.h>

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

#define MEM_LEN 65536
#define REG_LEN 8

short mem[MEM_LEN];
short reg[REG_LEN];
unsigned short pc;
unsigned short psr;
unsigned short last_addr;


/**
 * A simple interface for storing things in memory.
 */
void mem_store(unsigned short addr, unsigned short word) {
  mem[addr] = word;
}

/**
 * A simple interface for getting things from memory.
 */
short mem_load(unsigned short addr) {
  return mem[addr];
}


/**
 * Sign extend the given len-bit number.
 * @param n The number.
 * @param len The length of the number.
 * @return The sign-extended number.
 */
short sext(short n, unsigned short len) {
  if (len > 15) {
    fprintf(stderr, "sext(0x%4x, %d): Cannot have length >15 \n", n, len);
    exit(1);
  }
  short a = n << (16 - len);
  short b = a >> (16 - len);
  //  printf("sext(): %x -> %x -> %x \n", n, a, b);
  return b;
}



/**
 * Set the NZP bits and increments PC.
 * @param value The value to be tested for setting the NZP bits.
 */
void set_nzp(short value) {
  unsigned short nzp;

  if (value > 0) 
    nzp = 0x1;
  else if(value == 0)
    nzp = 0x2;
  else
    nzp = 0x4;
  psr = psr & 0xFFF8; // Set the NZP bits to 000
  psr += nzp; // Set the NZP bits
  pc++;
  //  printf("set_nzp(%d): 0x%X | PC = 0x%4X \n", value, psr, pc);
}


// Branch
void do_br(unsigned short nzp, short imm9) {
  imm9 = sext(imm9, 9);
  unsigned short old_nzp = I_2_0(psr);
  char n = ((nzp & 0x4) == 0x4) ? 'n' : 0;
  char z = ((nzp & 0x2) == 0x2) ? 'z' : 0;
  char p = ((nzp & 0x1) == 0x1) ? 'p' : 0;
  printf("BR%c%c%c 0x%4x \n", n, z, p, imm9);
  if ((old_nzp & nzp) > 0) {
    pc += 1 + imm9;
  } else {
    pc++;
  }
}


// Arithmetic Ops
void do_add(int rd, int rs, int rt) {
  printf("ADD R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] + reg[rt];
  set_nzp(reg[rd]);
}
void do_mul(int rd, int rs, int rt) {
  printf("MUL R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] * reg[rt];
  set_nzp(reg[rd]);
}
void do_sub(int rd, int rs, int rt) {
  printf("SUB R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] - reg[rt];
  set_nzp(reg[rd]);
}
void do_div(int rd, int rs, int rt) {
  printf("DIV R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] / reg[rt];
  set_nzp(reg[rd]);
}
void do_addi(int rd, int rs, short imm5) {
  imm5 = sext(imm5, 5);
  printf("ADD R%d, R%d, #%d \n", rd, rs, imm5);
  reg[rd] = reg[rs] + imm5;
  set_nzp(reg[rd]);
}
void do_mod(int rd, int rs, int rt) {
  printf("MOD R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] % reg[rt];
  set_nzp(reg[rd]);
}


// Comparisons
void do_cmp(int rs, int rt) {
  printf("CMP R%d, R%d \n", rs, rt);
  short cmp = (short) reg[rs] - (short) reg[rt];
  set_nzp(cmp);
}
void do_cmpu(int rs, int rt) {
  printf("CMPU R%d, R%d \n", rs, rt);
  unsigned short cmp = reg[rs] - reg[rt];
  set_nzp(cmp);
}
void do_cmpi(int rs, short imm7) {
  imm7 = sext(imm7, 7);
  printf("CMPI R%d, #%d \n", rs, imm7);
  short cmp = (short) reg[rs] - imm7;
  set_nzp(cmp);
}
void do_cmpiu(int rs, unsigned short uimm7) {
  printf("CMPIU R%d, #%d \n", rs, uimm7);
  unsigned short cmp = reg[rs] - uimm7;
  set_nzp(cmp);
}


// Jumps
void do_jsr(short imm11) {
  printf("JSR 0x%x \n", imm11);
  reg[7] = pc+1;
  pc = (pc & 0x8000) | (imm11 << 4);
}
void do_jsrr(int rs) {
  printf("JSRR R%d \n", rs);
  reg[7] = pc+1;
  pc = reg[rs];
}
void do_jmpr(int rs) {
  printf("JMPR R%d \n", rs);
  pc = reg[rs];
}
void do_jmp(short imm11) {
  imm11 = sext(imm11, 11);
  printf("JMP 0x%x \n", imm11);
  pc += 1 + imm11;
}


// Logical Operations
void do_and(int rd, int rs, int rt) {
  printf("AND R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] & reg[rt];
  set_nzp(reg[rd]);
}
void do_not(int rd, int rs) {
  printf("NOT R%d, R%d \n", rd, rs);
  reg[rd] = ~reg[rs];
  set_nzp(reg[rd]);
}
void do_or(int rd, int rs, int rt) {
  printf("OR R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] | reg[rt];
  set_nzp(reg[rd]);
}
void do_xor(int rd, int rs, int rt) {
  printf("XOR R%d, R%d, R%d \n", rd, rs, rt);
  reg[rd] = reg[rs] ^ reg[rt];
  set_nzp(reg[rd]);
}
void do_andi(int rd, int rs, short imm5) {
  imm5 = sext(imm5, 5);
  printf("AND R%d, #%d \n", rd, imm5);
  reg[rd] = reg[rs] & imm5;
  set_nzp(reg[rd]);
}


// Shift Ops
void do_sll(int rd, int rs, unsigned short uimm4) {
  printf("SLL R%d, R%d, #%d \n", rd, rs, uimm4);
  reg[rd] = reg[rs] << uimm4;
  set_nzp(reg[rd]);
}
void do_sra(int rd, int rs, unsigned short uimm4) {
  printf("SRA R%d, R%d, #%d \n", rd, rs, uimm4);
  short shift = reg[rs] >> uimm4;
  reg[rd] = shift;
  set_nzp(reg[rd]);
}
void do_srl(int rd, int rs, unsigned short uimm4) {
  printf("SRL R%d, R%d, #%d \n", rd, rs, uimm4);
  unsigned short shift = reg[rs] >> uimm4;
  reg[rd] = shift;
  set_nzp(reg[rd]);
}


// Memory Ops
void do_ldr(int rd, int rs, short imm6) {
  imm6 = sext(imm6, 6);
  printf("LDR R%d, R%d, #%d \n", rd, rs, imm6);
  reg[rd] = mem[reg[rs] + imm6];
  set_nzp(reg[rd]);
}
void do_str(int rt, int rs, short imm6) {
  imm6 = sext(imm6, 6);
  unsigned short addr = reg[rs] + imm6;
  printf("STR R%d, R%d, #%d \n", rt, rs, imm6);
  if (psr < 0x8000 && addr >= 0x8000) { // If PSR[15] = 0{
    fprintf(stderr, "Illegal memory access: Not in OS mode! \n");
    exit(1);
  }
  if (addr < 0x4000 || (0x7FFF < addr && addr < 0xA000)) {
    fprintf(stderr, "Illegal memory access: Cannot write to code sections!\n");
    exit(1);
  }
  mem[reg[rs] + imm6] = reg[rt];
  pc++; // Since other functions incr PC w/ set_nzp(), but STR doesn't set NZP
}


// OS Ops
void do_rti() {
  pc = reg[7];
  psr = psr & 0x7FFF; //PSR[15] = 0
  printf("RTI \n");
}
void do_trap(unsigned short uimm8) {
  printf("TRAP 0x%x \n", uimm8);
  reg[7] = pc+1;
  pc = 0x8000 | uimm8;
  psr = psr | 0x8000;
}


// Register Ops
void do_const(int rd, short imm9) {
  imm9 = sext(imm9, 9);
  printf("CONST R%d, 0x%x \n", rd, imm9);
  reg[rd] = imm9;
  set_nzp(reg[rd]);
}
void do_hiconst(int rd, unsigned short uimm8) {
  printf("HICONST R%d, 0x%x \n", rd, uimm8);
  reg[rd] = (reg[rd] & 0xFF) | (uimm8 << 8);
  set_nzp(reg[rd]);
}



/**
 * Parse a 2-byte word and execute the proper LC4 function.
 * @param word The instruction to parse.
 */
void parse_instruction(unsigned short word) {
  switch (I_OP(word)) {

    // BRxxx
  case 0x0: 
    if (I_11_9(word) != 0) 
      do_br(I_11_9(word), I_8_0(word)); 
    break;

  case 0x1: // ARITH
    switch (I_5_3(word)) {
    case 0x0: do_add(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x1: do_mul(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x2: do_sub(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    case 0x3: do_div(I_11_9(word), I_8_6(word), I_2_0(word)); break;
    default:
      if (I_5(word)) {
        do_addi(I_11_9(word), I_8_6(word), I_4_0(word));
      } else {
	fprintf(stderr, "Error parsing arith instruction.\n"); 
	exit(1);
      }
    }
    break;

  case 0x2: // LOGIC
    switch(I_8_7(word)) {
    case 0x0: do_cmp(I_11_9(word), I_2_0(word)); break;
    case 0x1: do_cmpu(I_11_9(word), I_2_0(word)); break;
    case 0x2: do_cmpi(I_11_9(word), I_6_0(word)); break;
    case 0x3: do_cmpiu(I_11_9(word), I_6_0(word)); break;
    default: 
      fprintf(stderr, "Error parsing cmp instruction.\n"); 
      exit(1);
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
      if (I_5(word)) {
        do_andi(I_11_9(word), I_8_6(word), I_4_0(word));
      } else {
	fprintf(stderr, "Error parsing logic instruction.\n"); 
	exit(1);
      }
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
    default: 
      fprintf(stderr, "Error parsing bitshift instruction.\n"); 
      exit(1);
    }
    break;

    // JMP & JMPR
  case 0xC:
    if (I_11(word) == 0) 
      do_jmpr(I_8_6(word));
    else do_jmp(I_10_0(word));
    break;

    // HICONST
  case 0xD: do_hiconst(I_11_9(word), I_7_0(word)); break;

    // TRAP
  case 0xF: do_trap(I_7_0(word)); break;

  default: 
    fprintf(stderr, "Error parsing instruction opcode.\n");
    exit(1);
  }
}



/**
 * Run the LC4 machine on the loaded memory.
 * @param last_pc The PC to stop on.
 * @param output_file The file to print to.
 */
void run_lc4(unsigned short last_pc, FILE *output_file) {
  pc = 0;
  psr = 0x2;
  
  while (last_pc != pc) {
    parse_instruction(mem[pc]);
  } 
  printf("Done. \n");
}



/**
 * Print the register, PC, and PSR state to stdout.
 */
void print_reg_state() {
  int r;
  printf("PC x%4hX: ", pc);
  for (r = 0; r < REG_LEN; r++)
    printf("R%d x%4hX|", r, reg[r]);
  printf("PSR x%4hX \n", psr);
}



/**
 * Print the state of the machine to stdout, passing over long stretches of 
 * nothingness.
 */
void print_lc4_state() {
  int r, m, is_nop_sequence;

  printf("\n>>> REGISTERS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  print_reg_state();

  printf("\n>>> MEMORY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

  for (m = 0; m < MEM_LEN; m++) {

    // Only print if there's content, or if it's the first 0 of a NOP sequence.
    if (mem[m] != 0 || !is_nop_sequence) {
      is_nop_sequence = 0;
      printf("MEM[%4x]: 0x%X \n", m, mem[m]);

      // Transition to a NOP sequence if it's a 0
      if (mem[m] == 0) {
        is_nop_sequence = 1;
        printf("...\n");
      }
    }
  }

  printf("\n>>> EOLC4 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}
