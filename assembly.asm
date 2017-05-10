
# assembly code
.text
j main
#main1
main1:addi $sp,$sp,100
lw $t1,0($fp)
jr $t1
#main
main:addi $sp,$sp,100
li $v0,10
syscall