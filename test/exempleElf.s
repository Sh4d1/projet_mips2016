# Pour compiler le fichier objet:
#     mips-as exempleElf.s -o exempleElf.o
# Pour avoir un exe:
#     mips-ld exempleElf.o -o exempleElf

.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ADDI $3, $0, 12345  # Met la valeur 12345 dans le registre 3
LUI $4, 0x123
LUI $5, 123
JAL write           # Appel a la procedure "write" (met PC a la bonne adresse)
NOP                 # branch delay slot
B end               # On revient ici apres le "write"; branchement sur la fin
NOP

write:
SW $3, Z            # Ecrit le contenu du registre 3 a l'adresse Z
JR $31              # fin de la procedure "write", retour a l'appelant
NOP

end:                # The End!

.data
X: .byte 0xAB       # Declaration d'un octet initialise avec la valeur 0xAB
Y: .word Z          # Mot de 32 bits, en fait une reference a l'adresse Z
Z: .word 0x0        # Declaration d'un mot de 32 bits initialise a 0

.bss
.skip 9             # Reservation de 9 octets, non initialises
.lcomm hehe, 10
