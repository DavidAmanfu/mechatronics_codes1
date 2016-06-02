; See chapter 15 of the datasheet. In particular, see the following
; tables: 15-1, 15-3, 15-4, and 15-8.
; Also pay attention to section 15.7.3 and 15.7.4 to understand the
; underlying logic of PWM.

.equ DDRD, 0x0A
.equ TCCR0A, 0x24
.equ TCCR0B, 0x25
.equ OCR0A, 0x27

.org 0x0000
rjmp init

.org 0x0032 ; First memory address after the interrupt vector table
            ; Some random code to keep the CPU busy.
	ldi r16,254
main:
	nop
	dec r16	
	rjmp main

init:
    ldi r16,0b10000011
    out TCCR0A,r16
    ldi r16,0b00000001
    out TCCR0B,r16
    ldi r16,127
    out OCR0A,r16
    sbi DDRD,6
    ret
