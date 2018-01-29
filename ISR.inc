ISR_INIT    
    BSF	    INTCON, GIE		    ;Enable general interrupts
    
    RETURN

ISR
    BCF	    STATUS, RP0		    ;BANK 0
    BTFSC   INTCON, T0IF	    ;Handle Timer0 interrupts
    CALL    TMR_TRIG
    
    RETFIE