#Function calls within an expression


a = 5 + 3 * 5 + 3 * 2 + 3 + 24 + 6 - 25
b = 10 + a + a + 2 * 3 * a + a + 5 

def evaluate(val):
    z = 5 * 2 + 6 + 3 - 5 + 3 * 2
    tmp = val + z
    return tmp

def sumInts(x,y):
    scale = 2
    tmp = x * scale + y
    return tmp


tmpA = evaluate(a) + 6
tmpB = 14 + sumInts(a,b)
print("a =", a)
print("b =", b)
print("tmpA =", tmpA)
print("tmpB =", tmpB)
