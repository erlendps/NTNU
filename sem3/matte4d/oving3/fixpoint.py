import math as ma

def fixpoint(function, initial_value, iteration):
    if (iteration == 0):
        return initial_value
    else:
        return function(fixpoint(function, initial_value, iteration-1))

def g(x):
    return x - ma.sin(x) + 1

print(fixpoint(g, ma.pi/4, 5))
print(ma.pi/2)
