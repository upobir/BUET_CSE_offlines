# -*- coding: utf-8 -*-
"""
Created on Fri Aug 23 15:46:15 2019

@author: User
"""

import numpy as np
import matplotlib.pyplot as plt
eps = 1e-5

def eq(x, y):
    if(abs(x - y) < eps):
        return True
    return False

def Trapezoidal(arr, a, t):
    h = arr[a+1][0] - arr[a][0]
    ans = arr[a][1] + arr[a+1][1]
    ans *= h/2.0
    plt.axvspan(arr[a][0], arr[a+1][0], color='red', alpha = 0.5, label = '_'*t + 'Trapezoidal')
    return ans

def Simpson_1_3(arr, a, t):
    h = arr[a+1][0] - arr[a][0]
    ans = arr[a][1] + 4.0*arr[a+1][1] + arr[a+2][1]
    ans *= h/3.0
    plt.axvspan(arr[a][0], arr[a+1][0], color='blue', alpha = 0.5, label = '_'*t + '1/3 rule')
    plt.axvspan(arr[a+1][0], arr[a+2][0], color='blue', alpha = 0.5)
    return ans

def Simpson_3_8(arr, a, t):
    h = arr[a+1][0] - arr[a][0]
    ans = arr[a][1] + 3.0*arr[a+1][1] + 3.0*arr[a+2][1] + arr[a+3][1]
    ans *= h*3.0/8.0
    plt.axvspan(arr[a][0], arr[a+1][0], color='green', alpha = 0.5, label = '_'*t + '3/8 rule')
    plt.axvspan(arr[a+1][0], arr[a+2][0], color='green', alpha = 0.5)
    plt.axvspan(arr[a+2][0], arr[a+3][0], color='green', alpha = 0.5)
    return ans
        

def Integration(arr, n):
    ans = 0
    a = 0
    t = 0
    s1 = 0
    s2 = 0
    
    plt.figure(figsize=(10,8))
    plt.plot(arr[:, 0], arr[:, 1], color = 'black', label = 'Integrand')
    
    while (a < n-1):
        b = a+1
        while (b < n and eq(arr[a+1][0]-arr[a][0], arr[b][0] - arr[b-1][0])):
            b += 1
        b -= 1
        # print(str(a) + ' to ' + str(b))
        if(b - a == 1):
            ans += Trapezoidal(arr, a, t)
            t += 1
            a += 1
        if((b-a) % 3 != 0):
            ans += Simpson_1_3(arr, a, s1)
            s1 += 2
            a += 2
        if((b-a) % 3 != 0):
            ans += Simpson_1_3(arr, a, s2)
            s1 += 2
            a += 2
        while(a < b):
            ans += Simpson_3_8(arr, a, s2)
            s2 += 3
            a += 3
            
    plt.xlabel('X values')
    plt.ylabel('Y values')
    plt.legend(loc = 'upper left')
    plt.show()
    
    return (ans, t, s1, s2)

if __name__ == "__main__":
    inputfile = []
    with open('input.txt', 'rt') as file:
        for line in file:
            inputfile.append(line)
    N = int(inputfile[0])
    
    arr = np.empty((N, 2))
    for i in range(N):
        arr[i] = np.array([float(x) for x in inputfile[i+1].split(' ')])
    
    minX = arr[0][0]
    maxX = arr[N-1][0]
    
    result = Integration(arr, N)
    print('Trapeziod: '+ str(result[1]) +' intervals')
    print('1/3 rule : '+ str(result[2]) +' intervals')
    print('3/8 rule : '+ str(result[3]) +' intervals')
    print()
    print('Intergration Result: ' + str(result[0]))