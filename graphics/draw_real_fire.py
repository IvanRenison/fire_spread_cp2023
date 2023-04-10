import csv
import numpy as np
import matplotlib.pyplot as plt
import sys
from typing import List, Tuple

def read_burned_ids(metadata_file_csv: str, burned_ids_file_csv: str) -> Tuple[int, int, List[Tuple[int, int]]]:
    """
    Metadata csv example:
        "size_rows","size_cols","counts_veg_shrubland","counts_veg_subalpine","counts_veg_wet","counts_veg_dry"
        5,5,4,6,3,1
    Burned ids csv example:
        "row_id","col_id"
        1,1
        1,0
        2,0
        0,0
        2,1
        3,0
        2,2
    """

    # Read size_rows and size_cols from metadata file
    with open(metadata_file_csv, "r") as metadata_file:
        metadata_reader = csv.reader(metadata_file)
        next(metadata_reader)
        size_rows, size_cols, *_ = next(metadata_reader)
        size_rows, size_cols = int(size_rows), int(size_cols)

    # Read burned ids from burned ids file
    burned_ids: List[Tuple[int, int]] = []
    with open(burned_ids_file_csv, "r") as burned_ids_file:
        burned_ids_reader = csv.reader(burned_ids_file)
        next(burned_ids_reader)
        for row_id, col_id in burned_ids_reader:
            burned_ids.append((int(row_id), int(col_id)))

    return size_rows, size_cols, burned_ids


def draw_real_fire(size_rows: int, size_cols: int, burned_ids: List[Tuple[int, int]], output_filename: str):
    """
    Generates a plot of the burned probabilities
    Colors:
    - Red: 100% probability
    - Green: 0% probability
    """
    burned_amounts: np.ndarray = np.zeros((size_rows, size_cols, 3), dtype=int)
    # Fill with green
    burned_amounts[:, :] = [0, 255, 0]
    for row_id, col_id in burned_ids:
        burned_amounts[row_id, col_id] = [255, 0, 0]
    im = plt.imshow(burned_amounts)
    plt.savefig(output_filename)


def main():
    if len(sys.argv) not in [2, 3]:
        print(f"Usage: {sys.argv[0]} <fire_path> [<output>]")
    fire_path: str = sys.argv[1]
    output_file: str = sys.argv[2] if len(sys.argv) == 3 else "real_fire.png"

    metadata_file: str = fire_path + "-metadata.csv"
    burned_ids_file: str = fire_path + "-burned_ids.csv"

    size_rows, size_cols, burned_ids = read_burned_ids(metadata_file, burned_ids_file)
    draw_real_fire(size_rows, size_cols, burned_ids, output_file)


if __name__ == '__main__':
    main()



