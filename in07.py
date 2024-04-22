#2 if else in the outer else block
a = 8
b = 3
val = a * 2 + b - 3
c = 0
y = 3
if val > 20:
    d = 5
    tempA = d + a * 3 - b
    if tempA > 10:
        c = 0
    else:
        c = 3
    z = d * 9
    if z < 40:
        y = 10
    else:
        y = 11
else:
    e = 16
    tempB = e * 2 + 12 - a
    if tempB < 50:
        y = 2
    else:
        y = 4
    tempC = e + b
    if tempC > 20:
        y = 3 
    else:
        y = 5

print("val =", val)
print("c =", c)
print("y =", y)

