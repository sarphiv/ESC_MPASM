DRV_INIT
    CLRF    DRV_CUR_STEP	;Reset step counter
    BCF	    ESTAT, DRV_RUN	;Set driver state to off
    BSF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to high
    
    BCF	    STATUS, RP0		;BANK 0
    
    MOVLW   b'00000111'		;Turn comparators off
    IORWF   CMCON0		;
        
    MOVLW   STEPOFF		;Turn off all driver pins
    MOVWF   PORTC		;
    
    BSF	    STATUS, RP0		;BANK 1
    CLRF    TRISC		;Set driver control pins to output
    
    MOVLW   b'0000111'		;Make output pins digital
    ANDWF   ANSEL, F	    
    
    RETURN

DRV_START
    BSF	    ESTAT, DRV_RUN	;Set driver run to on
    
    RETURN
    
DRV_STOP
    BCF	    ESTAT, DRV_RUN	;Set driver run to off
    
    ;Turn off all driver pins
    BCF	    STATUS, RP0		;BANK 0
    MOVLW   STEPOFF   
    MOVWF   PORTC	   
    
    RETURN
    
DRV_LOAD_STEP 
    DRV_CHCK_STEP0
    ;Check if not step 0
    MOVLW   d'0'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP1
    ;else
    BCF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to HIGH
    RETLW   STEP0
    
    DRV_CHCK_STEP1
    ;Check if not step 1
    MOVLW   d'1'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP2
    ;else
    BSF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to LOW
    RETLW   STEP1

    DRV_CHCK_STEP2
    ;Check if not step 2
    MOVLW   d'2'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP3
    ;else
    BCF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to HIGH
    RETLW   STEP2
    
    DRV_CHCK_STEP3
    ;Check if not step 3
    MOVLW   d'3'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP4
    ;else
    BSF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to LOW
    RETLW   STEP3
    
    DRV_CHCK_STEP4
    ;Check if not step 4
    MOVLW   d'4'
    SUBWF   DRV_CUR_STEP, W
    BTFSS   STATUS, Z
    GOTO    DRV_CHCK_STEP5
    ;else
    BCF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to HIGH
    RETLW   STEP4
    
    DRV_CHCK_STEP5
    ;It is step 5 or above
    BSF	    ESTAT, DRV_NEXT_EMFVAL  ;Set next BEMF state to LOW
    RETLW   STEP5		    
    
DRV_NEXT_STEP
    ;Return if driver state is off
    BTFSS   ESTAT, DRV_RUN
    RETURN
    
    CALL    DRV_LOAD_STEP	;Load correct step pin outputs into working
    
    BCF	    STATUS, RP0		;BANK 0
    MOVWF   PORTC		;Update driver pins
    
    INCF    DRV_CUR_STEP, F	;Increment step counter
    
    ;Reset current step if over step 5
    MOVFW   DRV_CUR_STEP
    SUBLW   d'5'
    BTFSS   STATUS, C
    CLRF    DRV_CUR_STEP
    
    RETURN