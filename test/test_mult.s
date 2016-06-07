/*
uint32_t mult(uint32_t x, uint32_t y);
{
    uint32_t res = 0;
    while (y != 0) {
        res = res + x;
        y--;
    }
    return res;
}
*/
    .set noreorder
    .text


addi $a0, $0, 5
addi $a1, $0, 6
jal mult
nop
move $a0, $v0
addi $v0, $0, 1
SYSCALL
j end
nop
    /* uint32_t mult(uint32_t x, uint32_t y); */
/*Contexte :
    Fonction feuille: pas d'espace réservé dans la pile pour d'éventuels paramètres de fonction appelée [np=0 voir notation du cours],
    ni pour sauvegarder $ra ni aucun registre [nr=0 voir notation du cours],
    par contre il y a une variable locale qui est placée  dans la pile [nv=1 voir notation du cours]
    x : $a0
    y : $a1
    res: à l'adresse SP+0
=> La fonction doit réserver dans la pile (np+nv+nr)*4 octets, ici 4 octets pour la variable locale res
=> Pile + 4
*/

mult:
    /* on reserve la place nécessaire dans la pile */
    addiu $sp,$sp,-4
    /* res = 0; */
    sw $zero,0($sp)
    /* while (y != 0) { */
while:
    beq $a1, $zero, fin_while
    nop
    /* res = res + x */
    lw $t0,0($sp)
    addu $t0, $t0, $a0
    sw $t0,0($sp)
    /* y-- */
    addiu $a1,$a1, -1
    /* } */
    j while
    nop
fin_while:
    /* return res; */
    lw $v0,0($sp)
    addiu $sp,$sp,4 /*On libère la pile */
    jr $ra
    nop

end:
/* On peut remarquer qu'il aurait été judicieux de matérialiser la variable locale dans un registre plutôt que dans la pile, mais ici on cherche à obtenir une traduction
systématique indépendante du nombre de variables locales, de l'usage qui serait fait des registres utilisées, etc... */
