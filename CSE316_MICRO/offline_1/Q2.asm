.MODEL SMALL


.STACK 100H


.DATA
    CR      EQU 0DH
    LF      EQU 0AH
    PROMPT  DB 'ENTER A UPPERCASE LETTER: $'
    OUT1    DB CR, LF, 'THE PREVIOUS LETTER IS: $'
    OUT2    DB CR, LF, "THE ONE'S COMPLEMENT IS: $"
    
    CHAR1   DB ?
    CHAR2   DB ?

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV AX, @DATA
    MOV DS, AX
    
    
    ;PROMPT
    LEA DX, PROMPT
    MOV AH, 9
    INT 21H
    
    ;READ CHAR
    MOV AH, 1
    INT 21H
    MOV CHAR1, AL
    
    ;PART1
        ;MESSAGE
        LEA DX, OUT1
        MOV AH, 9
        INT 21H
        
        ;COMPUTATION
        MOV AL, CHAR1
        DEC AL  
        ADD AL, 020H
        MOV CHAR2, AL
        
        ;OUTPUT
        MOV DL, CHAR2
        MOV AH, 2
        INT 21H
        
    ;PART2
        ;MESSAGE
        LEA DX, OUT2
        MOV AH, 9
        INT 21H
        
        ;COMPUTATION
        MOV AL, CHAR1
        XOR AL, 0FFH
        MOV CHAR2, AL
        
        ;OUTPUT
        MOV DL, CHAR2
        MOV AH, 2
        INT 21H
     
    
    
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN
