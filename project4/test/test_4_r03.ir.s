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
  addi $sp, $sp, -40
  sw $ra, ($sp)
  li $t1, 110
  li $t2, 97
  li $t3, 3
  sub $t4, $t1, $t2
  li $t5, 2
  mul $t6, $t3, $t5
  add $t7, $t4, $t6
  move $t3, $t7
  move $s0, $t3
  move $a0, $s0
  jal write
  li $s1, 0
  lw $ra ($sp)
  addi $sp, $sp, 40
  move $v0, $s1
  jr $ra
