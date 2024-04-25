#Scope for function calling another function

# Global variables
a = 10
b = 5

def addSum(x):
    z = x + 3 + a
    x = x + b
    result = 3 + x + x * 3 + 1 - 4 + 16
    return result

def evaluateSums(x, y):
    c = a * 3 + x - y
    x = c - x + 2
    tmp = addSum(x)
    return tmp

tmp = evaluateSums(a, b)
print("tmp =", tmp)    
