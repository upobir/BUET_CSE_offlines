.MODEL SMALL

;OFFLINE 2 QUESTION 2

.STACK 100H


.DATA
    CR    EQU 0DH
    LF    EQU 0AH
    PRMPT DB  'ENTER PASSWORD  : $' 
    OUT1  DB  '> VALID PASSWORD  :D $'
    OUT2  DB  '> INVALID PASSWORD  :( $'
          
    ;VALID INPUT RANGE
    FIRST EQU 021H
    LAST  EQU 07EH
    
    ;DIGIT RANGE
    D_F   EQU 030H
    D_L   EQU 039H
    
    ;UPPERCASE RANGE
    U_F   EQU 041H
    U_L   EQU 05AH 
    
    ;LOWERCASE RANGE
    L_F   EQU 61H
    L_L   EQU 7AH
    
    ;COUNT OF CHARS
    D_FND DB 0
    U_FND DB 0
    L_FND DB 0  
    
    ;CONSTANT
    FOUND EQU 1

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV AX, @DATA
    MOV DS, AX
        
    ;OUTPUT PROMPT
        LEA DX, PRMPT
        MOV AH, 9
        INT 21H
        
    ;READ INPUT
        LOOP_START:
            ;READ CHAR
            MOV AH, 1
            INT 21H
            
            ;CHECK IF INVALID
                ;RANGE CHECK
                CMP AL, FIRST
                JL  LOOP_END
                
                CMP AL, LAST
                JG  LOOP_END
            
            ;CHECK IF DIGIT
                ;RANGE CHECK
                CMP AL, D_F
                JL  END_IF1
                
                CMP AL, D_L
                JG  END_IF1
                
                ;IS DIGIT
                IS_DIGIT:
                    MOV D_FND, FOUND
                END_IF1:
                
                
             ;CHECK IF LOWERCASE
                ;RANGE CHECK
                CMP AL, L_F
                JL  END_IF2
                
                CMP AL, L_L
                JG  END_IF2
                
                ;IS LOWERCASE LETTER
                IS_LOWER:
                    MOV L_FND, FOUND
                    
                END_IF2: 
                
             ;CHECK IF UPPERCASE
                ;RANGE CHECK
                CMP AL, U_F
                JL  END_IF3
                
                CMP AL, U_L
                JG  END_IF3
                
                ;IS LOWERCASE LETTER
                IS_UPPER:
                    MOV U_FND, FOUND
                    
                END_IF3:
            
            ;LOOPING   
            JMP LOOP_START
        LOOP_END:
        
    ;OUTPUT
        ;CRLF
            MOV AH, 2
            MOV DL, CR
            INT 21H
            MOV DL, LF
            INT 21H
             
        ;CHECK IF ALL CONDITION MET
        
            ;CHECK DIGIT
            CMP D_FND, FOUND
            JNE NOT_VALID
            
            ;CHECK LOWERCASE
            CMP L_FND, FOUND
            JNE NOT_VALID
            
            ;CHECK UPPERCASE
            CMP U_FND, FOUND
            JNE NOT_VALID
         
         ;VALID PASSWORD   
         VALID:
            LEA DX, OUT1
            JMP END_IF4
            
         ;INVALID PASSWORD
         NOT_VALID:    
            LEA DX, OUT2
            JMP END_IF4
        
         ;OUTPUT MESSAGE 
         END_IF4:
            MOV AH, 9
            INT 21H
    
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN
