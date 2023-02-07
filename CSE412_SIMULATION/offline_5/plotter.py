import matplotlib.pyplot as plt
import csv
import sys

if __name__ == "__main__":
    filename = sys.argv[1]
    s = int(filename[2:-4])
    graph_title = "Simulation Results for n = 100, s = " + str(s)

    reader = csv.reader(open(filename, "r"), delimiter=",")
    xs = []
    ys = []

    for row in reader:
        xs.append(int(row[0]))
        ys.append(float(row[1]))

    fig=plt.figure()
    plt.style.use("ggplot")

    plt.plot(xs, ys, color="red")
    plt.xlabel("Sample Size")
    plt.ylabel("Success Percentage")
    plt.title(graph_title)


    plt.yticks(range(0, 101, 10))
    plt.xticks(range(xs[0], xs[-1], 10))

    plt.show()

    save = input("Save graph? (y/n): ")
    if save != "y":
        exit()

    filename = filename[:-4] + ".png"
    fig.savefig(filename)

    print("Graph saved as", filename)