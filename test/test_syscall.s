.text

ADDI $v0, $0, 5
SYSCALL
MOVE $a0, $v0
ADDI $v0, $0, 1
SYSCALL

ADDI $sp, $sp, -50
MOVE $a0, $sp
ADDI $a1, $0, 50
ADDI $v0, $0, 8
SYSCALL
ADDI $v0, $0, 4
SYSCALL
ADDI $v0, $0, 10
SYSCALL
