#include <stdio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-svg.h>
#include "icosphere.h"
#include "map_gen.h"

void print_graph(graph g);

int main() {
  graph ico = icosphere(1);

  //print_graph(ico);
  printf("%d\n",ico.size);

  for (int i=0; i<ico.size; i++) {
    ico.nodes[i].pt.r = 1;
  }

  jitter_nodes(ico,0,M_PI/20,M_PI/20);
  
  cairo_surface_t* surface;
  cairo_t* cr;

  FILE* file;

  file = fopen( "icosphere.svg", "w" );
  if ( file == NULL ) {
    fprintf(stderr, "failed to open file\n");
    return -1;
  }
  
  surface = cairo_svg_surface_create("icosphere.svg", 72, 72);
  cr = cairo_create(surface);
  cairo_surface_destroy(surface);

  cartesian_point* pts = malloc( ico.size*sizeof(cartesian_point) );
  assert( pts != NULL );

  for (int i=0; i<ico.size; i++) {
    pts[i] = cartesian( ico.nodes[i].pt );
  }

  for (int i=0; i<ico.size; i++) {
    for (int j=0; j<6; j++) {
      if ( ico.nodes[i].neighbors[j] == -1 ) {
	break;
      }
      int k = ico.nodes[i].neighbors[j];
      //if (pts[i].z > 0 && pts[k].z > 0) {
	cairo_move_to(cr, 36*pts[i].x+36, 36*pts[i].y+36);
	cairo_line_to(cr, 36*pts[k].x+36, 36*pts[k].y+36);
	cairo_stroke(cr);
	//}
    }
  }
  
  cairo_show_page(cr);
  cairo_destroy(cr);
  fclose(file);

  free(ico.nodes);
  free(pts);

  return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void print_graph(graph g) {
  printf("GRAPH SIZE: %d\n", g.size);
  for (int i=0; i<g.size; i++) {
    printf("%d -> [ %d, %d, %d, %d, %d, %d ]\n", i,
	   g.nodes[i].neighbors[0],
	   g.nodes[i].neighbors[1],
	   g.nodes[i].neighbors[2],
	   g.nodes[i].neighbors[3],
	   g.nodes[i].neighbors[4],
	   g.nodes[i].neighbors[5] );
  }
}
    
