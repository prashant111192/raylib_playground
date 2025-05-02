import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("velocity.dat")
x, y, ux, uy = data[:, 0], data[:, 1], data[:, 2], data[:, 3]
plt.quiver(x, y, ux, uy)
plt.title("Velocity Field")
plt.xlabel("x")
plt.ylabel("y")
plt.show()
