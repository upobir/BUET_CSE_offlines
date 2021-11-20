.MODEL SMALL


.STACK 100H


.DATA
    CR EQU 0DH
    LF EQU 0AH
         
    ; INPUT 1
    MAT1 DW 2 DUP (?)
         DW 2 DUP (?)
    
    ; INPUT 2     
    MAT2 DW 2 DUP (?)
         DW 2 DUP (?)
    
    ; OUTPUT
    MAT3 DW 2 DUP (?)
         DW 2 DUP (?) 
         
    ; MESSAGES
    MSG1 DB 'ENTER FIRST MATRIX: $'
    MSG2 DB 'ENTER SECOND MATRIX: $'
    MSG3 DB 'OUTPUT MATRIX: $'

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV   AX, @DATA
    MOV   DS, AX
          
    ; PROMPT 1
    LEA   DX, MSG1
    MOV   AH, 9
    INT   21H   
    CALL  CRLF
    
    ; READ MATRIX 1
    LEA   AX, MAT1
    PUSH  AX
    CALL  READ_MATRIX     
    CALL  CRLF
    
    ; PROMPT 2
    LEA   DX, MSG2
    MOV   AH, 9
    INT   21H 
    CALL  CRLF
    
    ; READ MATRIX 2
    LEA   AX, MAT2
    PUSH  AX
    CALL  READ_MATRIX  
    CALL  CRLF
    
    ; ADDITION LOOP
    MOV   CX, 4
    MOV   BX, 0
    
    MAT_LOOP:           
        MOV   AX, [MAT1+BX]
        ADD   AX, [MAT2+BX]
        MOV   [MAT3+BX], AX       
        ADD   BX, 2
        LOOP  MAT_LOOP 
        
    ; PROMPT 3
    LEA   DX, MSG3
    MOV   AH, 9
    INT   21H  
    CALL  CRLF
    
    ; OUTPUT MATRIX 3
    LEA   AX, MAT3
    PUSH  AX
    CALL  PRINT_MATRIX
    CALL  CRLF
    
    
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP  
             
; READ 2*2 MATRIX             
READ_MATRIX PROC
    ; SAVE VARIABLES
    PUSH  BP
    MOV   BP, SP         
    PUSH  BX
    PUSH  AX
            
    MOV   BX, [BP+4]
    
    CALL  READ_NUM
    MOV   [BX], AX
    
    CALL  READ_NUM
    MOV   [BX+2], AX
    
    CALL  CRLF
    
    CALL  READ_NUM
    MOV   [BX+4], AX
    
    CALL  READ_NUM
    MOV   [BX+6], AX
    
    CALL CRLF
                
    ; RESTORE VARIABLES
    POP   AX
    POP   BX
    POP   BP   
    
    RET   2
READ_MATRIX ENDP     


; PRINT 2*2 MATRIX
PRINT_MATRIX PROC
    
    ; SAVE VARIABLES   
    PUSH  BP
    MOV   BP, SP  
    PUSH  BX
    
    MOV   BX, [BP+4]
    
    PUSH  [BX]
    CALL  PRINT_NUM
    
    MOV   DL, ' '
    MOV   AH, 2
    INT   21H
    
    PUSH  [BX+2]
    CALL  PRINT_NUM
    
    CALL  CRLF
    
    PUSH  [BX+4]
    CALL  PRINT_NUM
    
    MOV   DL, ' '
    MOV   AH, 2
    INT   21H
    
    PUSH  [BX+6]
    CALL  PRINT_NUM
    
    CALL  CRLF
    
    ; RESTORE VARIABLES
    POP   BX
    POP   BP
    
    RET 2
PRINT_MATRIX ENDP
             
   
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
