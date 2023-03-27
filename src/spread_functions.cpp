#include "spread_functions.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <vector>

#include "fires.hpp"
#include "landscape.hpp"

double spread_probability(
    Cell* burning, Cell* neighbour, SimulationParams params, double angle, double distance,
    double elevation_mean, double elevation_sd, double upper_limit = 1.0
) {

  double slope_term = sin(atan((neighbour->elevation - burning->elevation) / distance));
  double wind_term = cos(angle - burning->wind_direction);
  double elev_term = (neighbour->elevation - elevation_mean) / elevation_sd;

  double linpred = params.independent_pred;

  linpred += params.subalpine_pred * neighbour->subalpine;
  linpred += params.wet_pred * neighbour->wet;
  linpred += params.dry_pred * neighbour->dry;

  linpred += params.fwi_pred * neighbour->fwi;
  linpred += params.aspect_pred * neighbour->aspect;

  linpred += wind_term * params.wind_pred + elev_term * params.elevation_pred +
             slope_term * params.slope_pred;

  double prob = upper_limit / (1 + exp(-linpred));

  return prob;
}

Fire simulate_fire(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    SimulationParams params, double distance, double elevation_mean, double elevation_sd,
    double upper_limit = 1.0
) {

  uint n_row = landscape.height;
  uint n_col = landscape.width;
  uint n_cell = n_row * n_col;

  std::vector<std::pair<uint, uint>> burned_ids;
  burned_ids.reserve(n_cell);

  int start = 0;
  int end = ignition_cells.size();

  for (int i = 0; i < end; i++) {
    burned_ids.push_back(ignition_cells[i]);
  }

  int burning_size = end + 1;

  Matrix<bool> burned_bin = Matrix<bool>(n_col, n_row);

  for (int i = 0; i < end; i++) {
    uint cell_0 = ignition_cells[i].first;
    uint cell_1 = ignition_cells[i].second;
    burned_bin[cell_0, cell_1] = 1;
  }

  while (burning_size > 0) {
    int end_forward = end;

    // Loop over burning cells in the cycle

    // b is going to keep the position in burned_ids that have to be evaluated
    // in this burn cycle
    for (int b = start; b < end; b++) {
      uint burning_cell_0 = burned_ids[b].first;
      uint burning_cell_1 = burned_ids[b].second;

      Cell* burning_cell = landscape[burning_cell_0, burning_cell_1];

      const int moves[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 },
                                { 0, 1 },   { 1, -1 }, { 1, 0 },  { 1, 1 } };

      uint neighbours_coords[2][8];

      for (int i = 0; i < 8; i++) {
        neighbours_coords[0][i] = burned_ids[b].first + moves[i][0];
        neighbours_coords[1][i] = burned_ids[b].second + moves[i][1];
      }
      // Note that in the case 0 - 1 we will have UINT_MAX

      // Loop over neighbours_coords of the focal burning cell

      for (int n = 0; n < 8; n++) {

        uint neighbour_cell_0 = neighbours_coords[0][n];
        uint neighbour_cell_1 = neighbours_coords[1][n];

        // Is the cell in range?
        bool out_of_range = neighbour_cell_0 >= n_col || // check rows
                            neighbour_cell_1 >= n_row    // check cols
            ;
        // This is okey if n_row, n_col < UINT_MAX
        if (out_of_range)
          continue;

        Cell* neighbour_cell = landscape[neighbour_cell_0, neighbour_cell_1];

        // Is the cell burnable?
        bool burnable_cell =
            !burned_bin[neighbour_cell_0, neighbour_cell_1] && neighbour_cell->burnable;

        if (!burnable_cell)
          continue;

        const double angles[8] = { M_PI * 3 / 4, M_PI, M_PI * 5 / 4, M_PI / 2, M_PI * 3 / 2,
                                   M_PI / 4,     0,    M_PI * 7 / 4 };

        // simulate fire
        double prob = spread_probability(
            burning_cell, neighbour_cell, params, angles[n], distance, elevation_mean,
            elevation_sd, upper_limit
        );

        // Burn with probability prob (Bernoulli)
        bool burn = (double)rand() / (double)RAND_MAX < prob;

        if (burn == 0)
          continue;

        // If burned,
        // store id of recently burned cell and
        // set 1 in burned_bin
        // (but advance end_forward first)
        end_forward += 1;
        burned_ids.push_back({ neighbour_cell_0, neighbour_cell_1 });
        burned_bin[neighbour_cell_0, neighbour_cell_1] = true;

      } // end loop over neighbours_coords of burning cell b

    } // end loop over burning cells from this cycle

    // update start and end
    start = end;
    end = end_forward;
    burning_size = end - start;

  } // end while

  return { n_col, n_row, burned_bin, burned_ids };
}
