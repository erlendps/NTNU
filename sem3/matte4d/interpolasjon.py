import numpy as np
import sympy as sp

x = sp.symbols("x")

for i in range(1,1):
    print("ja")

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
    omega_1 = omega_func(1, data_x)
    omegas = np.append(omegas, omega_1)

    if (len(data_x) <= 2):
        return;

    for i in range(2, len(data_x)):
        if (i == len(data_x) - 1):
            j = -1
        else:
            j = i+1
        c_i = divided_diff(data_x[0:j], data_y[0:j]) #(divided_diff(data_x[1:j], data_y[1:j]) - constants[i-1])/(data_x[i] - data_x[0])
        constants = np.append(constants, c_i)
        omega_i = omega_func(i, data_x)
        omegas = np.append(omegas, omega_i)

    print(constants)
    print(omegas)
    return np.dot(constants, omegas)



def omega_func(i, data_x):  # i must be lower than len(data_x)
    product = 1
    if (i > 0):
        for j in range(1, i+1):
            product = product * (x - data_x[i-1])
    return product

    """Or if you want to do it recursivly
    if (i == 0):
        return 1
    else:
        return (x-data[i-1])*omega_func(i-1, data_x)
    """

def divided_diff(data_x, values):
    if (len(values) == 1):
        return values[0]
    else:
        return (divided_diff(data_x[1:] ,values[1:]) - divided_diff(data_x[0:-1],values[0:-1]))/(data_x[-1] - data_x[0])


# main program
data_x = [3, 4, 5]
data_y = [2, 6, 12]


print(interpolate(data_x, data_y))
