/*
uint32_t pgcd(uint32_t a, uint32_t b)
{
    while (a != b) {
        if (a < b) {
            b = b - a;
        } else {
            a = a - b;
        }
    }
    return a;
}

*/
    .set noreorder
    .text
    /* uint32_t pgcd(uint32_t a, uint32_t b) */
/* Contexte :
    Fonction feuille pas d'espace reservé dans la pile
    a : $a0
    b : $a1
*/
addi $v0, $0, 5
SYSCALL
move $a0, $v0
addi $v0, $0, 5
SYSCALL
move $a1, $v0
jal pgcd
move $a0, $v0
addi $v0, $0, 1
SYSCALL
j end


pgcd:
    /* while (a != b) {  */
while:
    beq $a0, $a1, fin /*Saut à fin si a==b*/
    /* if (a < b) {  */
    slt $t0, $a0, $a1 /*Le registre t2 est utilisé pour stocker le résultat du
test a<b*/
    beq $t0, $zero, else /*$zero est l'alias de $0 */
    /* b = b - a;  */
    sub $a1, $a1, $a0
    j fin_if
    /* } else {  */
else:
    /* a = a - b;  */
    sub $a0, $a0, $a1
    /*  }  */
fin_if:
    /*  }  */
    j while
    /* return a; */
fin:
    /* Par convention, le registre v0 (soit $2) contient la valeur de retour.*/
    addu $v0, $a0, $zero
    /* Par convention encore, l'adresse de retour dans la fonction appelante est
     * stockée dans le registre ra (soit $31) */
    jr $ra
end:
