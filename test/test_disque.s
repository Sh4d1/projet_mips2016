.set noreorder
.global _start
.text
_start:
LW $t3, hauteur
LW $t4, largeur

LW $t6, demih
LW $t7, demil

LI $t0, 0
LI $t1, 0

LA $a0, prompt
ADDI $v0, $0, 4
SYSCALL

ADDI $v0, $0, 5
SYSCALL

MULT $v0, $v0
MFLO $v0


while:
BEQ $t0, $t3, endwhile
    while2:
    BEQ $t1, $t4, endwhile2

        MULT $t0, $t4
        MFLO $t2
        ADD $t2, $t2, $t1
        LW $t5, fb
        ADD $t5, $t5, $t2

        SUB $a0, $t0, $t6
        MULT $a0, $a0
        MFLO $s0

        SUB $a1, $t1, $t7
        MULT $a1, $a1
        MFLO $s1

        ADD $s2, $s1, $s0
        ADD $s0, $0, $v0
        SUB $s0, $s2, $s0
        BGTZ $s0, endif
            ADD $s0, $0, 0xff
            SB $s0, 0($t5)
        endif:
        ADDI $t1, $t1, 1
        J while2
        endwhile2:
    ADDI $t1, $0, 0
    ADDI $t0, $t0, 1
    J while
endwhile:

.data
hauteur: .word 200
largeur: .word 320
demih: .word 100
demil: .word 160
fb: .word 0xFFFF0600
prompt: .asciiz "Entrez le rayon du cercle : "
