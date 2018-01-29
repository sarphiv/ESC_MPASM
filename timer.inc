TMR_INIT
    BSF	    STATUS, RP0		;Bank 1
    BSF	    INTCON, T0IE	;Enable Timer0 (8-bit) interrupts
    BCF	    OPTION_REG, T0CS	;Use internal clock cycle for Timer0
    BCF	    INTCON, T0IF	;Clear Timer0 interrupt flag
    
    CLRWDT			;Clear WDT and ;prescaler 
    MOVLW   b'11110000'		;Assign prescaler to Timer0 (bit 3) and clear prescaler
    ANDWF   OPTION_REG		;
    MOVLW   b'00000010'		;Set prescaler to 1:8
    IORWF   OPTION_REG		;

    RETURN
    
TMR_TRIG
   CALL	    DRV_NEXT_STEP	;Increment driver state
   
   BCF	    INTCON, T0IF	;Clear Timer0 interrupt flag
    
   RETURN