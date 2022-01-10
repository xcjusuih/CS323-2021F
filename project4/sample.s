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
fact:
  addi $sp, $sp, -20
  sw $ra, $sp
  lw $t1, 16($sp)
  li $at, 1
  beq $t1, $at, label1
  j label2
label1:
  addi $sp, $sp, 20
  move $v0, $t1
  lw $ra ($sp)
  jr $ra
label2:
  addi $t2, $t1, -1
  sw $t2, -4($sp)
  sw $t2, 12($sp)
  jal fact
  lw $t4, 16($sp)
  lw $t5, 4($sp)
  mul $t3, $t4, $t5
  addi $sp, $sp, 20
  move $v0, $t3
  lw $ra ($sp)
  jr $ra
main:
  addi $sp, $sp, -20
  sw $ra, $sp
  lw $t6, 16($sp)
  jal read
  move $t6, $v0
  move $t7, $t6
  li $at, 1
  bgt $t7, $at, label3
  j label4
label3:
  sw $t7, -4($sp)
  sw $t3, 8($sp)
  sw $t7, 12($sp)
  jal fact
  lw $s1, 4($sp)
  move $s0, $s1
  j label5
label4:
  li $s0, 1
label5:
  move $a0, $s0
  jal write
  addi $sp, $sp, 20
  li $v0, 0
  lw $ra ($sp)
  jr $ra
