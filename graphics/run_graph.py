from datetime import datetime
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

from typing import List, Tuple

def animate_fire(width: int, height: int, steps: List[List[Tuple[int, int]]]):
    """
    Animates the fire spread
    Each element of steps is a list of coordinates of burning cells in that step

    The burning cells in each step are plotted in red
    The already burned cells are plotted in black
    The rest of the cells are plotted in green
    """

    burned_cells_steps: np.ndarray = np.full((width, height), -1)
    for i, step in enumerate(steps):
        for x, y in step:
            burned_cells_steps[x, y] = i


    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xlim(0, width)
    ax.set_ylim(0, height)
    ax.set_aspect('equal')
    ax.set_xticks([])
    ax.set_yticks([])

    def animate(i):
        ax.set_title(f"Step {i}")
        for x in range(width):
            for y in range(height):
                if burned_cells_steps[x, y] == -1 or burned_cells_steps[x, y] > i:
                    ax.add_patch(plt.Rectangle((x, y), 1, 1, fc='g'))
                elif burned_cells_steps[x, y] < i:
                    ax.add_patch(plt.Rectangle((x, y), 1, 1, fc='k'))
                elif burned_cells_steps[x, y] == i:
                    ax.add_patch(plt.Rectangle((x, y), 1, 1, fc='r'))

    anim = animation.FuncAnimation(fig, animate, frames=len(steps))
    anim.save('fire.mp4', fps=5)


def read_fire() -> Tuple[int, int, List[List[Tuple[int, int]]]]:
    """
    Reads the fire from standard input
    Example input:
    Landscape size: 5 5
    Step 1:
    0 0
    Step 2:
    0 1
    1 0
    Step 3:
    0 2
    1 2
    1 1
    """
    _, _, width, height = input().split()
    width, height = int(width), int(height)
    steps: List[List[Tuple[int, int]]] = []
    while True:
        try:
            line: str = input()
        except EOFError:
            break
        if line == "":
            break
        elif line[0:5] == "Step ":
            steps.append([])
        else:
            x, y = line.split()
            steps[-1].append((int(x), int(y)))
    return width, height, steps


def main():
    width, height, steps = read_fire()
    animate_fire(width, height, steps)


if __name__ == "__main__":
    main()
