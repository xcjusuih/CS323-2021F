#include "mips32.h"

/* the output file descriptor, may not be explicitly used */
FILE *fd;

#define _tac_kind(tac) (((tac)->code).kind)
#define _tac_quadruple(tac) (((tac)->code).tac)
#define _reg_name(reg) regs[reg].name

struct VarDesc *select_var(char *name) {
    for (struct VarDesc *var = vars; var != NULL; var = var->next) {
        if (strcmp(name, var->var) == 0) {
            return var;
        }
    }
    return NULL;
}

Register get_register(tac_opd *opd) {
    // assert(opd->kind == OP_VARIABLE);
    if (opd->kind == OP_CONSTANT) {
        _mips_iprintf("li $v1, %d", opd->int_val);
        return v1;
    }
    assert(opd->kind == OP_VARIABLE);
    struct VarDesc *var = select_var(opd->char_val);
    // printf("var %s\n",opd->char_val);
    assert(var != NULL);
    // printf("var: %s, variable reg: %s, reg variable: %s\n", var->var,
    //        _reg_name(var->reg), regs[var->reg].var);
    if (var->reg != zero && strcmp(var->var, regs[var->reg].var) == 0) {
        return var->reg;
    } else {
        rp = (1 + rp - t0) % (t9 - t0) + t0;
        for (int c = 0; c < t9 - t0; c++) {  // loop in use of registers.
            int r = (c + rp - t0) % (t9 - t0) + t0;
            if (regs[r].var[0] == 0) {
                strcpy(regs[r].var, var->var);
                var->reg = r;
                _mips_iprintf("lw %s, %d($sp)", regs[r].name, var->offset);
                rp = r;
                return r;
            }
        }
    }
    var->reg = rp;
    struct VarDesc *var_replace = select_var(regs[rp].var);
    _mips_iprintf("sw %s, %d($sp)", regs[rp].name, var_replace->offset);
    var_replace->reg=zero;
    _mips_iprintf("lw %s, %d($sp)", regs[rp].name, var->offset);
    strcpy(regs[rp].var, opd->char_val);
    /* COMPLETE the register allocation */
    return rp;
}

void clear_registers() {
    for (Register r = t0; r <= t9; r++) {
        // printf("reg variable: %s\n", regs[r].var);
        if (regs[r].var[0] != 0) {
            struct VarDesc *var = select_var(regs[r].var);
            // printf("variable: %s, dirty: %d, reg: %s\n", regs[r].var,
            //    regs[r].dirty, regs[r].name);
            if (regs[r].dirty) {
                _mips_iprintf("sw %s, %d($sp)", regs[r].name, var->offset);
                regs[r].dirty = FALSE;
            }
            regs[r].var[0] = 0;
            var->reg = zero;
        }
    }
}

struct VarDesc *select_insert_var(struct VarDesc *vars, tac_opd *name,
                                  int *offset, int *var_num) {
    if (name->kind != OP_VARIABLE) {
        return NULL;
    }
    struct VarDesc *var;
    for (var = vars; var->next != NULL; var = var->next) {
        if (strcmp(var->next->var, name->char_val) == 0) {
            // printf("variable name: %s\n", var->next->var);
            return var->next;
        }
    }
    var->next = (struct VarDesc *)malloc(sizeof(struct VarDesc));
    var->next->next = NULL;
    strcpy(var->next->var, name->char_val);
    *offset -= 4;
    var->next->offset = *offset;
    (*var_num)++;
    // printf("# Variable name: %s\n", var->next->var);
    return var->next;
}

Register get_register_w(tac_opd *opd) {
    if (opd->kind == OP_CONSTANT) {
        return v1;
    }
    assert(opd->kind == OP_VARIABLE);

    struct VarDesc *var = select_var(opd->char_val);
    // printf("var %s\n", opd->char_val);
    // printf("var: %s, variable reg: %s, reg variable: %s\n", var->var,
    //        _reg_name(var->reg), regs[var->reg].var);
    assert(var != NULL);
    if (var->reg != zero && strcmp(var->var, regs[var->reg].var) == 0) {
        regs[var->reg].dirty = TRUE;
        return var->reg;
    } else {
        rp = (1 + rp - t0) % (t9 - t0) + t0;
        for (int c = 0; c < t9 - t0; c++) {  // loop in use of registers.
            int r = (c + rp - t0) % (t9 - t0) + t0;
            if (regs[r].var[0] == 0) {
                strcpy(regs[r].var, var->var);
                var->reg = r;
                rp = r;
                regs[r].dirty = TRUE;
                return r;
            }
        }
    }
    var->reg = rp;
    struct VarDesc *var_replace = select_var(regs[rp].var);
    _mips_iprintf("sw %s, %d($sp)", regs[rp].name, var_replace->offset);
    strcpy(regs[rp].var, opd->char_val);
    regs[rp].dirty = TRUE;
    var_replace->reg=zero;
    /* COMPLETE the register allocation (for write) */
    return get_register(opd);
}

void spill_register(Register reg) { /* COMPLETE the register spilling */
}

void _mips_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fputs("\n", fd);
}

void _mips_iprintf(const char *fmt, ...) {
    va_list args;
    fputs("  ", fd);  // `iprintf` stands for indented printf
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fputs("\n", fd);
}

/* PARAM: a pointer to `struct tac_node` instance
   RETURN: the next instruction to be translated */
tac *emit_label(tac *label) {
    assert(_tac_kind(label) == LABEL);
    _mips_printf("label%d:", _tac_quadruple(label).labelno->int_val);
    return label->next;
}

int push_stack(tac *function) {
    int num_vars = 0, offset = 0;
    bool exist_call = FALSE;
    for (tac *tacode = function;
         tacode != NULL && _tac_kind(tacode) != FUNCTION;
         tacode = tacode->next) {
        int exist = 0;
        if (_tac_kind(tacode) == ASSIGN) {
            select_insert_var(vars, tacode->code.assign.left, &offset,
                              &num_vars);
            select_insert_var(vars, tacode->code.assign.right, &offset,
                              &num_vars);
        } else if (_tac_kind(tacode) == ADD) {
            select_insert_var(vars, tacode->code.add.left, &offset, &num_vars);
            select_insert_var(vars, tacode->code.add.r1, &offset, &num_vars);
            select_insert_var(vars, tacode->code.add.r2, &offset, &num_vars);
        } else if (_tac_kind(tacode) == SUB) {
            select_insert_var(vars, tacode->code.sub.left, &offset, &num_vars);
            select_insert_var(vars, tacode->code.sub.r1, &offset, &num_vars);
            select_insert_var(vars, tacode->code.sub.r2, &offset, &num_vars);
        } else if (_tac_kind(tacode) == MUL) {
            select_insert_var(vars, tacode->code.mul.left, &offset, &num_vars);
            select_insert_var(vars, tacode->code.mul.r1, &offset, &num_vars);
            select_insert_var(vars, tacode->code.mul.r2, &offset, &num_vars);
        } else if (_tac_kind(tacode) == DIV) {
            select_insert_var(vars, tacode->code.div.left, &offset, &num_vars);
            select_insert_var(vars, tacode->code.div.r1, &offset, &num_vars);
            select_insert_var(vars, tacode->code.div.r2, &offset, &num_vars);
        } else if (_tac_kind(tacode) == PARAM) {
            select_insert_var(vars, tacode->code.param.p, &offset, &num_vars);
        } else if (_tac_kind(tacode) == READ) {
            select_insert_var(vars, tacode->code.param.p, &offset, &num_vars);
        } else if (_tac_kind(tacode) == CALL) {
            select_insert_var(vars, tacode->code.call.ret, &offset, &num_vars);
            exist_call = TRUE;
        }
    }
    for (struct VarDesc *var = vars; var != NULL; var = var->next) {
        if (var->offset < 0) {
            var->offset += -offset + 4;
            // printf("variable: %s, offset: %d\n", var->var, var->offset);
        }
    }
    _mips_iprintf("addi $sp, $sp, %d", offset - 4);
    _mips_iprintf("sw $ra, ($sp)");
    return num_vars;
}
void pull_stack(int var_num) {
    _mips_iprintf("lw $ra ($sp)");
    _mips_iprintf("addi $sp, $sp, %d", var_num * 4 + 4);
}

tac *emit_function(tac *function) {
    _mips_printf("\n%s:", _tac_quadruple(function).funcname);
    int var_num = push_stack(function->next);
    tac *(*tac_emitter)(tac *);
    function = function->next;
    while (function != NULL) {
        if (_tac_kind(function) != NONE) {
            if (_tac_kind(function) == FUNCTION) break;
            if (_tac_kind(function) == RETURN) {
                pull_stack(var_num);
            }
            tac_emitter = emitter[_tac_kind(function)];
            function = tac_emitter(function);
        } else {
            function = function->next;
        }
    }
    return function;
}

tac *emit_assign(tac *assign) {
    Register x, y;

    x = get_register_w(_tac_quadruple(assign).left);
    if (_tac_quadruple(assign).right->kind == OP_CONSTANT) {
        _mips_iprintf("li %s, %d", _reg_name(x),
                      _tac_quadruple(assign).right->int_val);
    } else {
        y = get_register(_tac_quadruple(assign).right);
        _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(y));
    }
    return assign->next;
}

tac *emit_add(tac *add) {
    Register x, y, z;

    x = get_register_w(_tac_quadruple(add).left);
    if (_tac_quadruple(add).r1->kind == OP_CONSTANT) {
        y = get_register(_tac_quadruple(add).r2);
        _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(y),
                      _tac_quadruple(add).r1->int_val);
    } else if (_tac_quadruple(add).r2->kind == OP_CONSTANT) {
        y = get_register(_tac_quadruple(add).r1);
        _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(y),
                      _tac_quadruple(add).r2->int_val);
    } else {
        y = get_register(_tac_quadruple(add).r1);
        z = get_register(_tac_quadruple(add).r2);
        _mips_iprintf("add %s, %s, %s", _reg_name(x), _reg_name(y),
                      _reg_name(z));
    }
    return add->next;
}

tac *emit_sub(tac *sub) {
    Register x, y, z;

    x = get_register_w(_tac_quadruple(sub).left);
    if (_tac_quadruple(sub).r1->kind == OP_CONSTANT) {
        y = get_register(_tac_quadruple(sub).r2);
        _mips_iprintf("neg %s, %s", _reg_name(y), _reg_name(y));
        _mips_iprintf("addi %s, %s, %d", _reg_name(x), _reg_name(y),
                      _tac_quadruple(sub).r1->int_val);
    } else if (_tac_quadruple(sub).r2->kind == OP_CONSTANT) {
        y = get_register(_tac_quadruple(sub).r1);
        _mips_iprintf("addi %s, %s, -%d", _reg_name(x), _reg_name(y),
                      _tac_quadruple(sub).r2->int_val);
    } else {
        y = get_register(_tac_quadruple(sub).r1);
        z = get_register(_tac_quadruple(sub).r2);
        _mips_iprintf("sub %s, %s, %s", _reg_name(x), _reg_name(y),
                      _reg_name(z));
    }
    return sub->next;
}

tac *emit_mul(tac *mul) {
    Register x, y, z;

    x = get_register_w(_tac_quadruple(mul).left);
    // if (_tac_quadruple(mul).r1->kind == OP_CONSTANT) {
    //     y = get_register_w(_tac_quadruple(mul).r1);
    //     z = get_register(_tac_quadruple(mul).r2);
    //     _mips_iprintf("lw %s, %d", _reg_name(y),
    //                   _tac_quadruple(mul).r1->int_val);
    // } else if (_tac_quadruple(mul).r2->kind == OP_CONSTANT) {
    //     y = get_register(_tac_quadruple(mul).r1);
    //     z = get_register_w(_tac_quadruple(mul).r2);
    //     _mips_iprintf("lw %s, %d", _reg_name(z),
    //                   _tac_quadruple(mul).r2->int_val);
    // } else {
        y = get_register(_tac_quadruple(mul).r1);
        z = get_register(_tac_quadruple(mul).r2);
    // }
    _mips_iprintf("mul %s, %s, %s", _reg_name(x), _reg_name(y), _reg_name(z));
    return mul->next;
}

tac *emit_div(tac *div) {
    Register x, y, z;

    x = get_register_w(_tac_quadruple(div).left);
    // if (_tac_quadruple(div).r1->kind == OP_CONSTANT) {
    //     y = get_register_w(_tac_quadruple(div).r1);
    //     z = get_register(_tac_quadruple(div).r2);
    //     _mips_iprintf("lw %s, %d", _reg_name(y),
    //                   _tac_quadruple(div).r1->int_val);
    // } else if (_tac_quadruple(div).r2->kind == OP_CONSTANT) {
    //     y = get_register(_tac_quadruple(div).r1);
    //     z = get_register_w(_tac_quadruple(div).r2);
    //     _mips_iprintf("lw %s, %d", _reg_name(z),
    //                   _tac_quadruple(div).r2->int_val);
    // } else {
        y = get_register(_tac_quadruple(div).r1);
        z = get_register(_tac_quadruple(div).r2);
    // }
    _mips_iprintf("div %s, %s", _reg_name(y), _reg_name(z));
    _mips_iprintf("mflo %s", _reg_name(x));
    return div->next;
}

tac *emit_addr(tac *addr) {
    Register x, y;

    x = get_register_w(_tac_quadruple(addr).left);
    y = get_register(_tac_quadruple(addr).right);
    _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(y));
    return addr->next;
}

tac *emit_fetch(tac *fetch) {
    Register x, y;

    x = get_register_w(_tac_quadruple(fetch).left);
    y = get_register(_tac_quadruple(fetch).raddr);
    _mips_iprintf("lw %s, 0(%s)", _reg_name(x), _reg_name(y));
    return fetch->next;
}

tac *emit_deref(tac *deref) {
    Register x, y;

    x = get_register(_tac_quadruple(deref).laddr);
    y = get_register(_tac_quadruple(deref).right);
    _mips_iprintf("sw %s, 0(%s)", _reg_name(y), _reg_name(x));
    return deref->next;
}

tac *emit_goto(tac *goto_) {
    _mips_iprintf("j label%d", _tac_quadruple(goto_).labelno->int_val);
    return goto_->next;
}

tac *emit_iflt(tac *iflt) {
    Register c1 = get_register(_tac_quadruple(iflt).c1),
             c2 = get_register(_tac_quadruple(iflt).c2);

    _mips_iprintf("blt %s, %s, label%d", _reg_name(c1), _reg_name(c2),
                  _tac_quadruple(iflt).labelno->int_val);
    /* COMPLETE emit function */
    return iflt->next;
}

tac *emit_ifle(tac *ifle) {
    Register c1 = get_register(_tac_quadruple(ifle).c1),
             c2 = get_register(_tac_quadruple(ifle).c2);

    _mips_iprintf("ble %s, %s, label%d", _reg_name(c1), _reg_name(c2),
                  _tac_quadruple(ifle).labelno->int_val);
    /* COMPLETE emit function */
    return ifle->next;
}

tac *emit_ifgt(tac *ifgt) {
    Register c1 = get_register(_tac_quadruple(ifgt).c1),
             c2 = get_register(_tac_quadruple(ifgt).c2);

    _mips_iprintf("bgt %s, %s, label%d", _reg_name(c1), _reg_name(c2),
                  _tac_quadruple(ifgt).labelno->int_val);
    /* COMPLETE emit function */
    return ifgt->next;
}

tac *emit_ifge(tac *ifge) {
    Register c1 = get_register(_tac_quadruple(ifge).c1),
             c2 = get_register(_tac_quadruple(ifge).c2);

    _mips_iprintf("bge %s, %s, label%d", _reg_name(c1), _reg_name(c2),
                  _tac_quadruple(ifge).labelno->int_val);
    /* COMPLETE emit function */
    return ifge->next;
}

tac *emit_ifne(tac *ifne) {
    Register c1 = get_register(_tac_quadruple(ifne).c1),
             c2 = get_register(_tac_quadruple(ifne).c2);

    _mips_iprintf("bne %s, %s, label%d", _reg_name(c1), _reg_name(c2),
                  _tac_quadruple(ifne).labelno->int_val);
    /* COMPLETE emit function */
    return ifne->next;
}

tac *emit_ifeq(tac *ifeq) {
    Register c1 = get_register(_tac_quadruple(ifeq).c1),
             c2 = get_register(_tac_quadruple(ifeq).c2);

    _mips_iprintf("beq %s, %s, label%d", _reg_name(c1), _reg_name(c2),
                  _tac_quadruple(ifeq).labelno->int_val);
    /* COMPLETE emit function */
    return ifeq->next;
}

tac *emit_return(tac *return_) {
    if (_tac_quadruple(return_).var->kind == OP_CONSTANT) {
        _mips_iprintf("li $v0, %d", _tac_quadruple(return_).var->int_val);
    } else {
        Register reg = get_register(_tac_quadruple(return_).var);
        _mips_iprintf("move $v0, %s", _reg_name(reg));
    }
    _mips_iprintf("jr $ra");
    /* COMPLETE emit function */
    return return_->next;
}

tac *emit_dec(tac *dec) {
    /* NO NEED TO IMPLEMENT */
    return dec->next;
}

tac *emit_arg(tac *arg) {
    int offset = 0;
    for (tac *a = arg; a->code.kind == ARG; a = a->next) {
        offset -= 4;
    }
    while (arg->code.kind == ARG) {
        Register reg = get_register(_tac_quadruple(arg).var);
        _mips_iprintf("sw %s, %d($sp)", _reg_name(reg), offset);
        arg = arg->next;
        offset += 4;
    }
    /* COMPLETE emit function */
    return arg;
}

tac *emit_call(tac *call) {
    clear_registers();
    _mips_iprintf("jal %s", _tac_quadruple(call).funcname);

    _mips_iprintf("move %s, $v0",
                  _reg_name(get_register_w(_tac_quadruple(call).ret)));
    /* COMPLETE emit function */
    return call->next;
}

tac *emit_param(tac *param) {
    get_register(param->code.param.p);
    /* COMPLETE emit function */
    return param->next;
}

tac *emit_read(tac *read) {
    Register x = get_register_w(_tac_quadruple(read).p);

    // _mips_iprintf("addi $sp, $sp, -4");
    // _mips_iprintf("sw $ra, 0($sp)");
    _mips_iprintf("jal read");
    // _mips_iprintf("lw $ra, 0($sp)");
    // _mips_iprintf("addi $sp, $sp, 4");
    _mips_iprintf("move %s, $v0", _reg_name(x));
    return read->next;
}

tac *emit_write(tac *write) {
    Register x = get_register(_tac_quadruple(write).p);

    _mips_iprintf("move $a0, %s", _reg_name(x));
    // _mips_iprintf("addi $sp, $sp, -4");
    // _mips_iprintf("sw $ra, 0($sp)");
    _mips_iprintf("jal write");
    // _mips_iprintf("lw $ra, 0($sp)");
    // _mips_iprintf("addi $sp, $sp, 4");
    return write->next;
}

void emit_preamble() {
    _mips_printf("# SPL compiler generated assembly");
    _mips_printf(".data");
    _mips_printf("_prmpt: .asciiz \"Enter an integer: \"");
    _mips_printf("_eol: .asciiz \"\\n\"");
    _mips_printf(".globl main");
    _mips_printf(".text");
}

void emit_read_function() {
    _mips_printf("read:");
    _mips_iprintf("li $v0, 4");
    _mips_iprintf("la $a0, _prmpt");
    _mips_iprintf("syscall");
    _mips_iprintf("li $v0, 5");
    _mips_iprintf("syscall");
    _mips_iprintf("jr $ra");
}

void emit_write_function() {
    _mips_printf("write:");
    _mips_iprintf("li $v0, 1");
    _mips_iprintf("syscall");
    _mips_iprintf("li $v0, 4");
    _mips_iprintf("la $a0, _eol");
    _mips_iprintf("syscall");
    _mips_iprintf("move $v0, $0");
    _mips_iprintf("jr $ra");
}

// static tac *(*emitter[])(tac *) = {
//     emit_label, emit_function, emit_assign, emit_add,   emit_sub,  emit_mul,
//     emit_div,   emit_addr,     emit_fetch,  emit_deref, emit_goto, emit_iflt,
//     emit_ifle,  emit_ifgt,     emit_ifge,   emit_ifne,  emit_ifeq,
//     emit_return, emit_dec,   emit_arg,      emit_call,   emit_param,
//     emit_read, emit_write};

tac *emit_code(tac *head) {
    tac *(*tac_emitter)(tac *);
    tac *tac_code = head;
    emit_preamble();
    emit_read_function();
    emit_write_function();
    while (tac_code != NULL) {
        if (_tac_kind(tac_code) != NONE) {
            tac_emitter = emitter[_tac_kind(tac_code)];
            tac_code = tac_emitter(tac_code);
        } else {
            tac_code = tac_code->next;
        }
    }
    return head;
}

/* translate a TAC list into mips32 assembly
   output the textual assembly code to _fd */
void mips32_gen(tac *head, FILE *_fd) {
    regs[zero].name = "$zero";
    regs[at].name = "$at";
    regs[v0].name = "$v0";
    regs[v1].name = "$v1";
    regs[a0].name = "$a0";
    regs[a1].name = "$a1";
    regs[a2].name = "$a2";
    regs[a3].name = "$a3";
    regs[t0].name = "$t0";
    regs[t1].name = "$t1";
    regs[t2].name = "$t2";
    regs[t3].name = "$t3";
    regs[t4].name = "$t4";
    regs[t5].name = "$t5";
    regs[t6].name = "$t6";
    regs[t7].name = "$t7";
    regs[s0].name = "$s0";
    regs[s1].name = "$s1";
    regs[s2].name = "$s2";
    regs[s3].name = "$s3";
    regs[s4].name = "$s4";
    regs[s5].name = "$s5";
    regs[s6].name = "$s6";
    regs[s7].name = "$s7";
    regs[t8].name = "$t8";
    regs[t9].name = "$t9";
    regs[k0].name = "$k0";
    regs[k1].name = "$k1";
    regs[gp].name = "$gp";
    regs[sp].name = "$sp";
    regs[fp].name = "$fp";
    regs[ra].name = "$ra";
    vars = (struct VarDesc *)malloc(sizeof(struct VarDesc));
    vars->next = NULL;
    fd = _fd;
    emit_code(head);
}
