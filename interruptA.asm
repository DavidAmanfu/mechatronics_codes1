;
.equ SPH , 0x3E
.equ SPL , 0x3D
.equ RAMEND , 0x08FF ; Page 19

;power reduction bit
.equ PRR , 0x64
.equ PRADC, 0

;registers
.equ DDRB , 0x04
.equ PORTB , 0x05
.equ PINB , 0x03
.equ DDRC , 0x07
.equ PORTC , 0x08
.equ PINC , 0x06
.equ PIND , 0x09
.equ DDRD , 0x0A
.equ PORTD , 0x0B


.equ SREG , 0x3F ; Page 10


;output compare register 1A/B for TCNT1
.equ OC1AH , 0x89 ; to write to oc1a, (out)put this first
.equ OC1AL , 0x88 ; to read oc1a, (out)put this value first
.equ OC1BH , 0x8B
.equ OC1BL , 0x8A
.equ TCCR1A , 0x80
.equ TCCR1B , 0x81
;port d register stuff
.equ B5 , 5
.equ INT0 , 2
.equ INT1 , 3


;analog comparator stuff
.equ ACSR , 0x30 ; analog comparator status register
.equ ACD , 7 ; DISABLE analog comparator. we want this at 0
.equ ACBG , 6 ; positive input is either AIN0 or this 1.1V bandgap voltage. we want this set to 0.
.equ ACIE , 3 ; allows analog comparator interrupts
.equ ACIC , 2  ; Input Capture enable: requireS TCNT1 Input Capture ;noise canceler and edge select features.
.equ ACIS1 , 1 ;comparator interrupt mode select. set both to 0 for toggle mode
.equ ACIS0 , 0 ; we want to trigger a comparator interrupt when the output value is toggled
.equ DIDR1 , 0x7F ;digital input disable register 1
.equ AIN1D , 1 ;setting this bit disables AIN1, AIN1 is replaced by ADC2 - the sensor input
.equ AIN0D , 0

;ANALOG TO DIGITAL THINGS
.equ ADCSRA ,0x7A
.equ ADCSRB , 0x7B
.equ ADMUX , 0x7C
.equ DIDR0 , 0x7E
.equ ADCH , 0x79 ;converted input signals, then we rewrite to the OC1A/B a new value on conversion completion
.equ ADCL , 0x78 ;read this  first

;ADCSRA bits
.equ ADEN , 7 ; enable ADC = 1
.equ ADSC , 6 ; set this bit
.equ ADATE , 5 ; auto trigger ADC with source from ADTS in ADCSRB
.equ ADIE , 3; enable interrupts
.equ ADPS2 , 2 ;clock prescaler bits
.equ ADPS1 , 1
.equ ADPS0 , 0
;ADCSRB bits
.equ ACME , 6
.equ ADTS2 , 2 ; sets the trigger source. a positive edge from this source's interrupt flag will reset the prescaler and start a conversion. use the free running mode
.equ ADTS1 , 1
.equ ADTS0 , 0



;TCNT1 INPUT CAPTURE interrupt
.equ ICR1H , 0x87 ;ICP input is port b pin 0
.equ ICR1L , 0x86 ; also we can look at the analog comparator output for TCNT 1
.equ TIMSK1 , 0x6F
.equ TIFR1, 0x16
;TCNT1 Interrupt mask
.equ ICIE1 , 5
.equ OCIE1B , 2
.equ OCIE1A , 1 
.equ TOIE1 , 0
;TIFR1 Flag register
.equ OCF1B , 2
.equ OCF1A , 1


; Interrupt vectors
.org 0x0000
	rjmp reset
.org 0x0014
	rjmp TIM1_CAPT
.org 0x002E
	rjmp ACOMP
.org 0x002A
	rjmp ADC
.org 0x0016
	rjmp TIM1_COMPA
.org 0x0018
	rjmp TIM1_COMPB

.org 0x0068
reset:
	cli ; disable all interrupts while setting things up

	; Setup the stack. Make sure that it points to a sensible memory
	; address. In this case, the last memory locations of the SRAM
	ldi r16 , hi8(RAMEND)
	out SPH , r16
	ldi r16 , lo8(RAMEND)
	out SPL , r16

	
	rcall tccrsetup
	rcall adcsetup
	rcall acompsetup
	rcall TCNTInterruptSetup

	
	sei ; Globally enable interrupts
tccrsetup:
	lds r16, 0b10100011 ;non inverting for OC1A/B, WGM mode 7, 10 bit fast pwm should this be changed
	out TCCR1A , r16
	ldi r16, 0b00001001 ;ICP noise canceler, 264 prescaler, edge selector set to zero/falling edge triggers event.
	out TCCR1B , r16
	ldi r23 , 0x01
	ldi r24 , 0xFF
	out OCR1AH , 0x01 ;initial PWM set
	out OCR1AL ,r24
	ldi r25 , 0x01
	ldi r26 , 0xFF
	out OCR1BH ,r25 ;initial pwm set 
	out OCR1BL ,r26
	sbi DDRB, 1
	sbi DDRB, 2
	ret
acompsetup: 
	ldi r16, (1<<7)|(1<<6)
	out PORTD, r16

	ldi r16, (0<<ACD)|(0<<ACBG)|(1<<ACIE)|(0<<ACIC)|(0<<ACIS1)|(0<<ACIS0);tcnt input capture function interrupts triggered, analog comp interrupt disabled, ACIS10 dont matter
	out ACSR , r16
	ldi r16 ,(0<<AIN1D)|(1<<AIN0D); we do use the digital input in ain1
	out DIDR1 , r16
	ret
	
adcsetup: 
	ldi r16 , (1<<2)
	out PORTC, r16
	; setup adc interrupts
	ldi r16 , 0b00000010 ; AREF=5V for conversion, right adjusted, A2 connected to the ADC
	out ADMUX , r16
	ldi r16, (1<<ADATE)|(1<<ADIE)|(0<<ADPS2)|(0<<ADPS1)|(1<<ADPS0); 
	out ADCSRA , r16
	ldi r16 , (1<<ACME)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0) ;ACME needs to be written to 1.now in free running mode
	out ADCSRB, r16
	ldi r16 , 0b00111011 ;disables all the other adc values.
	out DIDR0, r16
	ret

TCNTInterruptSetup:
	;setup TCNT1 Input Capture interrupts
	ldi r16 , (0<<ICIE1)|(1<<OCIE1B)|(1<<OCIE1A)|(0<<TOIE1); oca,b and overflow
	ret


main:
	rjmp main ; The main loop does nothing right now.

; All global interrupts are disabled when entering an interrupt service
; routine (ISR). The RETI command to exit the ISR will re-enable global
; interrupts.

ACOMP: ;called when Analog comparatoris toggled which means we've hit our target value
	in r16 , SREG ; Save the current status register to stack
	push r16
	ldi r16, (1<<ACD)|(0<<ACIE)|;tcnt input capture function interrupts triggered, analog comp interrupt disabled,
	out ACSR , r16
	ldi r16, (1<<ADEN)|(1<<ADSC)
	out ADCSRA , r16
	pop r16
	out SREG , r16 ; Recover the status register from the stack
	reti;
TIM1_CAPT: ;disabled
	in r16 , SREG ; Save the current status register to stack
	push r16

	;do something if the TCNT interrupt is set (change OCR1)
	;maybe determine magnitude of necessary change
	nop
	pop r16
	out SREG , r16 ; Recover the status register from the stack
	reti
TIM1_COMPA:
	in r16 , SREG ; Save the current status register to stack
	push r16

	;do something if the TCNT interrupt is set (change OCR1)
	;maybe determine magnitude of necessary change
	in r18, ADCL
	in r19, ADCH
	cp r18, r20; compare current digital value to a reference value
	cpc r19, r21;
	brne changeocr1A
	pop r16
	out SREG , r16 ; Recover the status register from the stack
	reti
TIM1_COMPB:
	in r16 , SREG ; Save the current status register to stack
	push r16

	in r29, ADCL
	in r30, ADCH
	cp r29, r20; compare current digital value to a reference value
	cpc r30, r21;
	brne changeocr1B
	pop r16
	out SREG , r16 ; Recover the status register from the stack
	reti
ADC: ;called when adc conversion is complete
	in r16 , SREG ; Save the current status register to stack
	push r16

	in r20, ADCL
	in r21, ADCH
	
	pop r16
	out SREG , r16 ; Recover the status register from the stack
	reti;
changeocr1B:
	sub r29,r20
	subc r30,r21
	add r26,r29
	adc r25, r30
	out OCR1BH ,r25 
	out OCR1BL ,r26
	ret
changeocr1A:
	sub r18,r20
	subc r19,r21
	add r24, r18
	adc r23, r19
	out OCR1AH ,r23 ; PWM update
	out OCR1AL ,r24
	ret