#Function calls and declarations

val = 10

def evaluate(x):
    b = x + 3
    c = x + 7
    d = c * b + 5
    result = 0
    if d < c:
        result = 1
    return result


result = evaluate(val)

print("result =", result)

