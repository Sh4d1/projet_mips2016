

.text
LI $v0, 4
LA $a0, chaine
SYSCALL

.data
chaine: .asciiz "Hello World\n"
