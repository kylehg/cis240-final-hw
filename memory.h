#include <stdio.h>
#include <stdlib.h>

int mem_store(char type, unsigned short addr, unsigned short word);

void do_br(int nzp, short imm9);

void do_add(int rd, int rs, int rt);
void do_mul(int rd, int rs, int rt);
void do_sub(int rd, int rs, int rt);
void do_div(int rd, int rs, int rt);
void do_addi(int rd, int rs, short imm5);

void do_cmp(int rs, int rt);
void do_cmpu(int rs, int rt);
void do_cmpi(int rs, short imm7);
void do_cmpiu(int rs, unsigned short uimm7);

void do_jsr(short imm11);
void do_jsrr(int rs);

void do_and(int rd, int rs, int rt);
void do_not(int rd, int rs);
void do_or(int rd, int rs, int rt);
void do_xor(int rd, int rs, int rt);
void do_andi(int rd, int rs, short imm5);

void do_ldr(int rd, int rs, short imm6); 
void do_str(int rt, int rs, short imm6);

void do_rti();

void do_const(int rd, short imm9);

void do_sll(int rd, int rs, unsigned short uimm4); 
void do_sra(int rd, int rs, unsigned short uimm4); 
void do_srl(int rd, int rs, unsigned short uimm4);
void do_mod(int rd, int rs, int rt);

void do_jmpr(int rs);
void do_jmp(short imm11);

void do_hiconst(int rd, unsigned short uimm8);

void do_trap(unsigned short uimm8);

short sext(short n, unsigned short len);

unsigned short parse_instruction(unsigned short word);

void print_lc4_state(FILE *f);
