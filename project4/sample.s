# SPL compiler generated assembly
.data
_prmpt: .asciiz "Enter an integer: "
_eol: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prmpt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _eol
  syscall
  move $v0, $0
  jr $ra
hanoi:
# Variable name: v0
# Variable name: v1
# Variable name: v2
# Variable name: v3
# Variable name: v4
# Variable name: v5
# Variable name: v9
# Variable name: v10
# Variable name: v8
# Variable name: v11
# Variable name: v7
# Variable name: v14
# Variable name: v15
# Variable name: v13
# Variable name: v16
# Variable name: v17
# Variable name: v18
# Variable name: v22
# Variable name: v23
# Variable name: v21
# Variable name: v24
# Variable name: v20
# Variable name: v27
# Variable name: v28
# Variable name: v26
# Variable name: v29
# Variable name: v30
# Variable name: v31
# Variable name: v32
  addi $sp, $sp, -120
  sw $ra, $sp
  lw $t2, 116($sp)
  move $t1, $t2
  li $t3, 1
  beq $t1, $t3, label0
  j label1
label0:
  lw $t5, 112($sp)
  move $t4, $t5
  li $t6, 10000
  mul $t7, $t4, $t6
  lw $s1, 104($sp)
  move $s0, $s1
  add $s2, $t7, $s0
  move $a0, $s2
  jal write
  j label2
label1:
  move $s3, $t2
  li $s4, 1
  sub $s5, $s3, $s4
  move $s6, $t5
  move $s7, $s1
  lw $t0, 108($sp)
  move $t8, $t0
  sw $t8, -4($sp)
  sw $s7, -8($sp)
  sw $s6, -12($sp)
  sw $s5, -16($sp)
  sw $t1, 100($sp)
  sw $t3, 96($sp)
  sw $t4, 92($sp)
  sw $t6, 88($sp)
  sw $t7, 84($sp)
  sw $s0, 80($sp)
  sw $s2, 76($sp)
  sw $s3, 72($sp)
  sw $s4, 68($sp)
  sw $s5, 64($sp)
  sw $s6, 60($sp)
  sw $s7, 56($sp)
  sw $t8, 52($sp)
  jal hanoi
  lw $t2, 112($sp)
  move $t1, $t2
  li $t3, 10000
  mul $t4, $t1, $t3
  lw $t6, 104($sp)
  move $t5, $t6
  add $t7, $t4, $t5
  move $a0, $t7
  jal write
  lw $s1, 116($sp)
  move $s0, $s1
  li $s2, 1
  sub $s3, $s0, $s2
  lw $s5, 108($sp)
  move $s4, $s5
  move $s6, $t2
  move $s7, $t6
  sw $s7, -4($sp)
  sw $s6, -8($sp)
  sw $s4, -12($sp)
  sw $s3, -16($sp)
  sw $t1, 48($sp)
  sw $t3, 44($sp)
  sw $t4, 40($sp)
  sw $t5, 36($sp)
  sw $t7, 32($sp)
  sw $s0, 28($sp)
  sw $s2, 24($sp)
  sw $s3, 20($sp)
  sw $s4, 16($sp)
  sw $s6, 12($sp)
  sw $s7, 8($sp)
  jal hanoi
label2:
  li $t8, 0
  addi $sp, $sp, 120
  move $v0, $t8
  lw $ra ($sp)
  jr $ra
main:
# Variable name: v34
# Variable name: v33
# Variable name: v36
# Variable name: v37
# Variable name: v38
# Variable name: v39
# Variable name: v40
  addi $sp, $sp, -32
  sw $ra, $sp
  li $t0, 3
  move $t1, $t0
  move $t2, $t1
  li $t3, 1
  li $t4, 2
  li $t5, 3
  sw $t5, -4($sp)
  sw $t4, -8($sp)
  sw $t3, -12($sp)
  sw $t2, -16($sp)
  sw $t0, 28($sp)
  sw $t1, 24($sp)
  sw $t2, 20($sp)
  sw $t3, 16($sp)
  sw $t4, 12($sp)
  sw $t5, 8($sp)
  sw $t8, 4($sp)
  jal hanoi
  li $t6, 0
  addi $sp, $sp, 32
  move $v0, $t6
  lw $ra ($sp)
  jr $ra
