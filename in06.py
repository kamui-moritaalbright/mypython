#Nested If/else
a = 16
b = 4
val = a + 3 * 1 + b - 3
c = 0
y = 0
if val < 20:
    d = 15
    tempA = d + a * 3 - b + a 
    if 10 < tempA:
        c = 2
    else:
        c = 5
else:
    e = 16
    tempB = e * 2 + 12 - a
    if tempB < 50:
        c = 1
    else:
        c = 3

print("val =", val)
print("c =", c)
print("y =", y)
