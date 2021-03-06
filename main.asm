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
    ;All on (NEVER USE)	b'00000111'
    STEPOFF	    EQU	b'00111000' ;All off
    STEP0	    EQU	b'00101001' ;0+4 on
    STEP1	    EQU	b'00101100' ;2+4 on
    STEP2	    EQU	b'00110100' ;2+3 on
    STEP3	    EQU	b'00110010' ;1+3 on
    STEP4	    EQU	b'00011010' ;1+5 on
    STEP5	    EQU	b'00011001' ;0+5 on
   
    TMR_OPN_LP_LIM  EQU	d'254'	;254
	    
    ;ESTAT bits
    DRV_RUN	    EQU	d'0'
    DRV_NEXT_EMFVAL EQU d'1'
    TMR_CLS_LOOP    EQU	d'2'	    ;1 = BEMF control, 0 = Timer control
	    
    cblock  0x70
    DRV_CUR_STEP
    ESTAT
    TEMPA
    TEMPB
    TMR_CYCLES1
    TMR_CYCLES2
    endc

; ******* MODULES ***************************************************
#INCLUDE    "driver.inc"

#INCLUDE    "BEMF.inc"
#INCLUDE    "timer.inc"

#INCLUDE    "ISR.inc"
    
; ******* SETUP *****************************************************
INIT
    CALL    DRV_INIT
    
    CALL    BEMF_INIT
    CALL    TMR_INIT
    
    CALL    DRV_START
    CALL    ISR_INIT
    
    
    
LOOP
    ;implement some kind of delay before BEMF_NEXT (phase shift)
    ;speed control
    GOTO    LOOP		;Goto start of loop
    
END