#Evaluate if with and without else block


a = 10
b = 5
c = a + b
d = a * b

x = a * b + a + b * a + b + c + c * d
y = 0
z = 0

#Check if block
if a > 9:
    tmpA = 6 + 3
    tmpB = 7 * 4 - 13 + tmpA
    z = tmpA + tmpB - 13
else: 
    tmpA = 6 * 3
    tmpB = 7 - 4 + 13 + tmpA
    z = tmpA + tmpB + 5

#Check else block
if x <= 100:
    y = 5 + 4 * 3 
    resA = 1
    resB = y
else: 
    resA = 5 + 3 + 8 + 14 - 6 + 3 * 2
    resB = 3 + 3 * 2 + 13 + 5 * 1
    y = resA + resB - 3

#Check if block with no else
e = 15
if b < 6:
    t1 = 3 + 7 + 14 - 5 + 6 * 2 
    t2 = 7 + 2 + 2 + 4 - 6 + 3 * 2
    e = t1 + t2

print("e =", e)
print("y =", y)
print("z =", z)
