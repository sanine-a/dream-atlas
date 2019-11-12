#include <stdlib.h>
#include <assert.h>
#include "icosphere.h"

#ifndef WORLD_H
#define WORLD_H

double randm();
void jitter_nodes(graph g, double max_r, double max_theta, double max_phi);

typedef struct {
  graph terrain;
  double sealevel;
} world;

world make_world(double base_radius, int graph_subdivisions, int terrain_iterations, double terrain_max_step, int terrain_max_radius);

#endif
