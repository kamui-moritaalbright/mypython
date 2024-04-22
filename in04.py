# TestCase: if-else, nested in else 

a = 5
b = 4
val = a * 3 - 2 * b + 3 
c = 0
# if-else condition
if val > 50: 
    d = 11 
    tempA = 5 + 7 - 2 * d + 4 
    if tempA > 30: 
        c = 0
    else:
        c = 3 
else:
    e = 14 
    tempB = 20 + e + 6 - 2 
    if tempB > 75: 
        c = 1
    else:
        c = 2 

print("val =", val)
print("c =", c)

