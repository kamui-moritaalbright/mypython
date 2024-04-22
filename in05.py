# 2 if else statements in 1 nested level
a = 5
b = 10
val = a * 2 + b - 3
c = 0
y = 0
if val < 20:
    d = 15
    tempA = d + a * 3 - b
    if tempA > 10:
        c = 0
    else:
        c = 3
    z = d + 15
    # Additional nested if-else
    if z == 30:
        y = 1
    else:
        y = 0
else:
    e = 16
    tempB = e * 2 + 12 - a
    if tempB < 50:
        c = 1
    else:
        c = 2

print("val =", val)
print("c =", c)
print("y =", y)
