#Function calls with global variables


a = 10 + 31 - 20 + 3 * 2 + 3 - 14 + 6 * 2
b = 10 + 11 + a + 2 - 3 + a + a - 5 

def evaluate(a):
    val = a * 2
    a = val - 5

    c = a - 4
    d = c * val + 5
    result = 0
    if 16 < d:
        result = 1
    else:
        result = 8
    
    a = result
    return a

def increment(b):
    scale = 0
    x = b + 3
    val = 3 * x + 5 + 7 + 5 - 4 * 2
    b = val + x - 7
    if val > 45:
        scale = 1
    else:
        scale = 2

    b = scale
    return b


tmpA = evaluate(a)
tmpB = increment(b)
print("a =", a)
print("b =", b)
print("tmpA =", tmpA)
print("tmpB =", tmpB)

