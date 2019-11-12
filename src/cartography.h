#include <cairo/cairo.h>
#include "world.h"

#ifndef CARTOGRAPHY_H
#define CARTOGRAPHY_H

typedef enum { RAW_MAP, COAST_MAP } map_type;
void render_orthographic(world w, cairo_t* cr, double theta0, double phi0, int width, int height, map_type mp);

#endif
