.MODEL SMALL


.STACK 100H


.DATA
    CR      EQU 0DH
    LF      EQU 0AH
    
    PRMPT1  DB 'ENTER DIGIT X : $'
    PRMPT2  DB CR, LF, 'ENTER DIGIT Y : $'
    OUT1    DB CR, LF, 'X-2Y = $'
    OUT2    DB CR, LF, '25-(X+Y) = $'
    OUT3    DB CR, LF, '2X-3Y = $'
    OUT4    DB CR, LF, 'Y-X+1 = $'  
    
    X       DW ?
    Y       DW ? 
    Z       DW ?
    

.CODE

MAIN PROC
	;DATA SEGMENT INITIALIZATION
    MOV AX, @DATA
    MOV DS, AX    
    
        
    ;GIVE PROMPT FOR X
    LEA DX, PRMPT1
    MOV AH, 9  
    INT 21H  
           
    
    ;READ X
    MOV AH, 1
    INT 21H     
    SUB AL, '0' 
    MOV AH, 0
    MOV X, AX
    
    
    ;GIVE PROMPT FOR Y
    LEA DX, PRMPT2
    MOV AH, 9
    INT 21H

    
    ;READ Y
    MOV AH, 1
    INT 21H    
    SUB AL, '0'
    MOV AH, 0
    MOV Y, AX  
    
    ;NEWLINE
    MOV AH, 2
    MOV DL, CR
    INT 21H
    MOV DL, LF
    INT 21H
    
    
    ;PART 1 ->  X-2Y
    
        ;MESSAGE
        LEA DX, OUT1
        MOV AH, 9
        INT 21H
                 
        ;CALCULATION
        MOV AX, Y
        MOV BL, 2
        MUL BL 
        MOV BX, AX  ; BX = 2Y
        MOV AX, X
        SUB AX, BX  
        MOV Z, AX
    
        ;OUTPUT
        MOV DX, Z
        ADD DX, '0'
        MOV AH, 2
        INT 21H  
        
        
    ;PART 2 ->  25-(X+Y)
        
        ;MESSAGE
        LEA DX, OUT2
        MOV AH, 9
        INT 21H
        
        ;CALCULATION
        MOV AX, 25
        SUB AX, X
        SUB AX, Y
        MOV Z, AX
        
        ;OUTPUT
        MOV DX, Z
        ADD DX, '0'
        MOV AH, 2
        INT 21H      
        
        
    ;PART 3 -> 2X-3Y
        ;MESSAGE
        LEA DX, OUT3
        MOV AH, 9
        INT 21H
        
        ;CALCULATION
        MOV AX, Y
        MOV BL, 3
        MUL BL
        MOV BX, AX ; BX = 3Y
        MOV AX, X
        MOV CL, 2
        MUL CL    ; AX = 2X
        SUB AX, BX
        MOV Z, AX
        
        
        ;OUTPUT
        MOV DX, Z
        ADD DX, '0'
        MOV AH, 2
        INT 21H
        
    
    ;PART 4 -> Y-X+1
        ;MESSAGE
        LEA DX, OUT4
        MOV AH, 9
        INT 21H
        
        ;CALCULATION
        MOV AX, Y
        SUB AX, X
        ADD AX, 1
        MOV Z, AX
        
        ;OUTPUT
        MOV DX, Z
        ADD DX, '0'
        MOV AH, 2
        INT 21H
        
    
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN