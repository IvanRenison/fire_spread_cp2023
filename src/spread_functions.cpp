#include "spread_functions.hpp"

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "fires.hpp"
#include "landscape.hpp"
#include "xoshiro256plus.hpp"

#ifdef BENCHMARKING
#include "wtime.hpp"
#endif

static inline float spread_probability(
    Cell* burning, Cell* neighbour, SimulationParams params, float angle, float distance,
    float elevation_mean, float elevation_sd, float upper_limit = 1.0
) {

  float elevation_diff = neighbour->elevation - burning->elevation;
  float distance_sq = distance * distance;
  float slope_term = elevation_diff / sqrtf(distance_sq + elevation_diff * elevation_diff);
  float wind_angle = angle - burning->wind_direction;
  float wind_term = cosf(wind_angle);
  float elev_diff_mean = neighbour->elevation - elevation_mean;
  float elev_term = elev_diff_mean / elevation_sd;

  float linpred = params.independent_pred;

  linpred += params.subalpine_pred * (neighbour->vegetation_type == SUBALPINE);
  linpred += params.wet_pred * (neighbour->vegetation_type == WET);
  linpred += params.dry_pred * (neighbour->vegetation_type == DRY);

  linpred += params.fwi_pred * neighbour->fwi;
  linpred += params.aspect_pred * neighbour->aspect;

  linpred += wind_term * params.wind_pred + elev_term * params.elevation_pred +
             slope_term * params.slope_pred;

  float prob = upper_limit / (1.0f + expf(-linpred));

  return prob;
}

Fire simulate_fire(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    SimulationParams params, float distance, float elevation_mean, float elevation_sd,
    float upper_limit = 1.0
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

#ifdef GRAPHICS
  std::vector<uint> burned_ids_steps;
  burned_ids_steps.push_back(end);
#endif

  int burning_size = end + 1;

  Matrix<char> burned_bin = Matrix<char>(n_col, n_row);

  for (int i = 0; i < end; i++) {
    uint cell_0 = ignition_cells[i].first;
    uint cell_1 = ignition_cells[i].second;
    burned_bin[{ cell_0, cell_1 }] = 1;
  }
  while (burning_size > 0) {
    int end_forward = end;
#ifdef BENCHMARKING
    double start_time, burned_cell_per_time, time_elapsed;
    start_time = 0.0;
    burned_cell_per_time = 0.0;

    start_time = wtime();
#endif

    // Loop over burning cells in the cycle

    // b is going to keep the position in burned_ids that have to be evaluated
    // in this burn cycle
    for (int b = start; b < end; b++) {
      uint burning_cell_0 = burned_ids[b].first;
      uint burning_cell_1 = burned_ids[b].second;

      Cell burning_cell = landscape[{ burning_cell_0, burning_cell_1 }];

      const int moves[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 },
                                { 0, 1 },   { 1, -1 }, { 1, 0 },  { 1, 1 } };
      const float angles[8] = { M_PI * 3 / 4, M_PI, M_PI * 5 / 4, M_PI / 2, M_PI * 3 / 2,
                                M_PI / 4,     0,    M_PI * 7 / 4 };

      uint neighbors_coords[2][8];

      for (int i = 0; i < 8; i++) {
        neighbors_coords[0][i] = burned_ids[b].first + moves[i][0];
        neighbors_coords[1][i] = burned_ids[b].second + moves[i][1];
      }
      // Note that in the case 0 - 1 we will have UINT_MAX

      bool out_of_range[8];
      for (int n = 0; n < 8; n++) {
        out_of_range[n] = neighbors_coords[0][n] >= n_col || // check rows
                          neighbors_coords[1][n] >= n_row    // check cols
            ;
      }

      Cell neighbour_cell[8];
      for (int n = 0; n < 8; n++) {
        if (!out_of_range[n]) {
          neighbour_cell[n] = landscape[{ neighbors_coords[0][n], neighbors_coords[1][n] }];
        } else {
          neighbour_cell[n] = Cell();
        }
      }

      bool burnable_cell[8];
      for (int n = 0; n < 8; n++) {
        if (!out_of_range[n]) {
          burnable_cell[n] = !burned_bin[{ neighbors_coords[0][n], neighbors_coords[1][n] }] &&
                             neighbour_cell[n].burnable;
        } else {
          burnable_cell[n] = false;
        }
      }

      float prob[8];
      for (int n = 0; n < 8; n++) {
        prob[n] = spread_probability(
            &burning_cell, &neighbour_cell[n], params, angles[n], distance, elevation_mean,
            elevation_sd, upper_limit
        );
      }

      bool burn[8];
      for (int n = 0; n < 8; n++) {
        if (!out_of_range[n]) {
          burn[n] = bernoulli(prob[n]);
        }
      }

      for (int n = 0; n < 8; n++) {

        uint neighbour_cell_0 = neighbors_coords[0][n];
        uint neighbour_cell_1 = neighbors_coords[1][n];

        // This is okey if n_row, n_col < UINT_MAX
        if (out_of_range[n])
          continue;

        if (!burnable_cell[n])
          continue;

        // simulate fire

        // Burn with probability prob (Bernoulli)

        if (!burn[n])
          continue;

        // If burned,
        // store id of recently burned cell and
        // set 1 in burned_bin
        // (but advance end_forward first)
        end_forward += 1;
        burned_ids.push_back({ neighbour_cell_0, neighbour_cell_1 });
        burned_bin[{ neighbour_cell_0, neighbour_cell_1 }] = true;

      } // end loop over neighbors_coords of burning cell b

    } // end loop over burning cells from this cycle
#ifdef BENCHMARKING
    time_elapsed = wtime() - start_time;
    burned_cell_per_time = burning_size / time_elapsed;
    std::cout.precision(17);
    std::cout << burned_cell_per_time << "," << burning_size << "," << time_elapsed
              << std::endl;
#endif
#ifdef GRAPHICS
    burned_ids_steps.push_back(end_forward);
#endif
    // update start and end
    start = end;
    end = end_forward;
    burning_size = end - start;

  } // end while

  Fire fire(n_col, n_row, burned_bin, burned_ids);

#ifdef GRAPHICS
  fire.burned_ids_steps = burned_ids_steps;
#endif

  return fire;
}

SimulationParams random_params() {
  return { (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX,
           (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX,
           (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX,
           (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX,
           (float)rand() / (float)RAND_MAX };
}
