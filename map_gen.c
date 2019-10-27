#include "map_gen.h"

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
    
    
    
