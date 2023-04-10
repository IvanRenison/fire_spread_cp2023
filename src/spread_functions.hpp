#ifndef _SPREAD_FUNCTIONS_HPP
#define _SPREAD_FUNCTIONS_HPP

#include <vector>

#include "fires.hpp"
#include "landscape.hpp"

typedef struct _s_simulation_params {
  float independent_pred;
  float wind_pred;
  float elevation_pred;
  float slope_pred;
  float subalpine_pred;
  float wet_pred;
  float dry_pred;
  float fwi_pred;
  float aspect_pred;
} SimulationParams;

Fire simulate_fire(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    SimulationParams params, float distance, float elevation_mean, float elevation_sd,
    float upper_limit
);

SimulationParams random_params();

#endif