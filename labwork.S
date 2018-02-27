# labwork.S
# Written 2015-2017 by F Lundevall
# Skeleton file for IS1200/IS1500 lab 1.
# The macros PUSH and POP are in the public domain.
# Please add your own code at the end of the file.

#
# Please keep the two macros PUSH and POP unchanged
#
.macro	PUSH reg
addi	$sp,$sp,-4
sw	\reg,0($sp)
.endm

.macro	POP reg
lw	\reg,0($sp)
addi	$sp,$sp,4
.endm
#
# Please add your own code below this line
#
.global enable_interrupt
enable_interrupt:
ei
jr $ra
nop

.data
.text
.global hexasc
hexasc:
andi $a0, $a0, 0xf
sltiu $t0, $a0, 10 #comparison
bne $t0, 1, case2 #if statement

#case1
addi $v0, $a0, 0x30
j finish
nop

case2:
addi $v0, $a0, 0x37

finish:
jr $ra
nop

.global delay
delay:
PUSH $ra
addi $t1, $0, 4711

while:
ble $a0, $0, done
nop
addi $a0, $a0, -1
addi $t0, $0, 0

for:
beq $t0, $t1, while
nop
addi $t0, $t0, 1
j for
nop

done:
POP $ra
jr $ra
nop

.global time2string
  	  time2string:
  PUSH $ra
  PUSH $s0
  move $s0, $a0
  move $a0, $a1

  #null
  addi $t6, $0, 0
  sb $t6, 5($s0)

  #seconds digit 1
  addi $t5, $0, 0x32
  jal hexasc
  nop
  beq $v0, $v0, nottwo
  nop
  addi $t3, $0, 0x004f
  sll $t3, $t3, 16
  ori $t4, $t3, 0x5754
  add $v0, $0, $t4
nottwo:
  sw $v0, 4($s0)

  #seconds digit 2
  srl $a0, $a1, 4
  jal hexasc
  nop
  sb $v0, 3($s0)

  #colon
  addi $t7, $0, 0x3A
  sb $t7, 2($s0)

  #minutes digit 1
  srl $a0, $a1, 8
  jal hexasc
  nop
  sb $v0, 1($s0)

  #minutes digit 2
  srl $a0, $a1, 12
  jal hexasc
  nop
  sb $v0, 0($s0)

  POP $s0
  POP $ra
  jr $ra
  nop
