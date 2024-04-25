#Function calls within an expression, with arithmetic in parameter


a = 6 * 2 + 7 + 8 + 6 - 15
b = 10 + a + a + 2 * a + a + 5 
c = a + 3 + b + 3 + 2 * 3 - 25

def evaluate(val):
    z = 5 * 2 + 5 * 2 - 5
    tmp = val + z
    return tmp


tmpA = evaluate(a-5) - 3 
tmpB = 14 + evaluate(b+c)
tmpC = 2 * evaluate(c*2)
tmpD = evaluate(5+b) + 4
tmpE = 100 - evaluate(2)
tmpF = evaluate(3+2) * 2
print("tmpA =", tmpA)
print("tmpB =", tmpB)
print("tmpC =", tmpC)
print("tmpD =", tmpD)
print("tmpE =", tmpE)
print("tmpF =", tmpF)
