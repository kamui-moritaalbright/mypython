#Functions with nested if/else

a = 10
b = 16
x = 3
y = 9
z = 0
c = a + 5 + b * x * y - 30
def evaluate(x):
    b = x * 2 
    c = x - 4
    d = c * b + 5
    result = 0
    if c > 13:
        result = 0
    else:
        result = 6
    return result

def increment(x):
    scale = 0
    val = 3 * x + 5 + 7 + 5 - 4 * 2
    
    if val > 45:
        e = 1 + 3 + 6 - 5 + 7 - 3
        if e > 7:
            scale = 2
        else:
            scale = 4
    else:
        e = 8 * 3 - 6 - 5 + 7 - 11
        if e > 23:
            scale = 3
        else:
            #target
            scale = 5 
    x = x * scale
    return x


z = evaluate(x)
result = increment(y)
print("z =", z)
print("result =", result)

