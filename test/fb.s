.global _start

.set noreorder
.text
_start:
MOVE $t0, $0
MOVE $t1, $0
ADDI $t3, 320
MOVE $a0, $t3
OR $a0, $t3, $0
ADDI $t4, 200
LUI $t2, 0xFFFF
ADDI $t2, $t2, 0x0600
while:
BEQ $t0, $t4, endwhile
MOVE $t1, $0
while2:
BEQ $t1, $t3, endwhile2
SB $t0, 0($t2)
ADDI $t1, $t1, 1
ADDI $t2, $t2, 1
J while2
endwhile2:
ADDI $t0, $t0, 1
J while
endwhile:

ADDI $sp, $sp, -4
SW $t1, -4($sp)
