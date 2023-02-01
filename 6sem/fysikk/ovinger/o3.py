import matplotlib.pyplot as plt
import math
import numpy as np

angles = []
forces = []
m = 0.185

with open("input.txt", "r") as f:
    for line in f.readlines():
        line = line.split(",")
        angles.append(eval(line[0]))
        forces.append(float(line[1]))

def coeff_value(angle, weight):
    return (1/angle) * math.log(weight/m)
# mean
coeffs = [coeff_value(angles[i], forces[i]) for i in range(8)]

# convert to numpy
angles = np.asarray(angles)
forces = np.asarray(forces)
coeffs = np.asarray(coeffs)

def expected_value(values):
    return values.sum() / values.size

def standard_dev(values):
    n = values.size
    ev = expected_value(values)
    return math.sqrt(1/(n-1)*((values - ev)**2).sum())

def uncertainity(values):
    return standard_dev(values)/math.sqrt(values.size)

print("Expected Value:", expected_value(coeffs))
print("Standard Deviation:", standard_dev(coeffs))
print("Uncertain:", uncertainity(coeffs))
print("μ =", expected_value(coeffs), "±", uncertainity(coeffs))

