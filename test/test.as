.set noreorder
.text
LUI $t0, 0x0FF2
ADDI $t1, $t0, 2
AND $t2, $t1, $t0
SRL $t3, $t2, 4
ADDI $4, $0, 3
ADDI $5, $0, 4
MULT $4, $5
MFLO $23
