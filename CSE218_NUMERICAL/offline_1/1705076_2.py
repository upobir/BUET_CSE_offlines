"""
Created on Tue May 21 05:11:18 2019

@author: sabbir

offline_1 problem_1
"""

import numpy as np
import matplotlib.pyplot as plt

K = 0.05
Pt = 3

def func(x):
    return x/(1-x)*np.sqrt(2*Pt/(2+x))-K

def RelError(trueval, approx):
    return abs(trueval-approx)/trueval*100

def Secant(f, r0, r1, eps, maxit):
    it = 0
    while(it <= maxit):
        slope = (r1-r0)/(f(r1)-f(r0))
        r1, r0 = r1-slope*f(r1), r1
        if(RelError(r1, r0) < eps):
            return (r1, it)
        it += 1
    print("maximum iteration limit crossed")
    return (np.nan, it)

def FalsePosition(f, l, u, eps, maxit):
    if(f(l) == 0):
        return (l, 0)
    elif(f(u) == 0):
        return (u, 0)
    elif(f(u)*f(l) > 0):
        print("Initial bounds are incorrect")
        return (np.nan, 0)

    x0 = l
    x1 = u
    
    it = 1
    while(it <= maxit):
        xm = (f(x0)*x1-f(x1)*x0)/(f(x0)-f(x1))
        err = RelError(xm, x1)
        
        prod = f(xm)*f(x1)
        if(prod == 0):
            return (xm, it)
        elif(prod < 0):
            x0 = x1
        
        x1 = xm
        if(err < eps):
            return (x1, it)
        it += 1
    return (np.nan, it)    
    
if __name__ == "__main__":
    X = np.arange(0, 0.1, 0.001)
    Y = func(X)
    Z = np.zeros(X.shape)
    plt.figure()
    plt.plot(X, Y,  label = "Function")
    plt.plot(X, Z, label = "Zero")
    plt.title("Graphical root finidng")
    plt.legend()
    print("from graph, it seems like x = 0.029 is a root")
    
    print("From secant method:")
    result = Secant(func, 0, 0.9, 0.5, 500)
    print("Root:")
    print(result[0])
    print("Number of Iteration:")
    print(result[1])
    
    print("From false position method:")
    result = FalsePosition(func, 0, 0.9, 0.5, 500)
    print("Root:")
    print(result[0])
    print("Number of Iteration:")
    print(result[1])
    
    