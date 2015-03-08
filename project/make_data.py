import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt

mpl.rcParams['legend.fontsize'] = 10

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_zlim(-10, 10)

def make_gauss(n, mean=0, sd=1):
    x = np.random.randn(n)
    y = np.random.randn(n)
    z = np.random.randn(n)
    ax.scatter(x, y, z, "k", marker= ".")


def make_ring(n, radius = 10, noise = 1):
    theta = 2*np.pi*np.random.rand(n)
    x = radius*np.cos(theta)+noise*np.random.randn(n)
    y = radius*np.sin(theta)+noise*np.random.randn(n)
    z = np.random.randn(n)*noise/10  #felt better
    ax.scatter(x, y, z, "k", marker= ".")

make_gauss(100)    
make_ring(100)


plt.show()
