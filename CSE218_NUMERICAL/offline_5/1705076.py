# -*- coding: utf-8 -*-
"""
Created on Fri Aug 30 20:26:38 2019

@author: User
"""

import numpy as np
import matplotlib.pyplot as plt
eps = 1e-5

def func(x, y):
    return (x+20*y)*np.sin(x*y)

def Euler(X, initY, h, f):
    Y = np.copy(X)
    Y[0] = initY
    for i in range(1, X.shape[0]):
        Y[i] = Y[i-1] + f(X[i-1], Y[i-1])*h
    return Y

def RK_2(X, initY, h, a2, f):
    Y = np.copy(X)
    Y[0] = initY
    a1 = 1 - a2
    p = 0.5/a2
    q = 0.5/a2
    for i in range(1, X.shape[0]):
        k1 = f(X[i-1], Y[i-1])
        k2 = f(X[i-1] + p*h, Y[i-1] + q*k1*h)
        Y[i] = Y[i-1] + (a1*k1 + a2*k2)*h
    return Y

def RK_4(X, initY, h, f):
    Y = np.copy(X)
    Y[0] = initY
    for i in range(1, X.shape[0]):
        k1 = f(X[i-1], Y[i-1])
        k2 = f(X[i-1] + 0.5*h, Y[i-1] + 0.5*k1*h)
        k3 = f(X[i-1] + 0.5*h, Y[i-1] + 0.5*k2*h)
        k4 = f(X[i-1] + h, Y[i-1] + k3*h)
        Y[i] = Y[i-1] + (k1 + 2*k2  + 2*k3 + k4)*h/6.0
    return Y

steps = [0.01, 0.05, 0.1, 0.5]
Methods = ['Euler', 'Heun', 'Midpoint', 'Ralston', 'RK-4th order']

if __name__ == "__main__":
    X = []
    for i in range(4):
        X.append(np.arange(0, 10+eps, steps[i]))
    AllY = []
    
    
    # Euler
    Y = []    
    for i in range(4):
        Y.append(Euler(X[i], 4, steps[i], func))
    AllY.append(Y)
    
    # Heun
    Y = []    
    for i in range(4):
        Y.append(RK_2(X[i], 4, steps[i], 0.5, func))
    AllY.append(Y)
    
    # Midpoint
    Y = []    
    for i in range(4):
        Y.append(RK_2(X[i], 4, steps[i], 1.0, func))
    AllY.append(Y)
    
    # Ralston
    Y = []    
    for i in range(4):
        Y.append(RK_2(X[i], 4, steps[i], 2.0/3.0, func))
    AllY.append(Y)
    
    # RK 4th order
    Y = []    
    for i in range(4):
        Y.append(RK_2(X[i], 4, steps[i], 2.0/3.0, func))
    AllY.append(Y)


    for j in range(5):
        plt.figure(figsize=(10,6))
        plt.title(Methods[j] + ' Method')
        plt.xlabel('X values')
        plt.ylabel('Y values')
        for i in range(4):
            plt.plot(X[i], AllY[j][i], label = str(steps[i]) + ' step')
        plt.legend()
        plt.show()
        
    
    for i in range(4):
        plt.figure(figsize=(10,6))
        plt.title(str(steps[i]) + ' step')
        plt.xlabel('X values')
        plt.ylabel('Y values')
        for j in range(5):
            plt.plot(X[i], AllY[j][i], label = Methods[j] + ' Method')
        plt.legend()
        plt.show()
        

