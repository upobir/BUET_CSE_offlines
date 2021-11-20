.MODEL SMALL


.STACK 100H


.DATA
    CR    EQU 0DH
    LF    EQU 0AH
    
    MSG1  DB 'ENTER INDEX: $'
    MSG2  DB 'FIBONACCI NUMBERS: $'
    SEP   DB ', $'

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV   AX, @DATA
    MOV   DS, AX
    
    ; PROMPT 1
    LEA   DX, MSG1
    MOV   AH, 9
    INT   21H 
    
    ; READ NUMBER
    CALL  READ_NUM
    CALL  CRLF
    
    ; LOOP
    MOV   CX, AX
    MOV   BX, 0    
    JCXZ  EXIT
    FIB_PRINT_LOOP:        
        CMP   BX, 0
        
        JE   IS_ZERO
        IS_NON_ZERO:  
            LEA   DX, SEP
            MOV   AH, 9
            INT   21H
        IS_ZERO:
        
        PUSH  BX
        CALL  FIBONACCI
        
        PUSH  AX
        CALL  PRINT_NUM
    
        INC   BX
        LOOP  FIB_PRINT_LOOP
    
    ;DOS EXIT 
    EXIT:
    MOV   AH, 4CH
    INT   21H
MAIN ENDP   
         
         
; RETURNS NTH FIBONACCI ON AX
; N IS ON STACK
FIBONACCI PROC
    ; SAVE VARIABLES   
    PUSH  BP
    MOV   BP, SP 
    PUSH  BX
    PUSH  CX
    
    CMP   [BP+4], 1
    JG    NON_BASE_CASE
    
    BASE_CASE:
        MOV   AX, [BP+4]
        JMP   END_FIBONACCI
    
    NON_BASE_CASE:
    
    MOV   CX, [BP+4]
    
    DEC   CX
    PUSH  CX
    CALL  FIBONACCI
    MOV   BX, AX
    
    DEC   CX
    PUSH  CX
    CALL  FIBONACCI
    ADD   AX, BX       
    
    ; RESTORE VARIABLES
    END_FIBONACCI: 
    POP   CX
    POP   BX
    POP   BP
    RET 2
FIBONACCI ENDP

; READ A NUMBER THAT FITS IN WORD 
; (ALSO READS THE NEXT NON-NUMBER)
; NUMBER RETURNED IN: AX
READ_NUM PROC 
    ; SAVE REGISTERS
    PUSH  BX
    PUSH  CX
    PUSH  DX
    
    ; BX = NUMBER, CX = TEMP, DX = SIGN
    MOV   BX, 0
    MOV   AH, 1
    INT   21H
           
    CMP   AL, '-' 
    JE    NEGATIVE    

    ; CHECK SIGN    
    POSITIVE:
        PUSH  1
                    
        ; DIGIT CHECK                    
        CMP   AL, '0'
        JL    NUM_LOOP_END
        CMP   AL, '9'
        JG    NUM_LOOP_END
        
        MOV   BL, AL
        SUB   BX, '0'   
        
        JMP SIGN_END
    NEGATIVE:   ; NEGATIVE
        PUSH  -1
        
    SIGN_END:       
    
    ; LOOP
    NUM_LOOP:       
        MOV   AH, 1
        INT   21H
        
        ; DIGIT CHECK                    
        CMP   AL, '0'
        JL    NUM_LOOP_END
        CMP   AL, '9'
        JG    NUM_LOOP_END
        
        ; ADD DIGIT
        MOV   AH, 0       ; AH = 0
        SUB   AL, '0'     ; AL -= '0'
        MOV   CX, AX      ; CX = AX
        MOV   AX, 10      ; AX = 10
        IMUL  BX          ; AX *= BX
        ADD   AX, CX      ; AX += CX
        MOV   BX, AX      ; BX = AX
    
        JMP   NUM_LOOP
    NUM_LOOP_END:
    
    
    ; USING THE SIGN   
    MOV   AX, BX       
    POP   DX
    IMUL  DX 
          
    ; RESTORE REGISTERS
    POP   DX
    POP   CX
    POP   BX
    
    RET
READ_NUM ENDP   
             

; PRINT THE NUMBER ON TOP OF STACK
; THE NUMBER IS DISCARDED             
PRINT_NUM PROC                    
    ; SAVE VARIABLES
    PUSH  BP  
    MOV   BP, SP  
    PUSH  AX
    PUSH  BX
    PUSH  DX
    
    MOV   BX, [BP+4]
    
    CMP   BX, 0
    JE    ZERO
    JL    NEG_NUM
    JMP   NON_ZERO
    
    ZERO:
        MOV   AH, 2
        MOV   DL, '0'
        INT   21H
        
        JMP   END_PRINT
    NEG_NUM: 
        MOV   AH, 2
        MOV   DL, '-'
        INT   21H
                      
        NEG   BX
        
        JMP   NON_ZERO
    NON_ZERO:    
              
    PUSH  0
    MOV   AX, BX
    
    BUILD_LOOP:   
     
        CMP   AX, 0
        JE    PRINT_LOOP
        
        CWD   
        MOV   BX, 10
        DIV   BX
        
        ADD   DL, '0'  
        PUSH  DX
        
        
        JMP   BUILD_LOOP
        
    PRINT_LOOP:
        POP   DX
        CMP   DL, 0
        JE    END_PRINT
        
        MOV   AH, 2
        INT   21H
        
        JMP   PRINT_LOOP
    END_PRINT:     
    
    ; RESTORE VARIABLES
    POP   DX
    POP   BX
    POP   AX
    POP   BP
    RET   2
PRINT_NUM ENDP


; PRINT NEWLINE
; I.E. PRINT CR THEN LF
CRLF PROC
    ; PUSH VARIABLES
    PUSH  AX
    PUSH  DX  
   
    MOV   AH, 2 
    
    MOV   DL, CR
    INT   21H  
    
    MOV   DL, LF
    INT   21H
    
    ; RESTORE VARIABLES
    POP   DX
    POP   AX 
    
    RET
CRLF ENDP

END MAIN
