import numpy as np
from matplotlib import pylab as plt

A = np.fromfile("salida.raw", dtype='int32')
A = A.reshape([2001, 2001])

print(A)
plt.imshow(A)
plt.show()