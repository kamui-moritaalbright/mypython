# Function calls within an expression, function in parameter

num = 7 + 3 * 3 + 4 * 2 + 2 + 18 + 7 - 22
sumVal = 8 + num + num + 2 * 2 * num + num + 6

def compute(value):
    increment = 2 + 5 + 4 - 4 + 3 * 1
    result = value + increment
    return result

def addFactors(x, y):
    multiplier = 2
    result = x * multiplier + y
    return result

resultA = compute(num) + 8
resultB = 12 + addFactors(compute(num), compute(sumVal))
print("num =", num)
print("sumVal =", sumVal)
print("resultA =", resultA)
print("resultB =", resultB)
