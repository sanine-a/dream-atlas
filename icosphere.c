#include "icosphere.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// point-conversion functions

cartesian_point spherical_to_cartesian( spherical_point pt_s ) {
  cartesian_point pt_c;

  pt_c.x = pt_s.r * cos(pt_s.theta) * sin(pt_s.phi);
  pt_c.y = pt_s.r * cos(pt_s.theta) * cos(pt_s.phi);
  pt_c.z = pt_s.r * sin(pt_s.theta);

  return pt_c;
}

spherical_point cartesian_to_spherical( cartesian_point pt_c ) {
  spherical_point pt_s;

  pt_s.r = sqrt( pt_c.x*pt_c.x + pt_c.y*pt_c.y + pt_c.z*pt_c.z );
  pt_s.theta = atan2(pt_c.y, pt_c.x);
  pt_s.phi = asin( pt_c.z/pt_s.r );

  return pt_s;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

