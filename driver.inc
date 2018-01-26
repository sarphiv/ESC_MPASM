DRV_INIT
    CLRF    DRV_CUR_STEP	;Reset step counter
    BCF	    ESTAT, DRV_STATE	;Set driver state to off
    
    BCF	    STATUS, RP0		;BANK 0
    ;Turn comparators off
    MOVLW   b'00000111'
    IORWF   CMCON0
    
    ;Turn off all driver pins
    MOVLW   b'00111000'	    
    MOVWF   PORTC	    
    
    BSF	    STATUS, RP0		;BANK 1
    CLRF    TRISC		;Set driver control pins to output
    
    MOVLW   b'0000111'		;Make output pins digital
    ANDWF   ANSEL, F	    
    
    RETURN

DRV_STOP
    BCF	    ESTAT, DRV_STATE	;Set driver state to off
    
    ;Turn off all driver pins
    BCF	    STATUS, RP0		;BANK 0
    MOVLW   b'00111000'	    
    MOVWF   PORTC	   
    
    RETURN

DRV_START
    BSF	    ESTAT, DRV_STATE	;Set driver state to on
    
    RETURN
    
    
DRV_NEXT_STEP
    ;Return if driver state is off
    BTFSS   ESTAT, DRV_STATE
    RETURN
    
;    BCF	    STATUS, RP0
;    MOVLW   h'FF'
;    XORWF   PORTC, F
;    
;    RETURN
    ;Load correct step pin outputs into working memory
    
DRV_CHCK_STEP0
    ;Check if step 0
    MOVLW   d'0'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP1
    ;else
    MOVLW   STEP0
    
    GOTO    DRV_NEXT_STEP_APPLY
    
DRV_CHCK_STEP1
    ;Check if step 1
    MOVLW   d'1'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP2
    ;else
    MOVLW   STEP1
    
    GOTO    DRV_NEXT_STEP_APPLY

DRV_CHCK_STEP2
    ;Check if step 2
    MOVLW   d'2'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP3
    ;else
    MOVLW   STEP2
    
    GOTO    DRV_NEXT_STEP_APPLY
    
DRV_CHCK_STEP3
    ;Check if step 3
    MOVLW   d'3'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP4
    ;else
    MOVLW   STEP3
    
    GOTO    DRV_NEXT_STEP_APPLY
    
DRV_CHCK_STEP4
    ;Check if step 4
    MOVLW   d'4'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP5
    ;else
    MOVLW   STEP4
    
    GOTO    DRV_NEXT_STEP_APPLY
    
DRV_CHCK_STEP5
    ;It is step 5
    MOVLW   STEP5
    
DRV_NEXT_STEP_APPLY
    BCF	    STATUS, RP0		;BANK 0
    MOVWF   PORTC		;Update driver pins
    
    INCF    DRV_CUR_STEP, F	;Increment step counter
    
    ;Reset current step if over step 5
    MOVFW   DRV_CUR_STEP
    SUBLW   d'5'
    BTFSS   STATUS, C
    CLRF    DRV_CUR_STEP
    
    
    ;BCF	    PIR1, TMR1IF	;Clear Timer1 interrupt flag
    BCF	    INTCON, T0IF
    
    RETURN