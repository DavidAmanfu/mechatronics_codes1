.equ PINB, 0x03
.equ PORTB, 0x05
.equ DDRB, 0x04
.equ TURNON, 0b0010000
.equ TURNOFF, 0b00000000

ldi r16 , TURNON ; Setup pin 5 por t B to wr i t a bl e
out DDRB, r16
main: 
	sbi PORTB,5 ;Replace with TURNOFF
	sbi PINB,5 ; Replace with TURNOFF
	rjmp main
	
