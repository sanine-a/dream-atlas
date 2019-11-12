#include "world.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// get random double between 0 and 1
double randm() {
  return (double)(rand())/RAND_MAX;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void jitter_nodes(graph g, double max_r, double max_theta, double max_phi) {
  for (int i=0; i<g.size; i++) {
    double delta_r = 2*max_r*randm() - max_r;
    double delta_theta = 2*max_theta*randm() - max_theta;
    double delta_phi = 2*max_phi*randm() - max_phi;

    g.nodes[i].pt.r += delta_r;
    g.nodes[i].pt.theta += delta_theta;
    g.nodes[i].pt.phi += delta_phi;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// build a world
world make_world(double base_radius, int graph_subdivisions, int terrain_iterations, double terrain_max_step, int terrain_max_radius) {
  world w;

  w.terrain = icosphere(graph_subdivisions);
  for (int i=0; i<w.terrain.size; i++) {
    // set terrain height to base radius
    w.terrain.nodes[i].pt.r = base_radius;
  }

  // jitter nodes
  double phi = (1+sqrt(5))/2;
  double alpha0 = atan2(1,phi);
  double alpha = alpha0/pow(2,graph_subdivisions+1);
  jitter_nodes(w.terrain, 0, alpha, alpha);

  // build random terrain
  for (int i=0; i<terrain_iterations; i++) {
    // pick random node
    int node0 = rand()%w.terrain.size;

    double step = randm()*terrain_max_step;
    double radius = randm()*terrain_max_radius;

    for (int j=0; j<w.terrain.size; j++) {
      if ( distance( w.terrain.nodes[node0].pt, w.terrain.nodes[j].pt ) <= radius ) {
	w.terrain.nodes[j].pt.r += step;
      }
    }
  }

  return w;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
