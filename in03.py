# TestCase: if-else nested else

a = 5
b = 10
val = a * 2 + b * 3 + 1 * 5 + a - b
c = 0
# if-else condition
if val == 40:
    d = 12
    tempA = 5 + 6 - 3 * d + 5
    if tempA == 33:
        c = 0
    else:
        c = 2 
else:
    e = 13
    tempB = 23 + e + 5 - 3
    if tempB < 80:
        c = 1

print("val =", val)
print("c =", c)

