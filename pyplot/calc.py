#!/bin/env python3
from math import *

h = 7
r = 10
xf = 60
s = h/xf
gm = h*s
dg = 2*s

def find_g():
    g = 1
    for i in range(10):
        t1 = (2*r/g)**(2/3)
        # print(f"{t1=}")
        root = sqrt((t1-1)/2)
        g = gm + g*root*dg
        print(f"iteration {i}: {g=}")

g = 1.4
y0 = (g - gm) / dg

print(f"{y0=}")

x = y0+h-0.07
L = (x**2 - y0**2)/g + r*g/sqrt(g**2 + 4*x**2)
print(f"{L=}")
H = x - 2*r*x/sqrt(g**2 + 4*x**2) - (y0-r)
print(f"{H=}")
# find_g()