.data
.balign 4	

@ String used for printg and scanf.
FirstOp: 	.asciz  "Give me the first operand: "
SecondOp:	.asciz  "Give me the second operand: "
Output: 	.asciz "\nThe result of %d + %d = %d\n"
format: 	.asciz  "%d"

@ Integers used for calculations.
a:    	.int    0
b:    	.int    0
d:    	.int 	0 

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
@ Sum():
@ Sum two numbers and put it on d.
sum:
	push {ip, lr}	@ push return address + dummy register
	
	add r1, r5, r6
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
    ldr	r2, =b           	@ r2 <- b
    ldr	r2, [r2]
    ldr	r3, =d           	@ r3 <- d
    ldr	r3, [r3] 
    bl 	printf           	@ print string and pass params into r1, r2, and r3
	
	pop {ip, pc} 	@ return to previous address.
	
@ ---------------------------------------



main:   push	{ip, lr}		@ push return address + dummy register for alignment
        
        bl	getFirstOperand
        bl	getSecondOperand
		
		bl sum
		bl printOutput

        pop     {ip, pc}        @ pop return address into pc
