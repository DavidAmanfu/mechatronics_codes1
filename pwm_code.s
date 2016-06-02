.equ DDRD, 0x0A ;#assigns var name DDRD to its address, i/o port d data direction register
.equ TCCR0A, 0x24 ;#same for TCCR0A, time counter control register
.equ TCCR0B, 0x25 ;#same for TCCR0B, time counter control register b, see page 95/660
.equ OCR0A, 0x27 ; #same for time counter output compare register A
.equ ORC0B, 0x28; #same for time counter output compare register A
.equ ACOM, 0x002E
.org 0x0000 ; store this bit of code at location 0
rjmp init ;jump to init

.org 0x0032 ; First memory address after the interrupt vector table
            ; Some random code to keep the CPU busy.
start: 
    ldi r16,254 ;load value 254 into register 16

main: ;main loop set
	nop ;wait
	dec r16	;decrement the register
	rjmp main ;loop gang
     # brbc (sreg flag bit #),stop area
init:
    ldi r16,0b10100011 ;non-obvious p104 COmpare match output a mode
                        ;first two bits are 10, clear OC0A on compare match
    out TCCR0A,r16
    ldi r16,0b00000001 ;not-obvious only clock select,clk i/o no prescaling
    out TCCR0B,r16
    ldi r16,127 ;obvious but why 127
    out OCR0A,r16
    ldi r16, 200
    out ORC0B, r16
    sbi DDRD,5
    sbi DDRD,6 ;toggles bit 6 of port d
    rjmp start