import sys
import pandas as pd
import matplotlib.pyplot as plt
from os.path import commonprefix


def plotGraph(name):
    plt.setp(ax.xaxis.get_majorticklabels(), rotation=10, ha="right")
    plt.gcf().subplots_adjust(bottom=0.15)
    # plt.legend()
    plt.title(f'Compillation flags comparasion: {name}')
    # plt.xlabel('Burned Cells per seconds')
    plt.ylabel('Burned Cells per seconds')
    plt.grid(axis='y', alpha=0.75)
    name = name.replace(' ','')
    plt.savefig(fname=f'./csv/base_atom/fig/{name}.svg', dpi=300, format='svg')
    #plt.show()

if (len(sys.argv) == 1):
    df = pd.read_csv(sys.stdin, names=[
        'burned_cell_per_time', 'amount_of_cells', 'elapsed'])
    print(df.describe().apply(lambda s: s.apply('{0:.5f}'.format)))

    df['burned_cell_per_time'].hist(
        grid=True, bins=1000, rwidth=0.9, color='#607c8e')
    plotGraph(sys.stdin.name)


else:
    fig, ax = plt.subplots()
    fig.set_size_inches(14, 7)
    prefix_string = commonprefix(sys.argv[1:])
    suffix_string = commonprefix([file[::-1] for file in sys.argv[1:]])[::-1]
    for file in sys.argv[1:]:
        df = pd.read_csv(file, names=[
            'burned_cell_per_time', 'amount_of_cells', 'elapsed'], usecols=['burned_cell_per_time'])
        # plt.hist(df['burned_cell_per_time'], alpha=0.5,
        #          label= file.replace('csv', '').replace('.', '').replace('/', '').replace('_', ' '), bins=100, rwidth=0.75)
        plt.bar(file.removeprefix(prefix_string).removesuffix(suffix_string) or 'base' ,
                df['burned_cell_per_time'].mean(), yerr=df['burned_cell_per_time'].std())
    plotGraph(''.join(prefix_string.split('/')[-1])+ ' ' + suffix_string.removesuffix('.csv').removeprefix('_'))