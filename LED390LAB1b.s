.equ PINB, 0x03
.equ DDRB, 0x04

sbi DDRB, 5
main: 
	sbi PINB,5 
	sbi PINB,5
	rjmp main
	
