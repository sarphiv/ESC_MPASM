TMR_INIT
    BSF	    STATUS, RP0		;Bank 1
    ;BSF	    PIE1, TMR1IE	;Enable Timer1 interrupts
    BSF	    INTCON, T0IE
    BCF	    OPTION_REG, T0CS
    BCF	    INTCON, T0IF
    
    CLRWDT  ;Clear WDT and ;prescaler 
    BANKSEL OPTION_REG ; 
    MOVLW b'11110000' ;Mask TMR0 select and 
    ANDWF OPTION_REG,W ; prescaler bits 
    IORLW b'00000010' ;Set prescale to 1:16 
    MOVWF OPTION_REG ;

    
    BCF	    STATUS, RP0		;Bank 0
    
    ;MOVLW   b'00000000'		;Set prescale to 1:8
    ;IORWF   T1CON, F
    
    ;BSF	    T1CON, TMR1ON	;Enable Timer1
    
    RETURN