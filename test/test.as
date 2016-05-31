.set noreorder
.text
add $10, $2, $3
addi $11, $2, 21
add $12, $3, $6
MULT $1, $2
SYSCALL
LW $21, 0($3)
