.MODEL SMALL


.STACK 100H


.DATA
    CR     EQU 0DH
    LF     EQU 0AH
                  
    ;PROMPTS
    PRMPT1 DB "ENTER FIRST OPERAND: $"
    PRMPT2 DB "ENTER OPERATOR: $"
    PRMPT3 DB "ENTER SECOND OPERAND: $" 
    INVLD  DB "WRONG OPERATOR :( $"
    
    ; VARIABLES
    X1    DW ?
    X2    DW ?  
    OP    DB ? 
    
    Z     DW ? 
    
    ; TEMPORARY VARIABLES
    SGN   DW ?

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV AX, @DATA
    MOV DS, AX                   
    
    ; INPUT          
        ; FIRST OPERAND
        LEA DX, PRMPT1
        MOV AH, 9     
        INT 21H     
        
        CALL READ_NUM
        MOV X1, AX
        
        ; CRLF
        CALL CRLF
        
        
        ; OPERATOR
        LEA DX, PRMPT2
        MOV AH, 9
        INT 21H
           
        CALL READ_OP
        MOV OP, AL  
        
        CMP OP, 0
        JE  END_PROGRAM
        
        ; CRLF
        CALL CRLF
          
        ; SECOND OPERAND
        LEA DX, PRMPT3
        MOV AH, 9
        INT 21H      
        
        CALL READ_NUM
        MOV X2, AX
        
    ; PROCESSSSING   
        MOV AX, X1
        MOV BX, X2    
        MOV CL, OP
        
        CALL OP_PROCESS
        
        MOV Z, AX 
        
    ;OUTPUT  
        CALL CRLF   
        CALL CRLF
                 
        ; [X1]
        MOV AX, X1
        CALL PRINT_NUM
        
        ; SPACE
        MOV AH, 2
        MOV DL, ' '
        INT 21H
        
        ; [OP]
        MOV AL, OP
        CALL PRINT_OP
              
        ; SPACE
        MOV AH, 2
        MOV DL, ' '
        INT 21H
        
        ; [X2]
        MOV AX, X2
        CALL PRINT_NUM
        
        ; SPACE
        MOV AH, 2
        MOV DL, ' '
        INT 21H

        ; =               
        MOV AH, 2
        MOV DL, '='
        INT 21H
           
        ; SPACE
        MOV AH, 2
        MOV DL, ' '
        INT 21H
               
        ; [Z]
        MOV AX, Z
        CALL PRINT_NUM
    
    
    END_PROGRAM:
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP


; READS A NUMBER 
; RETURNS NUMBER IN AX                 
READ_NUM PROC 
    MOV BX, 0
    
    MOV AH, 1
    INT 21H
           
    CMP AL, '-' 
    JE  NEGATIVE    

    ; CHECK SIGN    
    POSITIVE:    ; POSITIVE
        MOV SGN, 1
                    
        ; DIGIT CHECK                    
        CMP AL, '0'
        JL  SIGN_END
        CMP AL, '9'
        JG  SIGN_END
        
        
        MOV BL, AL
        SUB BX, '0'   
        
        JMP SIGN_END
    NEGATIVE:   ; NEGATIVE
        MOV SGN, -1
             
        JMP SIGN_END
    SIGN_END:       
    
    ; LOOP
    NUM_LOOP:       
        
        MOV AH, 1
        INT 21H
        
        ; ENTER CHECK
        CMP AL, CR   
        JE  NUM_LOOP_END
        
        ; DIGIT CHECK                    
        CMP AL, '0'
        JL  NUM_LOOP
        CMP AL, '9'
        JG  NUM_LOOP
        
        ; ADD DIGIT
        MOV AH, 0       ; AH = 0
        SUB AL, '0'     ; AL -= '0'
        MOV CX, AX      ; CX = AX
        MOV AX, 10      ; AX = 10
        MUL BX          ; AX *= BX
        ADD AX, CX      ; AX += CX
        MOV BX, AX      ; BX = AX
    
        JMP NUM_LOOP
    NUM_LOOP_END:
    
    
    ; USING THE SIGN
    MOV AX, BX
    IMUL SGN
           
    RET
READ_NUM ENDP  

; READS OPERATOR      
; RETURNS OPERATOR IN AL            
READ_OP PROC 
    
    MOV AH, 1
    INT 21H
    
    ; CHECK FOR 'q'
    CMP AL, 'q'
    JE  BAD_OP
    
    ; CHECK FOR GOOD OPERATORS
    CMP AL, '+'
    JE  GOOD_OP
    CMP AL, '-'
    JE  GOOD_OP
    CMP AL, '*'
    JE  GOOD_OP
    CMP AL, '/'
    JE  GOOD_OP
    
    ; BAD OPERATOR 
    CALL CRLF
    LEA DX, INVLD
    MOV AH, 9
    INT 21H
                  
    JMP BAD_OP
    
    BAD_OP:
        MOV AX, 0
        JMP OP_END
    GOOD_OP:
        MOV AH, 0
        JMP OP_END
    OP_END:
    
    RET
READ_OP ENDP  
       
; PROCESSES OPERATOR 
; INPUT NUMBER IN AX, BX, OPERATOR IN CL
; RETURNS RESULT IN AX      
OP_PROCESS PROC   
    
    CMP CL, '+'
    JE  ADD_CASE
    CMP CL, '-'
    JE  SUB_CASE
    CMP CL, '*'
    JE  MUL_CASE
    CMP CL, '/'
    JE  DIV_CASE 
              
    ; SWITCH CASE
    ADD_CASE:   
        ADD AX, BX
        
        JMP OP_CASE_END
    SUB_CASE:          
        SUB AX, BX
        
        JMP OP_CASE_END
    MUL_CASE:          
        IMUL BX
        
        JMP OP_CASE_END
    DIV_CASE:           
        CWD
        IDIV BX
        
        JMP OP_CASE_END
    OP_CASE_END:   
    
    RET
OP_PROCESS ENDP
                
; OUTPUTS CR, LF
CRLF PROC
    MOV AH, 2
    MOV DL, CR
    INT 21H
    MOV DL, LF
    INT 21H
    
    RET
CRLF ENDP 
              
; OUTPUTS [NUMBER]
; INPUT IN AX              
PRINT_NUM PROC 
    MOV BX, AX
     
    MOV AH, 2
    MOV DL, '['
    INT 21H
    
    CMP BX, 0
    JE  ZERO
    JL  NEG_NUM
    JMP NON_ZERO
    
    ZERO:
        MOV AH, 2
        MOV DL, '0'
        INT 21H
        
        JMP END_PRINT
    NEG_NUM: 
        MOV AH, 2
        MOV DL, '-'
        INT 21H
                      
        NEG BX
        
        JMP NON_ZERO
    NON_ZERO:    
              
    PUSH 0
    MOV AX, BX
            
    BUILD_LOOP:   
     
        CMP AX, 0
        JE  PRINT_LOOP
        
        CWD   
        MOV BX, 10
        DIV BX
        
        ADD DL, '0'  
        PUSH DX
        
        
        JMP BUILD_LOOP
        
    PRINT_LOOP:
        POP DX
        CMP DL, 0
        JE  END_PRINT
        
        MOV AH, 2
        INT 21H
        
        JMP PRINT_LOOP
    END_PRINT:
        
        
    MOV AH, 2
    MOV DL, ']'
    INT 21H
    
    RET
PRINT_NUM ENDP
             
; OUTPUTS [OPERATOR]
; INPUT OPERATOR IN AL             
PRINT_OP PROC  
    MOV BL, AL
    
    MOV AH, 2
    
    MOV DL, '['
    INT 21H
    
    MOV DL, BL
    INT 21H
    
    MOV DL, ']'
    INT 21H
    
    RET
PRINT_OP ENDP

END MAIN
                  
