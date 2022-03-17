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

    fig, axs = plt.subplots(num_plots // 2, 1)
    manager = plt.get_current_fig_manager()
    manager.full_screen_toggle()
    x = np.arange(0, operations+1)
    for plot in range(num_plots // 2):
        group = np.array(Y[plot*(operations+1):(plot+1)*(operations+1)-1,1])
        plot_y = Y[plot*(operations+1):(plot+1)*(operations+1),0]
        y = np.array([plot_y[i - 1] - plot_y[i] for i in range(1, operations+1)])
        for g in np.unique(group):
            ix = np.where(group == g)
            axs.scatter(x[ix], y[ix],
            c=cdict[g], label=g, s=10, marker="^")
    fig.legend(["add_node_to_neighbours", "label_propagation", "remove node", 
                "add_node_to_set", "label_propagation_wr", "fast_merge"], loc="upper left")
    fig.tight_layout(pad=0.6)
    #plt.legend([0, 1, 2, 3, 4, 5], loc="upper left")
    plt.suptitle("Cost change over runs of ALNS algorithm on problem heur" + integer_to_three_digits(t))
    plt.xlabel("operations")
    plt.ylabel("cost")
    plt.show()

