# -*- coding: utf-8 -*-
"""
Created on Thu Jul 11 22:21:46 2019

@author: sabbir
"""
import numpy as np
eps = 1e-5

def eq(x, y):
    if(abs(x-y) < eps):
        return True
    return False

def LUdecompose(arr, N):
    L = np.identity(N)
    U = np.copy(arr)
    for col in range(N):
        for row in range(col+1, N):
            if(eq(U[col][col], 0)):
                continue;
            L[row][col] = U[row][col]/U[col][col]
            U[row] -= U[col]*L[row][col]
    return L, U

def BackSubLower(L, B, N):
    X = np.zeros((N,1))
    for i in range(N):
        tmp = 0
        for j in range(i):
            tmp = tmp + L[i][j]*X[j][0]
        X[i][0] = (B[i][0] - tmp)/L[i][i]
    return X

def BackSubUpper(U, B, N):
    X = np.zeros((N, 1))
    for i in reversed(range(N)):
        tmp = 0
        for j in range(i+1, N):
            tmp = tmp + U[i][j]*X[j][0]
        X[i][0] = (B[i][0] - tmp)/U[i][i]
    return X
    

if __name__ == "__main__":
    inputfile = []
    with open('in1.txt', 'rt') as file:
        for line in file:
            inputfile.append(line)
    N = int(inputfile[0])
    
    
    arr = np.empty((N, N))
    for i in range(N):
        arr[i] = np.array([float(x) for x in inputfile[i+1].split(' ')])
    B = np.empty((N,1))
    for i in range(N):
        B[i][0] = float(inputfile[i+N+1])
    
    L, U = LUdecompose(arr, N)
    outfile = open('out1.txt', 'w')

    for row in range(N):
        outfile.write(' '.join("{:.4f}".format(x) for x in L[row]))
        outfile.write('\n')

    outfile.write('\n')

    for row in range(N):
        outfile.write(' '.join("{:.4f}".format(x) for x in U[row]))
        outfile.write('\n')

    outfile.write('\n')
    
    unique = True
    for row in range(N):
        zerorow = True
        for col in range(N):
            if(abs(U[row][col]) > eps):
                zerorow = False
        if(zerorow):
            unique = False
    if(not unique):
        outfile.write('No unique solution\n')
    else:
        Y = BackSubLower(L, B, N)
        X = BackSubUpper(U, Y, N)
        
        for row in range(N):
            outfile.write("{:.4f}".format(Y[row][0]))
            outfile.write('\n')
        outfile.write('\n')
        
        
        for row in range(N):
            outfile.write("{:.4f}".format(X[row][0]))
            outfile.write('\n')
        
    outfile.close()
        
    
    
    