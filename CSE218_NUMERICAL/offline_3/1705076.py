# -*- coding: utf-8 -*-
"""
Created on Fri Jul 26 18:34:58 2019

@author: sabbir
"""

import numpy as np
import matplotlib.pyplot as plt

def GaussJordan(n, A, b):
    for c in range(n):
        p = c;
        for i in range(c, n):
            if(np.fabs(A[i][c]) > np.fabs(A[p][c])):
                p = i
        if(p != c):
            A[[c, p]] = A[[p, c]]
            b[c], b[p] = b[p], b[c]
        b[c] = b[c] / A[c][c]
        A[c] = A[c] / A[c][c]
        for i in range(n):
            if(i == c):
                continue
            b[i] = b[i] - A[i][c]*b[c]
            A[i] = A[i] - A[i][c]*A[c]
    return
            
def Regression(n, X, Y, d):
    A = np.zeros((d+1, d+1))
    b = np.zeros(d+1)
    for r in range(d+1):
        for c in range(d+1):
            for i in range(n):
                A[r][c] = A[r][c] + X[i]**(r+c)
        for i in range(n):
            b[r] = b[r] + Y[i]*(X[i]**(r))
            
    GaussJordan(d+1, A, b)
    return b

def Polynomial(poly, d, x):
    ret = 0
    for i in range(d+1):
        ret = ret + poly[i]*(x**i)
    return ret

def RegressionCoef(poly, d, X, Y, n):
    ybar = 0
    for i in range(n):
        ybar = ybar + Y[i]
    ybar = ybar / n
    
    St = 0
    Sr = 0
    for i in range(n):
        St = St + (Y[i] - ybar)**2
        Sr = Sr + (Y[i] - Polynomial(poly,  d, X[i]))**2
    
    r = np.sqrt((St - Sr)/St)
    return r

if __name__ == "__main__":
    X = []
    Y = []
    with open('data.txt', 'rt') as file:
        for line in file:
            pair = line.split(' ')
            X.append(float(pair[0]))
            Y.append(float(pair[1]))
    n = len(X)
        
    X = np.array(X)
    Y = np.array(Y)
    
    plt.figure(figsize=(12,8))
    plt.scatter(X, Y, s = 1, color = 'green')
    x1, x2 = plt.xlim()
    
    linear = Regression(n, X, Y, 1)
    quadratic = Regression(n, X, Y, 2)
    cubic = Regression(n, X, Y, 3)
    
    xaxis = np.arange(x1, x2, 0.1)
    yaxis1 = np.array([Polynomial(linear, 1, x) for x in xaxis])
    yaxis2 = np.array([Polynomial(quadratic, 2, x) for x in xaxis])
    yaxis3 = np.array([Polynomial(cubic, 3, x) for x in xaxis])
    
    plt.plot(xaxis, yaxis1, color = 'purple', label = 'linear')
    plt.plot(xaxis, yaxis2, color = 'red', label = 'quadratic')
    plt.plot(xaxis, yaxis3, color = 'blue', label = 'cubic')
    plt.legend()
    
    print('linear regression')
    for i in range(2):
        print('a' + str(i) + ': ' + str(linear[i]))
    print('Regression Coefficient: '+ str(RegressionCoef(linear, 1, X, Y, n)))
    print()
    
    print('quadratic regression')
    for i in range(3):
        print('a' + str(i) + ': ' + str(quadratic[i]))
    print('Regression Coefficient: '+str(RegressionCoef(quadratic, 2, X, Y, n)))
    print()
    
    print('cubic regreesion')
    for i in range(4):
        print('a' + str(i) + ': ' + str(cubic[i]))
    print('Regression Coefficient: '+str(RegressionCoef(cubic, 3, X, Y, n)))
    
    
    
    
    
    
    
    
    