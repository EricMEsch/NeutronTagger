import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

plt.rc('font', size=12)
plt.rc('axes', titlesize=20)
plt.rc('axes', labelsize=20)

thresh = np.linspace(1,3000, 300)


def sumup(x, thresh):
    y = np.array([])
    

    for i in thresh:
        temp = 0
        for j in x:
            if j >= i:
                temp = temp + 1
        y = np.append(y,temp / 10193)
    return y


def ReadIn(i, j):
    path = f"/home/eric/sim/GdDesignStudiesBuild/PMMAautoEva/PMMAwidth{j}dist{i}.csv"
    x = np.loadtxt(path, dtype=float, delimiter='\t', skiprows=5, unpack=True)
    return sumup(x, thresh)
dists = np.array([0,2,4,6,8,10])
widths = np.array([1,3,5,7,9,11,13,15])

fig, ax = plt.subplots()
for i in dists:
    for j in widths:
        ax.plot(thresh, ReadIn(i,j),  label=f'width {j} dist {i}')


ax.set_xlabel('Required photons that reach the detector wall')
ax.set_ylabel('Percentage of detected Showers')
ax.set_title('Lambertian reflection, 0.9 reflection, without absorption')
ax.legend()
ax.grid()


plt.show()