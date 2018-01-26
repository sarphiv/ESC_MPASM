list        p=16F684    
INCLUDE "p16f684.inc"
;errorlevel  -302    ; no "register not in bank 0" warnings 
;errorlevel  -305    ; no "page or bank selection not needed for this device" messages
; ******* COMPILER configuration bits *****************************************************
; ext reset, no code or data protect, no brownout detect,
; no watchdog, power-up timer, int clock with I/O,
; no failsafe clock monitor, two-speed start-up disabled 
 __CONFIG _FCMEN_OFF & _IESO_OFF & _BOD_OFF & _CPD_OFF &  _CP_OFF & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT

; ******* Vectors ***************************************************	
    ORG	    0x0000	;Processor reset vector
    goto    INIT

    ORG	    0x0004	;Interrupt service routine vector
    goto    ISR
    
; ******* CONSTANTS *************************************************
    ;Step table
    ;All off		b'00111000'
    ;All on		    b'00000111'
    STEP0	    EQU	b'00101001' ;0+4 on
    STEP1	    EQU	b'00101100' ;2+4 on
    STEP2	    EQU	b'00110100' ;2+3 on
    STEP3	    EQU	b'00110010' ;1+3 on
    STEP4	    EQU	b'00011010' ;1+5 on
    STEP5	    EQU	b'00011001' ;0+5 on
    
    ;ESTAT bits
    DRV_STATE	    EQU	d'1'
	    
    cblock  0x70
    DRV_CUR_STEP
    ESTAT
    endc

; ******* MODULES ***************************************************
#INCLUDE    "driver.inc"
#INCLUDE    "timer.inc"    
#INCLUDE    "ISR.inc"
    
; ******* SETUP *****************************************************
INIT
    CALL    DRV_INIT
    CALL    DRV_START
    
    CALL    TMR_INIT
    
    CALL    ISR_INIT
    ;CALL    DRV_NEXT_STEP
LOOP
    
    ;create a timer to step through the steps
    ;call next step a few times and shit... not in here.. 
    ;but in the timer interrupt
    ;then hook up that motor and watch it burn
;    BCF	    STATUS, RP0
;    BSF	    PORTC, 0
    
    GOTO    LOOP		;Goto start of loop
    
END