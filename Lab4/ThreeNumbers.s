
.data
.balign 4	

@ String used for printg and scanf.
FirstOp: 	.asciz  "Give me the first operand: "
Operation:	.asciz  "Give me the operation to be performed (+, -, *, /): "
SecondOp:	.asciz  "Give me the second operand: "
ThirdOp:	.asciz  "Give me the third operand: "
Output: 	.asciz  "\nThe result of %d %c %d"
Output2: 	.asciz  " %c %d = %d\n"
format: 	.asciz  "%d"
formatOp:	.asciz  "%c"

@ Integers used for calculations.
a:    	.int    0
b:    	.int    0
c:		.int	0
d:    	.int 	0 
op:		.int	0

@ ---------------------------------------
@     Code Section
@ ---------------------------------------
	
.text
.global main
.extern printf
.extern scanf

@ ---------------------------------------
@ getFirstOperand():
@ Get the first operand from the user, and put it on r5.
getFirstOperand:
	push 	{ip, lr}	@ push return address + dummy register

	ldr	r0, =FirstOp	@ Load address of FirstOp
	bl	printf			@ Print FirstOp

	ldr r0, =format		@ call scanf, and pass address of format
	ldr	r1, =a
	bl	scanf
	
	ldr r5, =a			@ get address of a into r5
    ldr r5, [r5]        @ get a into r5

    pop 	{ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ getOperator():
@ Get the operator from the user, and put it on r8.
getOperator:
	push 	{ip, lr}	@ push return address + dummy register

	ldr	r0, =Operation	@ Load address of Operation
	bl	printf			@ Print Operation

	ldr r0, =formatOp		@ call scanf, and pass address of formatOp
	ldr	r1, =op
	bl	scanf
	
	ldr r0, =formatOp		@ call scanf, and pass address of formatOp
	ldr	r1, =op
	bl	scanf
	
	ldr r8, =op			@ get address of a into r8
    ldr r8, [r8]        @ get a into r8

    pop 	{ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ getSecondOperand():
@ Get the first operand from the user, and put it on r6.
getSecondOperand:
	push 	{ip, lr}	@ push return address + dummy register

	ldr	r0, =SecondOp	@ Load address of SecondOp
	bl	printf			@ Print SecondOp

	ldr r0, =format		@ call scanf, and pass address of format
	ldr	r1, =b
	bl	scanf
	
	ldr r6, =b			@ get address of b into r6
    ldr r6, [r6]        @ get a into r6

    pop 	{ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ getThirdOperand():
@ Get the third operand from the user, and put it on r7.
getThirdOperand:
	push 	{ip, lr}	@ push return address + dummy register

	ldr	r0, =ThirdOp	@ Load address of SecondOp
	bl	printf			@ Print SecondOp

	ldr r0, =format		@ call scanf, and pass address of format
	ldr	r1, =c
	bl	scanf
	
	ldr r7, =c			@ get address of b into r7
    ldr r7, [r7]        @ get a into r7

    pop 	{ip, pc} 	@ return to previous address.
@ ---------------------------------------

@ ---------------------------------------
@ ChooseOperation():
chooseOperation:
	push {ip, lr}	@ push return address + dummy register
		
	cmp r8, #43
	bleq sum
	cmp r8, #45
	bleq subt
	cmp r8, #42
	bleq mult
	cmp r8, #47
	bleq divt
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ Sum():
@ Sum two numbers and put it on d.
sum:
	push {ip, lr}	@ push return address + dummy register
	
	add r1, r5, r6
	add r1, r1, r7
    ldr	r2, =d      @ get address of c into r2
    str	r1, [r2]    @ store r1 into d
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ Subt():
@ Subt two numbers and put it on d.
subt:
	push {ip, lr}	@ push return address + dummy register
	
	sub r1, r5, r6
	sub r1, r1, r7
    ldr	r2, =d      @ get address of c into r2
    str	r1, [r2]    @ store r1 into d
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ mult():
@ mult two numbers and put it on d.
mult:
	push {ip, lr}	@ push return address + dummy register
	
	mul r2, r5, r6
	mul r1, r2, r7
    ldr	r2, =d      @ get address of c into r2
    str	r1, [r2]    @ store r1 into d
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------

@ ---------------------------------------
@ divt():
@ divt two numbers and put it on d.
divt:
	push {ip, lr}	@ push return address + dummy register
	
	sdiv r2, r5, r6
	sdiv r1, r2, r7
    ldr	r2, =d      @ get address of c into r2
    str	r1, [r2]    @ store r1 into d
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------



@ ---------------------------------------
@ PrintOutput():
@ print the result.
printOutput:	
	
	push {ip, lr}	@ push return address + dummy register
	
	ldr r0, =Output      	@ get address of string into r0
    ldr	r1, =a           	@ r1 <- a
    ldr	r1, [r1]
    ldr	r2, =op           	@ r2 <- op
    ldr	r2, [r2] 
    ldr	r3, =b           	@ r2 <- b
    ldr	r3, [r3] 
    bl 	printf           	@ print string and pass params into r1, r2, and r3
    
    ldr r0, =Output2      	@ get address of string into r0
    ldr	r1, =op          	@ r1 <- op
    ldr	r1, [r1]
    ldr	r2, =c           	@ r2 <- c
    ldr	r2, [r2] 
    ldr	r3, =d           	@ r2 <- d
    ldr	r3, [r3] 
    bl printf
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------



main:   push	{ip, lr}		@ push return address + dummy register for alignment
        
        bl	getFirstOperand
        bl	getSecondOperand
        bl	getThirdOperand
        bl	getOperator
		
		bl chooseOperation
		bl printOutput

        pop     {ip, pc}        @ pop return address into pc

