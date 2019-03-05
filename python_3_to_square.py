x = [0.0,0.0,5.0]
y = [5.0,0.0,-5.0]

ax = x[0] - x[1]
ay = y[0] - y[1]

bx = x[2] - x[1]
by = y[2] - y[1]

fx = x[1] + ax + bx
fy = y[1] + ay + by

print fx, ",", f
