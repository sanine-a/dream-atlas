#include "cartography.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cartesian_point orthographic(spherical_point pt, double theta0, double phi0) {
  cartesian_point result;
  result.x = cos(pt.phi)*sin(pt.theta - theta0);
  result.y = cos(phi0)*sin(pt.phi) - sin(phi0)*cos(pt.phi)*cos(pt.theta - theta0);
  result.z = 0;
  return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// render orthographic projection
void render_orthographic(world w, cairo_t* cr, double theta0, double phi0, int width, int height, map_type mp) {
  double sin_phi0 = sin(phi0);
  double cos_phi0 = cos(phi0);
  double half_width = 0.5*width;
  double half_height = 0.5*height;

  // get projections of all points
  cartesian_point* projected = malloc( w.terrain.size*sizeof(cartesian_point) );
  assert(projected != NULL);
  for (int i=0; i<w.terrain.size; i++) {

    spherical_point pt = w.terrain.nodes[i].pt;
    if ( sin_phi0*sin(pt.phi) + cos_phi0*cos(pt.phi)*cos(pt.theta-theta0) < 0 ) {
      // point is on the far side of the planet, clip it out
      projected[i].z = -1;
    }
    else {
      projected[i] = orthographic(pt,theta0,phi0);
    }
  }
  
  switch(mp) {
  case RAW_MAP:
    // draw the raw graph
    for (int i=0; i<w.terrain.size; i++) {
      if (projected[i].z != -1) {
	for (int j=0; j<6; j++) {
	  int neighbor = w.terrain.nodes[i].neighbors[j];
	  if ( neighbor > i && projected[neighbor].z != -1 ) {
	    cairo_move_to(cr, half_width*(projected[i].x+1), half_height*(1-projected[i].y));
	    cairo_line_to(cr, half_width*(projected[neighbor].x+1), half_height*(1-projected[neighbor].y));
	    cairo_stroke(cr);
	  }
	}
      }
    }
    break;
  default:
    //do nothing
    break;
  }

  free(projected);
}
