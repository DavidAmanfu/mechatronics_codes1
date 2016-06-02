; Sample code to toggle the on-board LED on the Arduino in response to
; a rising edge on the INT0 pin. So, if you connect a variable voltage
; input to INT0, and switch the voltage between 0V and 5V (on-state),
; the LED will toggle every time that you go from 0V to 5V, but will not
; respond when you go back down to 0V.
; - Madhusudhan Venkadesan, 2016.

; Chapter 12 and section 12.4 are essential for learning interrupts

; Stack registers: Page 13 of the data sheet
.equ SPH , 0x3E
.equ SPL , 0x3D
.equ RAMEND , 0x08FF ; Page 19

; I/O registers: Ch. 14, and page 91 onwards for a summary
.equ DDRB , 0x04
.equ PORTB , 0x05
.equ PINB , 0x03
.equ DDRD , 0x0A
.equ PORTD , 0x0B
.equ SREG , 0x3F ; Page 10

; I/O bits: page 91 onwards for a summary
.equ B5 , 5
.equ INT0 , 2

; External interrupt registers: Page 71
.equ EICRA , 0x69
.equ EIMSK , 0x1D
.equ EIFR , 0x1C

; External interrupt bits
.equ ISC11 , 3
.equ ISC10 , 2
.equ ISC01 , 1
.equ ISC00 , 0

.equ INT1MSK , 1
.equ INT0MSK , 0

.equ INTF1 , 1
.equ INTF0 , 0

; Interrupt vectors
; As said on page 55, "Each Interrupt Vector occupies two instruction 
; words in ATmega168A/168PA and ATmega328/328P". Each instruction word
; is a 16-bit number, i.e. 2 memory blocks. Therefore, each interrupt
; vector occupied 4 memory blocks. Therefore, INT0, which sits at
; Program Address 0x0002 is really memory block 0x0004.
.org 0x0000
	rjmp reset
.org 0x0004
	rjmp runint0

; The main program. In case you are using all possible interrupts, the
; first possible memory location for storing the main program is 0x0068
; based on the interrupt vector table given on page 65.
.org 0x0068
reset:
	cli ; disable all interrupts while setting things up

	; Setup the stack. Make sure that it points to a sensible memory
	; address. In this case, the last memory locations of the SRAM
	ldi r16 , hi8(RAMEND)
	out SPH , r16
	ldi r16 , lo8(RAMEND)
	out SPL , r16

	rcall interruptsetup

	sei ; Globally enable interrupts

main:
	rjmp main ; The main loop does nothing right now.

interruptsetup:
	; Setup INT0 as an input. See the Arduino pin mapping
	ldi r16 , (1<<INT0)
	out DDRD , r16

	; Setup the on-board LED as a writable pin.
	ldi r16 , (1<<B5)
	out DDRB , r16
	; Turn off the on-board LED.
	cbi PORTB , B5

	; Setup external interrupts
	ldi r16 , (0<<ISC11)|(0<<ISC10)|(1<<ISC01)|(1<<ISC00) ; Rising edge
	sts EICRA , r16 ; page 71: cannot use LDI because outside its range
	ldi r16 , (0<<INT1MSK)|(1<<INT0MSK) ; Enable INT0, and disable INT1
	out EIMSK , r16 ; page 72

	ret

; This subroutine is called when a rising edge event occurs on INT0.
; All global interrupts are disabled when entering an interrupt service
; routine (ISR). The RETI command to exit the ISR will re-enable global
; interrupts.
runint0:
	in r16 , SREG ; Save the current status register to stack
	push r16

	sbi PINB , B5 ; Toggle the LED

	pop r16
	out SREG , r16 ; Recover the status register from the stack
	reti ; Exit the interrupt service routine (ISR)
