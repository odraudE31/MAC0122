import math

L = int(input("type L: "))
l = (5/6) * L

A = pow(L, 2) * pow(3, 1/2) / 4
a = pow(l, 2) * pow(3, 1/2) / 4

At = (A - a)/3

x = (2*At)/(l*math.sin(math.radians(7)))

shift = L - x

print(shift)