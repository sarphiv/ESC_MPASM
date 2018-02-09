TMR_INIT
    BCF	    ESTAT, TMR_CLS_LOOP	;Default to open loop mode
    CLRF    TMR_CYCLES1		;Reset timer counters
    CLRF    TMR_CYCLES2		;
    
    BSF	    STATUS, RP0		;Bank 1
    BSF	    INTCON, T0IE	;Enable Timer0 (8-bit) interrupts
    BCF	    OPTION_REG, T0CS	;Use internal clock cycle for Timer0
    BCF	    INTCON, T0IF	;Clear Timer0 interrupt flag
    
    CLRWDT			;Clear WDT and ;prescaler 
    MOVLW   b'11110000'		;Assign prescaler to Timer0 (bit 3) and clear prescaler
    ANDWF   OPTION_REG		;
    MOVLW   b'00000010'		;Set prescaler to 1:8 (b'00000010')
    IORWF   OPTION_REG		;

    RETURN
    
TMR_TRIG
   BTFSC    ESTAT, TMR_CLS_LOOP	;Check if closed loop mode enabled
   GOTO	    TMR_TRIG_END	;Closed loop mode enabled
   
   MOVF	    TMR_CYCLES2, W	;Check if above or equal to cycle limit
   SUBLW    d'1'		;Ensures that we check equality
   SUBLW    TMR_OPN_LP_LIM	;Subtract to check carry flag
   BTFSS    STATUS, C		;Check if above (and technically equal to too)
   BSF	    ESTAT, TMR_CLS_LOOP	;
   
   BTFSC    ESTAT, TMR_CLS_LOOP	;Check if closed loop mode enabled
   GOTO	    TMR_TRIG_END	;Closed loop mode enabled
   
   CALL	    DRV_NEXT_STEP	;Increment driver state
   
   DECFSZ   TMR_CYCLES1, F
   GOTO	    TMR_TRIG_END
   
   INCF	    TMR_CYCLES2, F	;Increment cycle
   
   TMR_TRIG_END
   BCF	    INTCON, T0IF	;Clear Timer0 interrupt flag
    
   RETURN