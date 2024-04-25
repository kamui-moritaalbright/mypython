#Evaluate assignment and reassignment with scopes

a = 8
b = 3
val = a * 2 + b - 3

a = val + 1 + 3 * 2
c = 0 + b * 3 + a - a
b = a - c + 5 + 2 + 7 + val * 2


#local variable a,b
def evaluate(x):
  a = x + 3 * 2
  b = a + 4 - 6
  tmp = 25 - 3 + a * 2 - 16
  return tmp

#Test if global variables are modified in function
tmp = evaluate(a)

#a should be 23, b should be 60
print("a =", a)
print("b =", b)
print("result =", tmp)
