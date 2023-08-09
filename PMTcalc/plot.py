import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
import math


plt.rc('font', size=12)
plt.rc('axes', titlesize=20)
plt.rc('axes', labelsize=20)


totalDets = np.array([400, 500, 600])
multplicity = np.arange(1, 100)

def dark_rate(F, N, D, t):
    log_numer = math.log(np.math.factorial(F))
    log_denom = math.log(np.math.factorial(N))
    log_denom2 = math.log(np.math.factorial(F-N))
    return np.exp(log_numer - log_denom - log_denom2 + N * math.log(D) + (N - 1) * math.log(t))


def arrayitup(F,D,t):
    y = np.array([])

    for i in multplicity:
        y = np.append(y,dark_rate(F,i,D,t))
    
    return y

def accumulate_sum(y):
    n = len(y)
    x = np.zeros(n)  # Initialize the result array with zeros

    # Iterate through the array in reverse and accumulate the sum
    sum_so_far = 0
    for i in range(n - 1, -1, -1):
        sum_so_far += y[i]
        x[i] = sum_so_far

    return x




fig, ax = plt.subplots()
ax.plot(multplicity, accumulate_sum(arrayitup(400,5000,0.00001)),  label=r'400 PMTs, 1$\mu s$ Intervall')
ax.plot(multplicity, accumulate_sum(arrayitup(500,5000,0.00001)),  label=r'500 PMTs, 1$\mu s$ Intervall')
ax.plot(multplicity, accumulate_sum(arrayitup(600,5000,0.00001)),  label=r'600 PMTs, 1$\mu s$ Intervall')
ax.set_yscale("log")
ax.set_xlabel('At least Multiplicity N')
ax.set_ylabel('Frequency in Hz')
ax.set_title('Random coincidence rate')
ax.legend()
ax.grid()


plt.show()