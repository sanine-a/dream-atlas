#include "icosphere.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// point-conversion functions

cartesian_point cartesian( spherical_point pt_s ) {
  cartesian_point pt_c;

  pt_c.x = pt_s.r * cos(pt_s.theta) * sin(pt_s.phi);
  pt_c.y = pt_s.r * cos(pt_s.theta) * cos(pt_s.phi);
  pt_c.z = pt_s.r * sin(pt_s.theta);

  return pt_c;
}

spherical_point spherical( cartesian_point pt_c ) {
  spherical_point pt_s;

  pt_s.r = sqrt( pt_c.x*pt_c.x + pt_c.y*pt_c.y + pt_c.z*pt_c.z );
  pt_s.theta = atan2(pt_c.y, pt_c.x);
  pt_s.phi = asin( pt_c.z/pt_s.r );

  return pt_s;
}

// subdivide an edge

void subdivide_edge( struct edge* e ) {
  cartesian_point p1 = cartesian( *(e->p1) );
  cartesian_point p2 = cartesian( *(e->p2) );
  cartesian_point m;
  m.x = 0.5*(p1.x + p2.x);
  m.y = 0.5*(p1.y + p2.y);
  m.z = 0.5*(p1.z + p2.z);

  spherical_point* mid = malloc( sizeof(spherical_point) );
  assert( mid != NULL );
  *mid = spherical(m);

  e->e1 = malloc(sizeof(struct edge));
  e->e2 = malloc(sizeof(struct edge));

  assert( e->e1 != NULL && e->e2 != NULL );

  // new edge 1
  e->e1->p1 = e->p1;
  e->e1->p2 = mid;

  // new edge 2
  e->e2->p1 = e->p2;
  e->e2->p2 = mid;    
}


// subdivide a face

struct face* subdivide_face( struct face f ) {
  /* if we label the vertices in e1 as p1=1 and p2=2,
     with some third vertex out there, there are eight possible cases:

     0. e2 is (2,3) and e3 is (3,1)
     1. e2 is (2,3) and e3 is (1,3)
     2. e2 is (3,2) and e3 is (3,1)
     3. e2 is (3,2) and e3 is (1,3)
     4. e3 is (2,3) and e2 is (3,1)
     5. e3 is (2,3) and e2 is (1,3)
     6. e3 is (3,2) and e2 is (3,1)
     7. e3 is (3,2) and e2 is (1,3)

     the function tests for which case we are in, and then swaps pointers
     around so that we wind up in case 0. 
  */
  
  struct edge* e1 = f.e1;
  struct edge* e2 = f.e2;
  struct edge* e3 = f.e3;  
  

  if ( e1->p1 == e2->p1 || e1->p1 == e2->p2 ) {
    // state is in 4-7, so swap e2 and e3
    struct edge* tmp = e2;
    e2 = e3;
    e3 = tmp;
  }

  if ( e1->p2 == e2->p1 ) {
    // case 2 or 3, swap e2's points
    spherical_point* tmp_point = e2->p1;
    struct edge* tmp_edge = e2->e1;

    e2->p1 = e2->p2;
    e2->p2 = tmp_point;

    e2->e1 = e2->e2;
    e2->e2 = tmp_edge;
  }

  if ( e1->p1 == e3->p1 ) {
    // case 1, swap e3's points
    spherical_point* tmp_point = e3->p1;
    struct edge* tmp_edge = e3->e1;

    e3->p1 = e3->p2;
    e3->p2 = tmp_point;

    e3->e1 = e3->e2;
    e3->e2 = tmp_edge;
  }

  // now in case 0, we can subdivide the face properly

  // build central edges

  struct edge* center_edges[3];
  for (int i=0; i<3; i++) {
    center_edges[i] = malloc(sizeof(struct edge));
    assert(center_edges[i] != NULL);
  }

  center_edges[0]->p1 = e1->e1->p2;
  center_edges[0]->p2 = e2->e1->p2;
  
  center_edges[1]->p1 = e2->e1->p2;
  center_edges[1]->p2 = e3->e1->p2;
  
  center_edges[2]->p1 = e3->e1->p2;
  center_edges[2]->p2 = e1->e1->p2;

  // build the new faces
  
  struct face* result = malloc(4*sizeof(struct face));
  assert( result != NULL );

  result[0].e1 = e1->e1;
  result[0].e2 = center_edges[2];
  result[0].e3 = e3->e2;

  result[1].e1 = e1->e2;
  result[1].e2 = center_edges[0];
  result[1].e3 = e2->e1;

  result[2].e1 = e2->e2;
  result[2].e2 = center_edges[1];
  result[2].e3 = e3->e1;

  result[3].e1 = center_edges[0];
  result[3].e2 = center_edges[1];
  result[3].e3 = center_edges[2];

  return result;
}
