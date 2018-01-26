ISR_INIT    
    BSF	    INTCON, PEIE	    ;Enable peripheral interupts
    BSF	    INTCON, GIE		    ;Enable general interrupts
    
    RETURN

ISR
    BCF	    STATUS, RP0		    ;BANK 0
    
    ;BTFSC   PIR1, TMR1IF	    ;Clear Timer1 interrupt flag
    BTFSC   INTCON, T0IF
    CALL    DRV_NEXT_STEP
    
    RETFIE