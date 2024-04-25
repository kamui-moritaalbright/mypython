#Evaluate if else with varying comparisons


a = 10
b = 5
c = a + b
d = a * b

x = a * b + a + b * a + b + c + c * d
y = 0
z = 0

#variable integer
if c > 9:
    tmpA = 6 + 3
    tmpB = 7 * 4 - 13 + tmpA
    z = tmpA + tmpB - 13
else:
    tmpA = 6 * 3
    tmpB = 7 - 4 + 13 + tmpA
    z = tmpA + tmpB + 5

#variable variable 
if b > c:
    y = 5 + 4 * 3
    resA = 1
    resB = y
else:
    resA = 5 + 3 + 8 + 14 - 6 + 3 * 2
    resB = 3 + 3 * 2 + 13 + 5 * 1
    y = resA + resB - 3

#integer variable
e = 5
if 7 < e:
    t1 = 3 + 9 - 5 + 6 * 2
    t2 = 7 + 2 + 2 + 4 - 6 + 3 * 2
    e = t1 + t2

print("e =", e)
print("y =", y)
print("z =", z)
