#ifndef MIPS_H
#define MIPS_H

#include "tac.h"
#define FALSE 0
#define TRUE 1
typedef unsigned char bool;

typedef enum {
    zero, at, v0, v1, a0, a1, a2, a3,
    t0, t1, t2, t3, t4, t5, t6, t7,
    s0, s1, s2, s3, s4, s5, s6, s7,
    t8, t9, k0, k1, gp, sp, fp, ra, NUM_REGS
} Register;


struct RegDesc {    // the register descriptor
    const char *name;
    char var[8];
    bool dirty; // value updated but not stored
    /* add other fields as you need */
} regs[NUM_REGS];

static Register rp=t0;//register loop point

struct VarDesc {    // the variable descriptor
    char var[8];
    Register reg;
    int offset; // the offset from stack
    /* add other fields as you need */
    struct VarDesc *next;
} *vars;


void mips32_gen(tac *head, FILE *_fd);

tac *emit_label(tac *);
tac *emit_function(tac *);
tac *emit_assign(tac *);
tac *emit_add(tac *);
tac *emit_sub(tac *);
tac *emit_mul(tac *);
tac *emit_div(tac *);
tac *emit_addr(tac *);
tac *emit_deref(tac *);
tac *emit_goto(tac *);
tac *emit_iflt(tac *);
tac *emit_ifle(tac *);
tac *emit_ifgt(tac *);
tac *emit_ifge(tac *);
tac *emit_ifne(tac *);
tac *emit_ifeq(tac *);
tac *emit_return(tac *);
tac *emit_dec(tac *);
tac *emit_arg(tac *);
tac *emit_call(tac *);
tac *emit_param(tac *);
tac *emit_read(tac *);
tac *emit_write(tac *);
tac *emit_fetch(tac *);
void _mips_iprintf(const char *fmt, ...);

static tac *(*emitter[])(tac *) = {
    emit_label, emit_function, emit_assign, emit_add,   emit_sub,  emit_mul,
    emit_div,   emit_addr,     emit_fetch,  emit_deref, emit_goto, emit_iflt,
    emit_ifle,  emit_ifgt,     emit_ifge,   emit_ifne,  emit_ifeq, emit_return,
    emit_dec,   emit_arg,      emit_call,   emit_param, emit_read, emit_write};


#endif // MIPS_H
