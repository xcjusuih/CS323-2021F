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

main:
  addi $sp, $sp, -48
  sw $ra, ($sp)
  lw $t1, 44($sp)
  jal read
  move $t1, $v0
  move $t2, $t1
  move $t3, $t2
  li $t4, 0
  bgt $t3, $t4, label0
  j label1
label0:
  li $t5, 1
  move $a0, $t5
  jal write
  j label2
label1:
  move $t6, $t2
  li $t7, 0
  blt $t6, $t7, label3
  j label4
label3:
  li $s0, 1
  neg $s0, $s0
  addi $s1, $s0, 0
  move $a0, $s1
  jal write
  j label5
label4:
  li $s2, 0
  move $a0, $s2
  jal write
label5:
label2:
  li $s3, 0
  lw $ra ($sp)
  addi $sp, $sp, 48
  move $v0, $s3
  jr $ra
