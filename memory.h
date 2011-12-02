void print_lc4_state(unsigned short *reg, unsigned short *mem,
                     int reg_len, int mem_len, FILE *f);

void do_br(int imm9, int nzp);

void do_add(int rd, int rs, int rt);
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
