#!/bin/env python3

from sympy import plot_parametric, symbols, sqrt, cos, sin
from matplotlib import pyplot as plt

savepictures = False

def add_parametric(xt, yt, range, **kwargs):
    p = plot_parametric(xt, yt, range, show=False)
    pts = p[0].get_points()
    plt.plot(pts[0], pts[1], **kwargs)

def show_plots(save):
    plt.legend()
    plt.title("Cam profile")
    plt.gca().set_aspect('equal')
    plt.gca().spines[['left', 'bottom']].set_position('zero')
    plt.gca().spines[['top', 'right']].set_visible(False)
    if save:
        plt.savefig("pictures/picture.png", dpi=600, transparent=True)
        plt.savefig("pictures/picture.svg", dpi=600, transparent=True)
    plt.show()

plt.style.use('dark_background')

#============================ PLAYGROUND ======================================

h = 7
r = 10
xf = 60
s = h/xf
gm = h*s
dg = 2*s
g = 1.4
y0 = (g - gm) / dg

t = symbols('t')

add_parametric(r*cos(t), r*sin(t)+y0, (t, -4, 4),
        label= "ball", color='#505050')
add_parametric((t**2 - y0**2)/g + r*g/sqrt(g**2 + 4*t**2),
        t - 2*r*t/sqrt(g**2 + 4*t**2), (t, y0, y0 + h),
        label= "cam", color='blue')
add_parametric((t**2 - y0**2)/g, t, (t, -(y0 + h), y0 + h),
        label= "center", color='red', linestyle=':')

show_plots(savepictures)