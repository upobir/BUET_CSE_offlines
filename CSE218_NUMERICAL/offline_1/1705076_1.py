# -*- coding: utf-8 -*-
"""
Created on Tue May 21 05:11:18 2019

@author: sabbir

offline_1 problem_1
"""

import numpy as np
import matplotlib.pyplot as plt

def TaylorLn(x, n):
    """ approximates ln(1+x) with first n terms of taylor series """
    ans = 0
    y = -x
    x = -x
    for i in range(1, n+1):
        ans -= y/i
        y *= x
    return ans




if __name__ == "__main__":
    
    step = 0.1
    X = np.arange(-1+step, 1+step, step)
    Yreal = np.log(1+X)
    Y1 = TaylorLn(X, 1)
    Y3 = TaylorLn(X, 3)
    Y5 = TaylorLn(X, 5)
    Y20 = TaylorLn(X, 20)
    Y50 = TaylorLn(X, 50)
    
    plt.figure()
    plt.xlim(-1, 1)
    plt.plot(X, Y1, label = "1 term approx", color = "blue")
    plt.plot(X, Y3, label = "3 term approx", color = "green")
    plt.plot(X, Y5, label = "5 term approx", color = "grey")
    plt.plot(X, Y20, label = "20 term apporx", color = "red")
    plt.plot(X, Y50, label = "50 term approx", color = "violet")
    plt.plot(X, Yreal, label = "real values", color = "black")
    
    plt.title("Graphs of ln(1+x), real and taylor approximations")
    plt.xlabel("x")
    plt.ylabel("ln(1+x)")
    plt.legend()
    
    
    X = np.arange(1,51)
    Y = np.array([TaylorLn(0.5, x) for x in X])
    approxval = np.log(1+0.5)
    E = np.abs(approxval - Y)/approxval
    
    plt.figure()
    plt.xlim(0,50)
    plt.plot(X, E, label = "relative approx error for ln(1+x)")
    plt.title("relative arppoximation error for taylor \nseries approximation of ln(1+x) uptp 50 terms")
    plt.xlabel("number of terms")
    plt.ylabel("relative approximation error")
    plt.legend()

    
    