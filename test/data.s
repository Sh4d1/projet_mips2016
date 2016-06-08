.set noreorder
.text
ADDI $3, $0, 5
J word

word:
ADDI $3, $0, 5

.data
X: .word 32
Y: .word X
