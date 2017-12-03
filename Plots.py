#Este archivo usa ecoding: utf-8
import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("resultados.dat", delimiter = " ", skip_header = 0)
r = data[:,0]
velExp = data[:,1]
velModel =data[:,2]

plt.scatter(r,velExp)
plt.plot(r,velModel)
plt.savefig("grafica.png")
