a = 5 + 3 * 5 + 3 * 2 + 3 + 24 + 6 - 25
b = 10 + a + a + 2 * 3 * a + a + 5 

def evaluate(val):
    z = 5 * 2 + 6 + 3 - 5 + 3 * 2
    tmp = val + z
    return tmp


tmpA = 6 + evaluate(a)
#tmpB = 14 + sumInts(a,b)
print("a =", a)
print("b =", b)
print("tmpA =", tmpA)