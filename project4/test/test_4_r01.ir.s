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
  addi $sp, $sp, -128
  sw $ra, ($sp)
  lw $t1, 124($sp)
  lw $t2, 120($sp)
  lw $t3, 116($sp)
  lw $t4, 112($sp)
  move $t5, $t1
  li $t6, 1
  beq $t5, $t6, label0
  j label1
label0:
  move $t7, $t2
  li $s0, 10000
  mul $s1, $t7, $s0
  move $s2, $t4
  add $s3, $s1, $s2
  move $a0, $s3
  jal write
  j label2
label1:
  move $s4, $t1
  li $s5, 1
  sub $s6, $s4, $s5
  move $s7, $t2
  move $t8, $t4
  move $t0, $t3
  sw $t0, -16($sp)
  sw $t8, -12($sp)
  sw $s7, -8($sp)
  sw $s6, -4($sp)
  sw $t0, 60($sp)
  sw $t5, 108($sp)
  sw $t6, 104($sp)
  sw $t7, 100($sp)
  sw $s0, 96($sp)
  sw $s1, 92($sp)
  sw $s2, 88($sp)
  sw $s3, 84($sp)
  sw $s4, 80($sp)
  sw $s5, 76($sp)
  sw $s6, 72($sp)
  sw $s7, 68($sp)
  sw $t8, 64($sp)
  jal hanoi
  move $t1, $v0
  lw $t3, 120($sp)
  move $t2, $t3
  li $t4, 10000
  mul $t5, $t2, $t4
  lw $t7, 112($sp)
  move $t6, $t7
  add $s0, $t5, $t6
  move $a0, $s0
  jal write
  lw $s2, 124($sp)
  move $s1, $s2
  li $s3, 1
  sub $s4, $s1, $s3
  lw $s6, 116($sp)
  move $s5, $s6
  move $s7, $t3
  move $t8, $t7
  sw $t8, -16($sp)
  sw $s7, -12($sp)
  sw $s5, -8($sp)
  sw $s4, -4($sp)
  sw $t1, 56($sp)
  sw $t2, 52($sp)
  sw $t4, 48($sp)
  sw $t5, 44($sp)
  sw $t6, 40($sp)
  sw $s0, 36($sp)
  sw $s1, 32($sp)
  sw $s3, 28($sp)
  sw $s4, 24($sp)
  sw $s5, 20($sp)
  sw $s7, 16($sp)
  sw $t8, 12($sp)
  jal hanoi
  move $t0, $v0
label2:
  li $t1, 0
  lw $ra ($sp)
  addi $sp, $sp, 128
  move $v0, $t1
  jr $ra

main:
  addi $sp, $sp, -36
  sw $ra, ($sp)
  li $t2, 3
  move $t3, $t2
  move $t4, $t3
  li $t5, 1
  li $t6, 2
  li $t7, 3
  sw $t7, -16($sp)
  sw $t6, -12($sp)
  sw $t5, -8($sp)
  sw $t4, -4($sp)
  sw $t0, 8($sp)
  sw $t1, 4($sp)
  sw $t2, 32($sp)
  sw $t3, 28($sp)
  sw $t4, 24($sp)
  sw $t5, 20($sp)
  sw $t6, 16($sp)
  sw $t7, 12($sp)
  jal hanoi
  move $s0, $v0
  li $s1, 0
  lw $ra ($sp)
  addi $sp, $sp, 36
  move $v0, $s1
  jr $ra
