import math as ma

def newton(f, df, x0, tol, max_iter, force=False):
    count = 0
    x = x0
    while (count < max_iter):
        fx = f(x)
        x = x - fx/df(x)
        print(f"i = {count}, x = {x}, f(x) = {f(x)}")
        count += 1
    return x
    

def f(x):
    return x+ma.sin(x)-1

def df(x):
    return 1+ma.cos(x)

newton(f, df, ma.pi/4, 10**-9, 5)
