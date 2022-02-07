import numpy as np
import matplotlib.pyplot as plt

def integer_to_three_digits(i):
    if i < 10:
        return "00" + str(i)
    if i < 100:
        return "0" + str(i)
    return str(i)

for t in range(1, 4, 2):
    filename = "cost-heur" + integer_to_three_digits(t) + ".txt"
    f = open(filename, 'r')
    num_plots, operations = list(map(int, f.readline().split()))
    Y = np.array(np.loadtxt(filename, skiprows=1))
    print(Y)
    rows, columns = np.shape(Y)
    print(rows)
    print(columns)

    fig, axs = plt.subplots(num_plots, 1)
    x = np.arange(0, operations+1)
    for plot in range(num_plots):
        axs[plot].set_title("iteration " + str(plot))
        axs[plot].plot(x, Y[plot*(operations+1):(plot+1)*(operations+1),0])
    fig.tight_layout(pad=0.6)
    #plt.legend([0, 1, 2, 3, 4, 5], loc="upper left")
    plt.suptitle("Cost change over run of ALNS algorithm")
    plt.xlabel("operations")
    plt.ylabel("percent weight")
    plt.show()

