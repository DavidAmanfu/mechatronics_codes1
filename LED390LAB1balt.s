.equ PINB, 0x03
.equ PORTB, 0x05
.equ DDRB, 0x04
.equ TURNON, 0b0010000
.equ TURNOFF, 0b00000000
.equ COUNTMAX, 0xFF

ldi r31, COUNTMAX
ldi r16, TURNON
out DDRB, r16
out PORTB, r16
main: 
	sbi PINB,5 ;Replace with TURNOFF
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	sbi PINB,5 ; Replace with TURNOFF
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rjmp main
	
