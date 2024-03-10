.data
var1 : .word 11 22 33
.text
addi x30 x2 -2003
beq x0 x0 lab
and x4 x5 x6
ori x3 x3 15
lab :
and x12 x12 x12
auipc x12 100
lui x13 44
jal x1 lab
beq x0 x0 lab