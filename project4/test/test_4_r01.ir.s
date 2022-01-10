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
  lw $t2, 124($sp)
  move $t1, $t2
  li $t3, 1
  beq $t1, $t3, label0
  j label1
label0:
  lw $t5, 120($sp)
  move $t4, $t5
  li $t6, 10000
  mul $t7, $t4, $t6
  lw $s1, 112($sp)
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
  lw $t0, 116($sp)
  move $t8, $t0
  sw $t8, -4($sp)
  sw $s7, -8($sp)
  sw $s6, -12($sp)
  sw $s5, -16($sp)
  sw $t1, 108($sp)
  sw $t3, 104($sp)
  sw $t4, 100($sp)
  sw $t6, 96($sp)
  sw $t7, 92($sp)
  sw $s0, 88($sp)
  sw $s2, 84($sp)
  sw $s3, 80($sp)
  sw $s4, 76($sp)
  sw $s5, 72($sp)
  sw $s6, 68($sp)
  sw $s7, 64($sp)
  sw $t8, 60($sp)
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
  sw $t8, -4($sp)
  sw $s7, -8($sp)
  sw $s5, -12($sp)
  sw $s4, -16($sp)
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
  sw $t7, -4($sp)
  sw $t6, -8($sp)
  sw $t5, -12($sp)
  sw $t4, -16($sp)
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
