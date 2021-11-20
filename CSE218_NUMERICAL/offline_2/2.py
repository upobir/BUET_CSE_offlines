# -*- coding: utf-8 -*-
"""
Created on Sat Jul 13 00:49:03 2019

@author: sabbir
"""

import numpy as np
eps = 1e-5
def eq(x, y):
    if(abs(x-y) < eps):
        return True
    return False

def prettyprint(M):
    s = '\n'.join([' '.join(["{:9.2f}".format(x) for x in line]) for line in M])
    print(s)

if __name__ == "__main__":
    np.set_printoptions(precision=2)
    np.set_printoptions(suppress=True)
    inputfile = []
    with open('in2.txt', 'rt') as file:
        for line in file:
            inputfile.append(line)
    
    optimize = [ float(x) for x in inputfile[0].split(' ') ]
    N = len(optimize)
    
    M = len(inputfile) - 1
    
    Table = np.zeros((M+1, N + M + 2))
    
    Table[0][0] = 1;
    for i in range(N):
        Table[0][i+1] = -optimize[i]
    
    for c in range(M):
        eqn = [ float(x) for x in inputfile[c+1].split(' ')]
        for i in range(N):
            Table[c+1][i+1] = eqn[i]
        Table[c+1][c+N+1] = 1
        Table[c+1][N+M+1] = eqn[N]
    
    itr = 0
    while(True):
        print("\niteration : " + str(itr))
        prettyprint(Table)
        # STEP 1
        
        ok = True
        for i in range(N+M+1):
            if(Table[0][i] < 0):
                ok = False
        if(ok):
            break
        
        # STEP 2
        keycol = 0
        for i in range(N+M+1):
            if(Table[0][i] < Table[0][keycol]):
                keycol = i
                
        # STEP 3
        keyrow = 0
        bestratio = 999999999
        for i in range(1, M+1):
            if(not eq(Table[i][keycol], 0)):
                ratio = Table[i][N+M+1] / Table[i][keycol]
                if(ratio > 0 and ratio < bestratio):
                    bestratio = ratio
                    keyrow = i
            
        # STEP 4
        Table[keyrow] = Table[keyrow] / Table[keyrow][keycol]
        
        for i in range(M+1):
            if(i != keyrow):
                Table[i] = Table[i] - Table[i][keycol]*Table[keyrow]
        
        # Go Back to Step 1
        
        itr = itr+1
        if(itr > 300):
            print('Max iteration limit passed, possibly no solution left\n')
            break;
        
    FinalValues = np.zeros(N+M+1)
    
    for i in range(N+M+1):
        foundone = -1
        for j in range(M+1):
            if(eq(Table[j][i], 0)):
                continue;
            elif (eq(Table[j][i] , 1)):
                if(foundone == -1):
                    foundone = j
                else:
                    foundone = -2
                    break
            else:
                foundone = -2
                break
        if(foundone > -1):
            FinalValues[i] = Table[foundone][N+M+1]
    
    print("\nOptimum Value:\n" + "{:.2f}".format(FinalValues[0]))
    print("\nVariables:")
    for i in range(N):
        print("{:.2f}".format(FinalValues[i+1]))
        
    print('\nSlack Variables:')
    for i in range(M):
        print("{:.2f}".format(FinalValues[i+N+1]))