.equ PORTB, 0x05 ; .equ sets a variable to a value. this is the address of PORTB, as defined in the summary data sheet
.equ DDRB, 0x04 ; this is the address of the DDRB, as defined in the summary data sheet
.equ TURNON, 0b00100000 ;0b refers to a binary number, and 00100000 means that bit 5 will be set to 1
.equ TURNOFF, 0b00000000 ;0b is binary, 00000000 sets bit 5 back to zero

main: 
	ldi r16 , TURNON ; ldi (load immediate) loads an 8bit constant directly to a register Setup pin 5 por t B to wr i t a bl e
	out DDRB, r16 ; out stores data from register Rr in the register file to I/O space. so r16 data goes to the DDRB
	ldi r16 , TURNON ; here its used to turn and keep the LED on.
	out PORTB, r16 ;so r16 data goes to portB
	rjmp main ;relative jump refers back to an address, and will repeat the instructions
