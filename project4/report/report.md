# Compiler Project 4 Report

## 1. Introduction

In this project, the main task is to generate assambly code from three-address code from starter code. We designed a stack-based allocator for variables with range of functions. 

## 2. How to Run This Project

Run `make splc` and you can get `bin/splc` as executable file. Run `make testing` and the result of `test/*.ir` will be generated automatically to `test/*.ir.s`. Most of our code is inside `mips32.c` file and we change a little in other source code to suit our enviroment. 

## 3. Design & Implement

### 3.1 Stack structure

We use stack to manage all variables inside functions. As there is no global variables in this project test cases, we do not consider them and all variables are listed in stack. Within a function, we look ahead all instructions and scan all variables and build a variable list, mapping them with offsets. 

When a function start, the compiler will allocate the spaces of all variables on stack and `addi $sp $sp value` to give every variable some place to store. The parameters in this function are the most highest place and beneath is the local variables, and the place `($sp)` is reserved to `$ra`. The order of variables are same to the occurance of them in tac. If calling function, the arguement will be pushed to lower place then `($sp)` and we do not use a registers to store them. When a function end, it will load the $ra value and pull the stack to the origin place. At last call `jr $ra`. Beneath is the stack structure of our stacks. 

```
param 1 //args for caller
param 2
param 3
variable 1
variable 2
return address  --stack point
arg 1 //args for callee and stack point will be push down. 
arg 2
arg 3
```

### 3.2 Register Allocator

In this project, we design a simple algorithm to allocate the register, which is like `turntable`. We make t0 to t9 as the target registers for variables. When there is no need to call some function, which means to store dirty variables, and the register need to replace the value, this register can hold the variable. The allocator will find the next allocatable register for next variable from t0 to t9 and to t0 as a turntable. While there is a empty register and allocator meets a new variable, the allocator will map it to the target variable. If there is no empty register, and the allocator will make the next register to the target register and clear the origin information of previous variable. 

If the variable is on the right(to assign others), the register could load value from stack if the variable was not allocated with register. If the variable is on the left(to be assigned with some value), the allocator will choose a register like above and mark them as `dirty`. When calling a function, all dirty register should save the value to the stack and all variable will lost their register and the variable should be reloaded from stack. The main code for this algorithm is beneath. 

```c
    if (var->reg != zero && strcmp(var->var, regs[var->reg].var) == 0) {
        return var->reg;
    } else {
        rp = (1 + rp - t0) % (t9 - t0) + t0;
        for (int c = 0; c < t9 - t0; c++) {  // turntable to allocate registers.
            int r = (c + rp - t0) % (t9 - t0) + t0;
            if (regs[r].var[0] == 0) {
                strcpy(regs[r].var, var->var);
                var->reg = r;
                _mips_iprintf("lw %s, %d($sp)", regs[rp].name, var->offset);
                rp = r;
                return r;
            }
        }
    }
    var->reg = rp;
    struct VarDesc *var_replace = select_var(regs[rp].var);
    _mips_iprintf("sw $%s, %d($sp)", regs[rp].name, var_replace->offset);
    var_replace->reg=zero;
    strcpy(regs[rp].var, opd->char_val);

```

Within the small improvement, we reduce about 1/2 code in `test/test_4_r01` as the register can be reused and to save less values. As a easy improvement, the allocator can check whether the variable will be used in next instructions and only store the value to be used in next instructions. 

## 4. Labor Division

In our team we have 3 members

11812613 香佳宏 11810602 陈栋林  11712224 曹瑞德

and we actually implement this project together in several days in a meeting room,and specifically we have equal contribution. Some details are hard to judge simply by labor since every one spend a lot in debuging, discussion and helping each other.

- 香佳宏  design & implement stack structures
- 陈栋林  register allocation algorithm && generating instructions
- 曹瑞德  report && debuger && tester

