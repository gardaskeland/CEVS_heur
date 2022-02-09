import numpy as np
import matplotlib.pyplot as plt

def integer_to_three_digits(i):
    if i < 10:
        return "00" + str(i)
    if i < 100:
        return "0" + str(i)
    return str(i)

for t in range(1, 10, 2):
    filename = "weights-heur" + integer_to_three_digits(t) + ".txt"
    f = open(filename, 'r')
    num_plots, operations, weight_change_after, num_weight_changes = list(map(int, f.readline().split()))
    Y = np.array(np.loadtxt(filename, skiprows=1))
    print(Y)
    rows, columns = np.shape(Y)
    print(rows)
    print(columns)
    print(Y[2, 3])

    fig, axs = plt.subplots(num_plots, 1)
    manager = plt.get_current_fig_manager()
    manager.full_screen_toggle()
    x = np.arange(0, operations, weight_change_after)
    for plot in range(num_plots):
        axs[plot].set_title("iteration " + str(plot))
        for i in range(plot*num_weight_changes, (plot + 1)*num_weight_changes, num_weight_changes):
            for j in range(columns):
                print(i)
                print(j)
                axs[plot].plot(x, Y[i:i+num_weight_changes,j])
    fig.tight_layout(pad=0.6)
    axs[num_plots // 2].legend(["random choice add", "random choice split", "weighted random merge", 
                                "label propagation round", "remove nodes", "add node to all"], loc="left")
    plt.suptitle("Weights over " + str(operations) + " operations for 5 iterations on problem " + integer_to_three_digits(t))
    #ttl = plt.suptitle("Weights change for operations 0-6 of alns on heur" + integer_to_three_digits(t))
    #ttl.set_position([20, 20])
    plt.xlabel("operations")
    plt.ylabel("percent weight")
    plt.show(); #plt.savefig("weights-heur" + integer_to_three_digits(t) + ".png")

