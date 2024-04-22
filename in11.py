#Classical recursive factorial
def f(n):
    if n < 2: 
        return 1
    else:
        return n * f(n-1)


x = f(6)
y = f(2)
z = f(1)
print("x factorial =",x)
print("y factorial =",y)
print("z factorial =",z)
