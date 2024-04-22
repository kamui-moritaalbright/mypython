#Function with if/else

a = 10

def evaluate(x):
    b = x + 3
    c = x + 7
    d = c * b + 5
    result = 0
    if c > 13:
        result = 1
    return result

testValue = 12
result = evaluate(testValue)

print("result =", result)

