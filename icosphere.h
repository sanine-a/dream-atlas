#include "math.h"
#include "stdio.h"

#ifndef ICOSPHERE_H
#define ICOSPHERE_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// point structures

typedef struct {
  double x;
  double y;
  double z;
} cartesian_point;

typedef struct {
  double r;
  double theta;
  double phi;
} spherical_point;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// point-conversion functions

cartesian_point spherical_to_cartesian( spherical_point );
spherical_point cartesian_to_spherical( cartesian_point );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// node structure

struct node;

typedef struct {
  spherical_point pt;
  struct node** neighbors;
} node;

#endif
