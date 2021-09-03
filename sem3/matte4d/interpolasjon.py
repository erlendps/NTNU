import numpy as np
import sympy as sp

x = sp.symbols("x")

"""
Given a set of n+1 datapoints (x_i, y_i), the interpolate-function returns a polynom p
of deg(p) = n that interpolates all of the n+1 datapoints. This function uses Newtons idea
in which the polynom p is defined as p(x) = sum_(i, 0, n)(c_i * w_i(x)) where w_i is the
omega function and c_i is a constant and equal to the divided difference of i+1 values
[y0, y1, ..., y(i+1)].
"""
def interpolate(data_x, data_y):  
    constants = np.array([])
    c_0 = data_y[0]
    c_1 = (data_y[1] - c_0)/(data_x[1] - data_x[0])
    constants = np.append(constants, [c_0, c_1])
    omegas = np.array([1])
    omega_1 = x - data_x[0]
    omegas = np.append(omegas, omega_1)

    if (len(data_x) <= 2):
        return;

    for i in range(2, len(data_x)):
        if (i == len(data_x) - 1):
            c_i = divided_diff(data_x[0:], data_y[0:])
        else:
            c_i = divided_diff(data_x[0:i+1], data_y[0:i+1])
        
        constants = np.append(constants, c_i)
        omega_i = omegas[i-1] * (x-data_x[i-1])
        omegas = np.append(omegas, omega_i)

    print(constants)
    print(omegas)
    return sp.expand(np.dot(constants, omegas))


def divided_diff(data_x, values):
    if (len(values) == 1):
        return values[0]
    else:
        return (divided_diff(data_x[1:] ,values[1:]) - divided_diff(data_x[0:-1],values[0:-1]))/(data_x[-1] - data_x[0])


# main program
data_x = [3, 4, 5, 6]
data_y = [2, 6, 12, 26]


print(interpolate(data_x, data_y))
