#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#include "../definitions.h"

#define ZOOMSTEP      0.2
#define BOUNDZOOMIN   10
#define BOUNDZOOMOUT  0.1

#define CANVASOFFSET        25
#define CANVASWIDTHOFFSET   CANVASOFFSET
#define CANVASHEIGHTOFFSET  CANVASOFFSET

#define POINT_DIAMETER 2.5

extern GtkWidget *maincanvas_scroll; // scrolled window container for main canvas //
extern GtkWidget *maincanvas; // main canvas widget // 
extern cairo_t *maincanvas_cs; // main canvas cairo state // 
extern GdkDrawable *maincanvas_drawable; // drawable layout window of maincanvas //

extern GtkWidget *maincanvas_fixed;

extern GtkWidget *maincanvashscrollbar; // horizontal scrollbar for maincanvas //
extern GtkWidget *maincanvasvscrollbar; // vertical scrollbar for maincanvas //

extern GtkObject *maincanvashscrollbaradjustment; // horizontal scrollbar adjustment //
extern GtkObject *maincanvasvscrollbaradjustment; // vertical scrollbar adjustment //

extern int reversey; // reverse y-axis flag, place origin at bottom-left //

extern double scaledvpagesize, scaledhpagesize, vstep, hstep, vpagesize, hpagesize; // horizontal and vertical scrollbars adjustment parameters //

extern int maincanvasWidth; // main canvas visible dimension - width //
extern int maincanvasHeight; // main canvas visible dimension - height //
extern int maincanvasOx; // main canvas visible origin translation - x offset //
extern int maincanvasOy; // main canvas visible origin translation - y offset //

extern GtkWidget *mainwindow; // your main window //

// *** Function Prototypes *** //

static void expose(GtkWidget *, GdkEventExpose *, gpointer);
static void maincanvaspaint(GtkWidget *, GdkEventExpose *, gpointer);
static void resizemaincanvas(GtkWidget *, GdkRectangle *, gpointer);
static gboolean maincanvasvscroll(GtkRange *, GtkScrollType, gdouble, gpointer);
static gboolean maincanvashscroll(GtkRange *, GtkScrollType, gdouble, gpointer);
static void scroll(GtkWidget *, GdkEventScroll *, gpointer);
void setupscrolladjustments();
static void mousebutton(GtkWidget *, GdkEventButton *, gpointer);
static void quitaction();
void start_gui();

void draw_maincancas();
void draw_shapes();
void draw_point(double x, double y);

// External Variables //

// From structs //
extern double pointMaxXCoordinate;
extern double pointMaxYCoordinate;
extern unsigned long elementArraySize;

// External Functions //

// From structs //
extern int get_point_hash_depth(unsigned long hash);
extern double get_point_x_coord(unsigned long hash, int depth);
extern double get_point_y_coord(unsigned long hash, int depth);
extern char *get_point_name(unsigned long hash, int depth);
extern void dump_nearest_neighbor(double x, double y);
extern char *get_element_name(unsigned long index);
extern double get_element_x(unsigned long index);
extern double get_element_y(unsigned long index);
extern double get_element_width(unsigned long index);
extern double get_element_height(unsigned long index);
extern int get_element_type(unsigned long index);
