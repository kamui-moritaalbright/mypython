#Recursively sum until n

def sumn(n):
    if n == 0:
        return 0
    else:
        return n + sumn(n-1)


a = 15
b = 11

suma = sumn(a)
sumb = sumn(b)

print("sum A =",suma)
print("sum B =",sumb)
