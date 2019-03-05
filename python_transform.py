# Qx : array with all the x positions of the distorted quad
# Qy : array with all the y positions of the distorted quad
# x  : x position of point you want normalised
# y  : y position of point you want normalised
def quad_to_cell(Qx, Qy, x, y):

  #WJW 7-13-15
  #This function performs a coordinate transform from X,Y space to the normalized L,M.
  #
  #If a point {is within {0,1} on both axes, it is within the transformed unit square.
  #Qx,Qy vectors contain the 4 coordinates of the corners - x and y values, respectively, ordered as indicated below:
  #
  #The unit cell L(l,m) corresponding to Q(x,y) is oriented as:
  #L0(x=0,y=0),L1(0,1), L2(1,1), L3(1,0).  The order matters.
  #The following represent an algebraic solution to the system:
  #l=a1 + b1x + c1y + d1xy
  #m=a2 + b2x + c2y + d2xy


    L_Out = [0.0, 0.0]

    ax = (x - Qx[0]) + (Qx[1] - Qx[0]) * (y - Qy[0]) / (Qy[0] - Qy[1])
    a3x = (Qx[3] - Qx[0]) + (Qx[1] - Qx[0]) * (Qy[3] - Qy[0]) / (Qy[0] - Qy[1])
    a2x = (Qx[2] - Qx[0]) + (Qx[1] - Qx[0]) * (Qy[2] - Qy[0]) / (Qy[0] - Qy[1])
    ay = (y - Qy[0]) + (Qy[3] - Qy[0]) * (x - Qx[0]) / (Qx[0] - Qx[3])
    a1y = (Qy[1] - Qy[0]) + (Qy[3] - Qy[0]) * (Qx[1] - Qx[0]) / (Qx[0] - Qx[3])
    a2y = (Qy[2] - Qy[0]) + (Qy[3] - Qy[0]) * (Qx[2] - Qx[0]) / (Qx[0] - Qx[3])
    bx = x * y - Qx[0] * Qy[0] + (Qx[1] * Qy[1] - Qx[0] * Qy[0]) * (y - Qy[0]) / (Qy[0] - Qy[1])
    b3x = Qx[3] * Qy[3] - Qx[0] * Qy[0] + (Qx[1] * Qy[1] - Qx[0] * Qy[0]) * (Qy[3] - Qy[0]) / (Qy[0] - Qy[1])
    b2x = Qx[2] * Qy[2] - Qx[0] * Qy[0] + (Qx[1] * Qy[1] - Qx[0] * Qy[0]) * (Qy[2] - Qy[0]) / (Qy[0] - Qy[1])
    by = x * y - Qx[0] * Qy[0] + (Qx[3] * Qy[3] - Qx[0] * Qy[0]) * (x - Qx[0]) / (Qx[0] - Qx[3])
    b1y = Qx[1] * Qy[1] - Qx[0] * Qy[0] + (Qx[3] * Qy[3] - Qx[0] * Qy[0]) * (Qx[1] - Qx[0]) / (Qx[0] - Qx[3])
    b2y = Qx[2] * Qy[2] - Qx[0] * Qy[0] + (Qx[3] * Qy[3] - Qx[0] * Qy[0]) * (Qx[2] - Qx[0]) / (Qx[0] - Qx[3])

    L_Out[0] = (ax / a3x) + (1.0 - a2x / a3x) * (bx - b3x * ax / a3x) / (b2x - b3x * a2x / a3x)
    L_Out[1] = (ay / a1y) + (1.0 - a2y / a1y) * (by - b1y * ay / a1y) / (b2y - b1y * a2y / a1y)

    return L_Out

result = quad_to_cell([0.0, 0.0, 2.0, 2.0], [0.0, 2.0, 2.0, 0.0], 1.0, 1.0)

print result[0]
print result[1]
