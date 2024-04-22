# 2 if else statements in 1 nested level
a = 5
b = 10
val = a * 2 + b - 3
c = 0
y = 3
if val < 20:
    d = 5
    tempA = d + a * 3 - b
    if tempA > 10:
        c = 0
    else:
        c = 3
    z = d * 9
    # Additional nested if-else
    if z < 40:
        y = 1
    else:
        #target
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
