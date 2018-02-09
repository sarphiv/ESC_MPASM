ISR_INIT    
    BSF	    INTCON, GIE		    ;Enable general interrupts
    
    RETURN

ISR
    BTFSC   INTCON, T0IF	    ;Handle Timer0 interrupts
    CALL    TMR_TRIG
    
    BTFSC   INTCON, RAIE	    ;Handle PORTA change interrupts
    CALL    BEMF_TRIG
    
    RETFIE