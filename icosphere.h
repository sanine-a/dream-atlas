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
} spherical_point;

// point-conversion functions

cartesian_point cartesian( spherical_point );
spherical_point spherical( cartesian_point );

// edges

struct edge {
  spherical_point* p1;
  spherical_point* p2;
  struct edge* e1;
  struct edge* e2;
};

void subdivide_edge( struct edge* );

// faces

struct face {
  struct edge* e1;
  struct edge* e2;
  struct edge* e3;
};

struct face* subdivide_face( struct face );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// node structure

typedef struct {
  spherical_point pt;
  struct node* neighbors[6];
} node;

#endif
