#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifndef ICOSPHERE_H
#define ICOSPHERE_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// points

typedef struct {
  double x;
  double y;
  double z;
} cartesian_point;

typedef struct {
  double r;
  double theta;
  double phi;
  int index;
} spherical_point;

// point-conversion functions
cartesian_point cartesian( spherical_point );
spherical_point spherical( cartesian_point );

// a metric
double distance(spherical_point, spherical_point);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// graph structures

typedef struct {
  spherical_point pt;
  int neighbors[6];
} node;

typedef struct {
  int size;
  node* nodes;
} graph;

// graph-building
graph icosphere(int subdivisions);

#endif
