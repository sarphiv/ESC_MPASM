BEMF_INIT
    ;Set up BEMF pins
    BSF	    STATUS, RP0		;BANK 1
    MOVLW   b'00000111'		;Set BEMF pins to input
    IORWF   TRISA, F		;
    MOVLW   b'11111000'		;Make BEMF pins digital
    ANDWF   ANSEL, F		;
    
    ;Set up interrupt
    BSF	    INTCON, RAIE	;Enable PORTA interrupts
    BCF	    INTCON, RAIF	;Clear PORTA interrupt flag
    
    CALL    BEMF_UPDATE_INTPIN	;Set to interrupt on correct pin 
    
    RETURN
    
BEMF_UPDATE_INTPIN
    CALL    DRV_LOAD_STEP	;Load current step into working
    MOVWF   TEMPA		;Create copy of step
    MOVWF   TEMPB		;
    
    RRF	    TEMPB, F		;Move driver low side to high side position
    RRF	    TEMPB, F		;
    RRF	    TEMPB, F		;
    
    MOVLW   b'00000111'
    ANDWF   TEMPA		;Mask away all bits except driver ID
    ANDWF   TEMPB		;
    
    MOVF    TEMPA, W		;Move high side drivers to working
    XORWF   TEMPB, W		;Isolate the inactive driver path
    
    BSF	    STATUS, RP0		;BANK 1
    MOVWF   IOCA		;Update PORTA interrupt pin selector
    
    RETURN
    
BEMF_TRIG
    BTFSS   ESTAT, TMR_CLS_LOOP	;Check if closed loop mode disabled
    GOTO    BEMF_TRIG_END	;Closed loop mode disabled
    
    ;Figure out if state change is a correct state change
    BSF	    STATUS, RP0		;BANK 1
    MOVF    IOCA, W		;Move PORTA interrupt pin selector into working
    BCF	    STATUS, RP0		;BANK 0
    ANDWF   PORTA, W		;Isolate state of selected interrupt pin
    
    BEMF_CHCK_LOW		    ;Check for HIGH->LOW zero crossing
    BTFSS   STATUS, Z		    ;Check if PORTA pin is HIGH
    GOTO    BEMF_CHCK_HIGH	    ;PORTA pin is HIGH, goto high check
    BTFSC   ESTAT, DRV_NEXT_EMFVAL  ;Check if NEXT_EMFVAL is not LOW
    GOTO    BEMF_INCR_TRIG	    ;Mismatch between PORTA pin and val detected
    
    GOTO    BEMF_NEXT		    ;PORTA pin and NEXT_EMFVAL low
    
    BEMF_CHCK_HIGH		    ;Check for LOW->HIGH zero crossing
    BTFSC   ESTAT, DRV_NEXT_EMFVAL  ;Check if NEXT_EMFVAL is HIGH too
    GOTO    BEMF_NEXT		    ;LOW->HIGH zero crossing confirmed
    ;else
    BEMF_INCR_TRIG		;Incorrect trigger, mismatch detected
    BCF	    INTCON, RAIF	;Clear PORTA interrupt flag
    
    RETURN
    
    BEMF_NEXT			;Correct state change, update driver pins
    CALL    DRV_NEXT_STEP	;Update driver state
    CALL    BEMF_UPDATE_INTPIN	;Set to interrupt on correct pin
   
    BEMF_TRIG_END
    BCF	    INTCON, RAIF	;Clear PORTA interrupt flag
    
    RETURN