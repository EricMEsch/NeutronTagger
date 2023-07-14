import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

x = np.loadtxt('/home/eric/sim/GdDesignStudies/Optics/PlatesOptics.txt', dtype=float, delimiter='\t', unpack=True)
xWater = np.loadtxt('/home/eric/sim/GdDesignStudies/Optics/WaterOptics.txt', dtype=float, delimiter='\t', unpack=True)
xdis = np.loadtxt('/home/eric/sim/GdDesignStudies/Optics/DissolvedOptics.txt', dtype=float, delimiter='\t', unpack=True)
xPMMA = np.loadtxt('/home/eric/sim/GdDesignStudies/Optics/PMMAOptics.txt', dtype=float, delimiter='\t', unpack=True)
xCurtain = np.loadtxt('/home/eric/sim/GdDesignStudies/Optics/CurtainOptics.txt', dtype=float, delimiter='\t', unpack=True)
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




fig, ax = plt.subplots()
ax.plot(thresh, sumup(xWater, thresh),  label='Water')
ax.plot(thresh, sumup(x, thresh),  label='GdPlates')
ax.plot(thresh, sumup(xdis, thresh),  label='Dissolved')
ax.plot(thresh, sumup(xPMMA, thresh),  label='20cm not so invisible PMMA')
ax.plot(thresh, sumup(xCurtain, thresh),  label='Gadolinium Curtain')
ax.set_xlabel('Required photons that reach the detector wall')
ax.set_ylabel('Percentage of detected Showers')
ax.set_title('Lambertian reflection, 0.9 reflection, without absorption')
ax.legend()
ax.grid()


plt.show()