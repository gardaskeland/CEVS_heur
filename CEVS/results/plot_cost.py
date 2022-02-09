import numpy as np
import matplotlib.pyplot as plt

def integer_to_three_digits(i):
    if i < 10:
        return "00" + str(i)
    if i < 100:
        return "0" + str(i)
    return str(i)
ylimlist = [0, [0, 30], 0, [800, 1500], 0, [1300, 2500], 0, [2200, 4000], 0, [400, 700]]
cdict = {0: "blue", 1: "lightblue", 2: "lightgreen", 3: "green", 4: "purple", 5: "pink"}
for t in range(1, 10, 2):
    filename = "cost-heur" + integer_to_three_digits(t) + ".txt"
    f = open(filename, 'r')
    num_plots, operations = list(map(int, f.readline().split()))
    Y = np.array(np.loadtxt(filename, skiprows=1))
    print(Y)
    rows, columns = np.shape(Y)
    print(rows)
    print(columns)

    fig, axs = plt.subplots(num_plots, 1)
    manager = plt.get_current_fig_manager()
    manager.full_screen_toggle()
    x = np.arange(0, operations+1)
    for plot in range(num_plots):
        group = np.array(Y[plot*(operations+1):(plot+1)*(operations+1)-1,1])
        axs[plot].set_title("iteration " + str(plot))
        axs[plot].set_ylim(ylimlist[t])
        for g in np.unique(group):
            ix = np.where(group == g)
            ix_1 = tuple(map(lambda x: x + 1, ix))
            axs[plot].scatter(x[ix], Y[plot*(operations+1)+1:(plot+1)*(operations+1),0][ix],
            c=cdict[g], label=g, s=30)
    axs[num_plots // 2].legend(["random choice add", "random choice split", "weighted random merge", 
                                "label propagation round", "remove nodes", "add node to all"], loc="left")
    fig.tight_layout(pad=0.6)
    #plt.legend([0, 1, 2, 3, 4, 5], loc="upper left")
    plt.suptitle("Cost change over runs of ALNS algorithm on problem heur" + integer_to_three_digits(t))
    plt.xlabel("operations")
    plt.ylabel("cost")
    plt.show()

