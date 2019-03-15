import numpy as np
import cv2

img = np.zeros((1024, 1024, 3), np.uint8)


#xa = [100,150,200,200,200,150,100,100]
#ya = [100,100,100,150,200,200,200,150]

#xa = [100,150,0,0,0,0,0,125]
#ya = [100,100,0,0,0,0,0,175]

#xa = [0,150,200,200,0,0,0,0]
#ya = [0,150,100,150,0,0,0,0]

#xa = [0,0,0,200,200,150,0,0]
#ya = [0,0,0,150,200,200,0,0]

xa = [0,0,0,0,0,150,100,75]
ya = [0,0,0,0,0,225,200,150]

for i in range(0, len(xa)):
    xa[i] = xa[i] * 2
    ya[i] = ya[i] * 2


for i in range(0, len(xa)):
    if xa[i] != 0:
        cv2.circle(img,(xa[i], ya[i]), 10, (0,0,255), -1)

if xa[0] != 0 and xa[2] != 0 and xa[4] != 0 and xa[6] != 0:
    continue

elif xa[7] != 0 and xa[0] != 0 and xa[1] != 0:
    xa[6] = xa[0] + (xa[7] - xa[0]) * 2
    ya[6] = ya[0] + (ya[7] - ya[0]) * 2

    xa[2] = xa[0] + (xa[1] - xa[0]) * 2
    ya[2] = ya[0] + (ya[1] - ya[0]) * 2

    xa[4] = xa[2] + (xa[6] - xa[0])
    ya[4] = ya[6] + (ya[2] - ya[0])

elif xa[1] != 0 and xa[2] != 0 and xa[3] != 0:
    xa[0] = xa[2] - (xa[2] - xa[1]) * 2
    ya[0] = ya[2] - (ya[2] - ya[1]) * 2

    xa[4] = xa[2] + (xa[3] - xa[2]) * 2
    ya[4] = ya[2] + (ya[3] - ya[2]) * 2

    xa[6] = xa[4] + (xa[0] - xa[2])
    ya[6] = ya[0] + (ya[4] - ya[2])

elif xa[3] != 0 and xa[4] != 0 and xa[5] != 0:
    xa[2] = xa[4] - (xa[4] - xa[3]) * 2
    ya[2] = ya[4] - (ya[4] - ya[3]) * 2

    xa[6] = xa[4] - (xa[4] - xa[5]) * 2
    ya[6] = ya[4] - (ya[4] - ya[5]) * 2

    xa[0] = xa[6] - (xa[4] - xa[2])
    ya[0] = ya[2] + (ya[6] - ya[5])

elif xa[5] != 0 and xa[6] != 0 and xa[7] != 0:
    xa[0] = xa[6] - (xa[6] - xa[7]) * 2
    ya[0] = ya[6] - (ya[6] - ya[7]) * 2

    xa[4] = xa[6] + (xa[5] - xa[6]) * 2
    ya[4] = ya[6] + (ya[5] - ya[6]) * 2

    xa[2] = xa[4] - (xa[6] - xa[0])
    ya[2] = ya[4] - (ya[6] - ya[0])


cv2.circle(img,(xa[0], ya[0]), 5, (255,255,255), -1)
cv2.circle(img,(xa[2], ya[2]), 5, (255,255,255), -1)
cv2.circle(img,(xa[4], ya[4]), 5, (255,255,255), -1)
cv2.circle(img,(xa[6], ya[6]), 5, (255,255,255), -1)


cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
