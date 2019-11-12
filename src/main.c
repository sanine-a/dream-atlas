#include <stdio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-svg.h>
#include "icosphere.h"
#include "world.h"
#include "cartography.h"
#include <gtk/gtk.h>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void update_long(GtkWidget*,void*);
void update_lat(GtkWidget*,void*);
void draw_map(GtkWidget*,cairo_t*,void*);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct {
  GtkSpinButton* long_spin;
  GtkSpinButton* lat_spin;
  GtkDrawingArea* map_render;
  world w;
  spherical_point perspective;
} glob;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(int argc, char** argv) {
  g_print("int size: %d\n",sizeof(int));
  
  // generate terrain
  graph ico = icosphere(3);
  for (int i=0; i<ico.size; i++) {
    ico.nodes[i].pt.r = 1;
  }
  
  glob.w.terrain = ico;

  // prepare window
  GtkBuilder* builder;
  GError* error = NULL;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, "test.ui", &error) == 0) {
    g_printerr("Error loading file %s\n", error->message);
    g_clear_error(&error);
    //free(ico.nodes);
    return 1;
  }

  gtk_builder_add_callback_symbol(builder, "update_long", G_CALLBACK(update_long));
  gtk_builder_add_callback_symbol(builder, "update_lat",  G_CALLBACK(update_lat) );
  gtk_builder_add_callback_symbol(builder, "draw_map",    G_CALLBACK(draw_map)   );

  gtk_builder_connect_signals(builder, NULL);

  // put spinbuttons & drawing_area in glob
  glob.long_spin = (GtkSpinButton*) gtk_builder_get_object(builder, "long-spin");
  glob.lat_spin  = (GtkSpinButton*) gtk_builder_get_object(builder, "lat-spin" );
  glob.map_render = (GtkDrawingArea*) gtk_builder_get_object(builder, "map-render");

  // set up drawing area
  gtk_widget_set_size_request(GTK_WIDGET(glob.map_render), 256, 256);

  gtk_main();//*/

  /*cairo_surface_t* surface;
  cairo_t* cr;

  FILE* file;

  file = fopen( "icosphere.svg", "w" );
  if ( file == NULL ) {
    fprintf(stderr, "failed to open file\n");
    return -1;
  }
  
  surface = cairo_svg_surface_create("icosphere.svg", 256, 256);
  cr = cairo_create(surface);
  cairo_surface_destroy(surface);

  render_orthographic(glob.w, cr, 0,0.5*M_PI, 256,256, RAW_MAP);
  
  cairo_show_page(cr);
  cairo_destroy(cr);
  fclose(file);//*/

  
  free(ico.nodes);

  return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void update_long(GtkWidget* w, void* d) {
  glob.perspective.theta = gtk_spin_button_get_value(glob.long_spin) * M_PI/180;
  gtk_widget_queue_draw(GTK_WIDGET(glob.map_render));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void update_lat(GtkWidget* w, void* d) {
  glob.perspective.phi = gtk_spin_button_get_value(glob.lat_spin) * M_PI/180;
  gtk_widget_queue_draw(GTK_WIDGET(glob.map_render));  
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void draw_map(GtkWidget* w, cairo_t* cr, void* d) {
  render_orthographic(glob.w, cr, glob.perspective.theta, glob.perspective.phi, 256, 256, RAW_MAP);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
