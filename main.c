#include <stdio.h>
#include <cairo.h>
#include "icosphere.h"

void print_face( struct face );

int main() {
  spherical_point p1, p2, p3;
  {
    cartesian_point tmp;
    tmp.x = 0; tmp.y = 0; tmp.z = 1;
    p1 = spherical(tmp);
    tmp.x = 1;
    p2 = spherical(tmp);
    tmp.x = 0; tmp.y = 1;
    p3 = spherical(tmp);
  }

  struct edge e1, e2, e3;
  e1.p1 = &p1;
  e1.p2 = &p2;

  e2.p1 = &p2;
  e2.p2 = &p3;

  e3.p1 = &p3;
  e3.p2 = &p1;

  subdivide_edge(&e1);
  subdivide_edge(&e2);
  subdivide_edge(&e3);  

  struct face f;
  f.e1 = &e1;
  f.e2 = &e2;
  f.e3 = &e3;

  print_face(f);
  printf("\n\n");

  struct face* faces = subdivide_face(f);

  for (int i=0; i<4; i++) {
    printf("%d: ",i); print_face(faces[i]);
  }

  return 0;
}

void print_face( struct face f ) {
  cartesian_point p1, p2;
  p1 = cartesian( *(f.e1->p1) );
  p2 = cartesian( *(f.e1->p2) );

  printf("(%2f, %2f) -> (%2f, %2f)    ",p1.x,p1.y,p2.x,p2.y);

  p1 = cartesian( *(f.e2->p1) );
  p2 = cartesian( *(f.e2->p2) );

  printf("(%2f, %2f) -> (%2f, %2f)    ",p1.x,p1.y,p2.x,p2.y);

  p1 = cartesian( *(f.e3->p1) );
  p2 = cartesian( *(f.e3->p2) );

  printf("(%2f, %2f) -> (%2f, %2f)\n",p1.x,p1.y,p2.x,p2.y);
}
