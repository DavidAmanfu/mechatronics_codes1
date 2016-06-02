.equ COUNTMAX, 0xFF
.equ PINB, 0x03
.equ DDRB, 0x04

sbi DDRB, 5
ldi r31, COUNTMAX
ldi r30, COUNTMAX
ldi r29, COUNTMAX
loop:
	sbi PINB,5
		main: 
			check:
				dec r30
			brne check
			sbi PINB, 5
			dec r29
		brne main
	dec r31
brne loop