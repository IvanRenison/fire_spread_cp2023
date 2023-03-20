#ifndef _SPREAD_FUNCTIONS_HPP
#define _SPREAD_FUNCTIONS_HPP

#include <vector>

#include "fires.hpp"
#include "landscape.hpp"

typedef struct _s_simulation_params {
  double independent_pred;
  double wind_pred;
  double elevation_pred;
  double slope_pred;
  double subalpine_pred;
  double wet_pred;
  double dry_pred;
  double fwi_pred;
  double aspect_pred;
} SimulationParams;

Fire simulate_fire(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    SimulationParams params, double distance, double elevation_mean, double elevation_sd,
    double upper_limit
);

#endif