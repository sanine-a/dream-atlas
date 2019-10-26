#include "icosphere.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// point-conversion functions

cartesian_point cartesian( spherical_point pt_s ) {
  cartesian_point pt_c;

  pt_c.x = pt_s.r * cos(pt_s.phi) * cos(pt_s.theta);
  pt_c.y = pt_s.r * cos(pt_s.phi) * sin(pt_s.theta);
  pt_c.z = pt_s.r * sin(pt_s.phi);

  return pt_c;
}

spherical_point spherical( cartesian_point pt_c ) {
  spherical_point pt_s;

  pt_s.r = sqrt( pt_c.x*pt_c.x + pt_c.y*pt_c.y + pt_c.z*pt_c.z );
  pt_s.theta = atan2(pt_c.y, pt_c.x);
  pt_s.phi = asin( pt_c.z/pt_s.r );

  return pt_s;
}

// find midpoints
spherical_point midpoint(spherical_point p1, spherical_point p2) {
  cartesian_point c1, c2;
  c1 = cartesian(p1);
  c2 = cartesian(p2);

  cartesian_point c3;
  c3.x = 0.5*(c1.x + c2.x);
  c3.y = 0.5*(c1.y + c2.y);
  c3.z = 0.5*(c1.z + c2.z);

  return spherical(c3);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// create a new node & initialize neighbors and midpoints to -1
node new_node() {
  node n;
  for (int i=0; i<6; i++) {
    n.neighbors[i] = -1;
  }
  return n;
}

// connect two nodes
void connect( node* nodes, int node1, int node2 ) {
  for (int i=0; i<6; i++) {
    if ( nodes[node1].neighbors[i] == node2 ) {
      // node 2 was already present
      break;
    }
    else if ( nodes[node1].neighbors[i] == -1 ) {
      nodes[node1].neighbors[i] = node2;
      break;
    }
  }
  for (int i=0; i<6; i++) {
    if ( nodes[node2].neighbors[i] == node1 ) {
      // node 2 was already present
      break;
    }
    else if ( nodes[node2].neighbors[i] == -1 ) {
      nodes[node2].neighbors[i] = node1;
      break;
    }
  }
}

//check if two nodes are connected
int connected(node* nodes, int node1, int node2) {
  if ( node1 == -1 || node2 == -1 ) {
    return 0;
  }
  for (int i=0; i<6; i++) {
    if ( nodes[node1].neighbors[i] == node2 ) {
      return 1;
    }
  }
  return 0;
}

// subdivide a graph
graph subdivide( graph g, int edges ) {
  graph result;
  result.size = g.size+edges;
  result.nodes = malloc( (g.size + edges)*sizeof(node) );
  assert( result.nodes != NULL );
  
  // copy original points into result nodes
  for (int i=0; i<g.size; i++) {
    result.nodes[i] = new_node();
    result.nodes[i].pt = g.nodes[i].pt;
  }

  // initialize midpoint indices
  int* midpoints = malloc( 6*g.size*sizeof(int) );
  assert( midpoints != NULL );
  for (int i=0; i<6*g.size; i++) {
    midpoints[i] = -1;
  }

  // find midpoints
  int index = g.size;
  for (int n1=0; n1<g.size; n1++) {
    for (int i=0; i<6; i++) {
      if (midpoints[6*n1+i] == -1 && g.nodes[n1].neighbors[i] != -1) {
	// we have not yet computed this midpoint

	int n2 = g.nodes[n1].neighbors[i];
	// find the neighbor's index for i
	int j=0;
	for (int k=0; k<6; k++) {
	  if ( g.nodes[n2].neighbors[k] == n1 ) {
	    j = k;
	    break;
	  }
	}

	result.nodes[index] = new_node();
	result.nodes[index].pt = midpoint(g.nodes[n1].pt, g.nodes[n2].pt);
	midpoints[6*n1+i] = index;
	midpoints[6*n2+j] = index;
	index++;
      }
    }
  }

  // rebuild graph
  for (int n1=0; n1<g.size; n1++) {
    for (int i=0; i<6; i++) {
      if ( g.nodes[n1].neighbors[i] != -1 ) {
	connect(result.nodes, n1, midpoints[6*n1+i]);
	for (int j=0; j<6; j++) {
	  // if two neighbors were connected, connect their midpoints
	  if ( connected(g.nodes, g.nodes[n1].neighbors[i], g.nodes[n1].neighbors[j]) ) {
	    connect(result.nodes, midpoints[6*n1+i], midpoints[6*n1+j]);
	  }
	}
      }
    }
  }

  free(midpoints);

  return result;
}
  

graph icosphere(int subdivisions) {

  // construct icosahedron
  double pi = 3.141592;
  double a = atan2(1,2);

  graph ico;
  ico.size = 12;
  ico.nodes = malloc(12*sizeof(node));
  assert(ico.nodes != NULL);

  // initialize all nodes
  for (int i=0; i<12; i++) {
    ico.nodes[i] = new_node();
  }

  // top & bottom
  ico.nodes[10].pt.r = 1;
  ico.nodes[10].pt.theta = 0;
  ico.nodes[10].pt.phi = pi/2;

  ico.nodes[11].pt.r = 1;
  ico.nodes[11].pt.theta = 0;
  ico.nodes[11].pt.phi = -pi/2;

  // middle points
  for (int i=0; i<10; i++) {
    int k = -2*(i%2) + 1; // alternate rotating up & down
    ico.nodes[i].pt.r = 1;
    ico.nodes[i].pt.theta = i*pi/5;
    ico.nodes[i].pt.phi = a*k;
  }

  // ~~~~build icosahedron~~~~
  // top connects to the even nodes
  for (int i=0; i<10; i+=2) {
    connect(ico.nodes, 10, i);
  }
  
  // bottom connects to the odd nodes
  for (int i=1; i<10; i+=2) {
    connect(ico.nodes, 11, i);
  }

  // connect middle nodes
  for (int i=0; i<10; i++) {
    int a = (i+8)%10; // back 2
    int b = (i+9)%10; // back 1
    int c = (i+11)%10; // forward 1
    int d = (i+12)%10; // forward 2
    
    connect(ico.nodes, i, a);
    connect(ico.nodes, i, b);
    connect(ico.nodes, i, c);
    connect(ico.nodes, i, d);
  }

  // subdivide
  for (int i=0; i<subdivisions; i++) {
    node* old_nodes = ico.nodes;
    int n_edges = 30 * pow(4,i);
    ico = subdivide(ico, n_edges);
    free(old_nodes);
  }
  
  return ico;
}
  
