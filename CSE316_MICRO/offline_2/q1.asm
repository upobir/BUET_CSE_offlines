.MODEL SMALL

;Offline 2 question 1

.STACK 100H


.DATA
    CR      EQU 0DH
    LF      EQU 0AH
    
    PRMPT1  DB 'ENTER FIRST DIGIT : $'
    PRMPT2  DB CR, LF, 'ENTER SECOND DIGIT : $'
    PRMPT3  DB CR, LF, 'ENTER THIRD DIGIT : $'
    OUT_EQ  DB 'ALL THE NUMBERS ARE EQUAL.$'
    OUT_NEQ DB 'SECOND HIGHEST NUMBER : $'
    
    X1      DB ?
    X2      DB ? 
    X3      DB ?
    

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV AX, @DATA
    MOV DS, AX    
    
        
    ;FIRST INPUT  
        ;PROMPT
        LEA DX, PRMPT1
        MOV AH, 9  
        INT 21H  
           
        ;READ
        MOV AH, 1
        INT 21H     
        SUB AL, '0' 
        MOV X1, AL
         
        
    ;SECOND INPUT  
        ;PROMPT
        LEA DX, PRMPT2
        MOV AH, 9  
        INT 21H  
           
        ;READ
        MOV AH, 1
        INT 21H     
        SUB AL, '0' 
        MOV X2, AL
        
    ;THIRD INPUT  
        ;PROMPT
        LEA DX, PRMPT3
        MOV AH, 9  
        INT 21H  
           
        ;READ
        MOV AH, 1
        INT 21H     
        SUB AL, '0' 
        MOV X3, AL
        
    
    ;BUBBLE SORT
        ;COMPARE AND SWAP X1, X2 
            ;READ TO REGISTER
            MOV AL, X1
            MOV BL, X2
        
            ;COMPARE AL, BL
            CMP AL, BL  
            JG  SWAP1:
            JMP END_IF1:
        
            ;SWAP IF AL > BL
            SWAP1:
                MOV CL, AL
                MOV AL, BL
                MOV BL, CL
            
            ;LOAD TO DATA
            END_IF1:  
            MOV X1, AL
            MOV X2, BL 
            
        ;COMPARE AND SWAP X2, X3 
            ;READ TO REGISTER
            MOV AL, X2
            MOV BL, X3
        
            ;COMPARE AL, BL
            CMP AL, BL  
            JG  SWAP2:
            JMP END_IF2:
        
            ;SWAP IF AL > BL
            SWAP2:
                MOV CL, AL
                MOV AL, BL
                MOV BL, CL
            
            ;LOAD TO DATA
            END_IF2:  
            MOV X2, AL
            MOV X3, BL
            
        ;COMPARE AND SWAP X1, X2 
            ;READ TO REGISTER
            MOV AL, X1
            MOV BL, X2
        
            ;COMPARE AL, BL
            CMP AL, BL  
            JG  SWAP3:
            JMP END_IF3:
        
            ;SWAP IF AL > BL
            SWAP3:
                MOV CL, AL
                MOV AL, BL
                MOV BL, CL
            
            ;LOAD TO DATA
            END_IF3:  
            MOV X1, AL
            MOV X2, BL
            
    ;CRLF 
    MOV AH, 2
    MOV DL, CR
    INT 21H
    MOV DL, LF
    INT 21H
        
    ;CHECK IF EQUAL
        ;READ SMALLEST AND LARGEST
        MOV AL, X1
        MOV BL, X3
        
        ;COMPARE X1 AND X3
        CMP AL, BL  
        JE  IF_EQ
        JMP IF_NEQ
        
        ;EQUAL CASE
        IF_EQ:
            LEA DX, OUT_EQ
            MOV AH, 9
            INT 21H
            
            JMP END_IF4
            
        ;UNEQUAL CASE
        IF_NEQ:   
            LEA DX, OUT_NEQ
            MOV AH, 9
            INT 21H
            
            MOV DL, X2
            ADD DL, '0'
            MOV AH, 2
            INT 21H    
            
            JMP END_IF4
            
        END_IF4:
        
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN