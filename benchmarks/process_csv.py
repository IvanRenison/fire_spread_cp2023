import sys
import pandas as pd
import matplotlib.pyplot as plt


def plotGraph(name):
    plt.legend()
    plt.title(name)
    plt.xlabel('Burned Cells per seconds')
    plt.ylabel('Frequency')
    plt.grid(axis='y', alpha=0.75)
    plt.show()


if (len(sys.argv) == 1):
    df = pd.read_csv(sys.stdin, names=[
        'burned_cell_per_time', 'amount_of_cells', 'elapsed'])
    print(df.describe().apply(lambda s: s.apply('{0:.5f}'.format)))

    df['burned_cell_per_time'].hist(
        grid=True, bins=1000, rwidth=0.9, color='#607c8e')
    plotGraph(sys.stdin.name)


else:
    for file in sys.argv[1:]:
        df = pd.read_csv(file, names=[
            'burned_cell_per_time', 'amount_of_cells', 'elapsed'], usecols=['burned_cell_per_time'])
        plt.hist(df['burned_cell_per_time'], alpha=0.5,
                 label= file.replace('csv', '').replace('.', '').replace('/', '').replace('_', ' '), bins=100, rwidth=0.75)
    plotGraph('ehhhhhhh')
