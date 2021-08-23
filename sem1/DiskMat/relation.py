A = (0,1)
B = (2,3)

def f(x):
    if x in A or x in B:
        return 1
    else:
        return 0


def g(x):
    if x in A:
        return 1
    else:
        return 0

def h(x):
    if x in B:
        return 1
    else:
        return 0

for i in range(5):
    print(g(i)+h(i)-g(h(i)))
