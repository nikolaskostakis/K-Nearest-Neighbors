#include "ui.h"

GtkWidget *maincanvas_scroll; // scrolled window container for main canvas //
GtkWidget *maincanvas; // main canvas widget // 
cairo_t *maincanvas_cs; // main canvas cairo state // 
GdkDrawable *maincanvas_drawable; // drawable layout window of maincanvas //

GtkWidget *maincanvas_fixed;

GtkWidget *maincanvashscrollbar; // horizontal scrollbar for maincanvas //
GtkWidget *maincanvasvscrollbar; // vertical scrollbar for maincanvas //

GtkObject *maincanvashscrollbaradjustment; // horizontal scrollbar adjustment //
GtkObject *maincanvasvscrollbaradjustment; // vertical scrollbar adjustment //

int reversey = 0; // reverse y-axis flag, place origin at bottom-left //

double scaledvpagesize, scaledhpagesize, vstep, hstep, vpagesize, hpagesize; // horizontal and vertical scrollbars adjustment parameters //

int maincanvasWidth = 550; // main canvas visible dimension - width //
int maincanvasHeight = 550; // main canvas visible dimension - height //
int maincanvasOx = 0; // main canvas visible origin translation - x offset //
int maincanvasOy = 0; // main canvas visible origin translation - y offset //

GtkWidget *mainwindow; // your main window //

double zoomvalue = 1;
double hscrollvalue = 0;
double vscrollvalue = 0;

double ratio = 0;

// Expose-Event Paint Function for maincanvas //
static void expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	#ifdef DEBUGMODE
	{
		printf("DEBUG: Canvas Expose\r\n");
	}
	#endif  

	maincanvaspaint(widget, event, data);
}

// Expose-Event Paint Function for maincanvas //
static void maincanvaspaint(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	maincanvas_drawable = maincanvas->window; // drawable drawing area window //
	//  maincanvas_drawable = GTK_LAYOUT(maincanvas)->bin_window; // drawable layout window //
	maincanvas_cs = gdk_cairo_create(maincanvas_drawable); // corresponding cairo state //

	// call main drawing function //
	draw_maincancas();
	cairo_destroy(maincanvas_cs);
}

// 
void draw_maincancas()
{
	// Draw Canvas
	cairo_set_source_rgb(maincanvas_cs, 0, 0, 0);
	cairo_rectangle(maincanvas_cs, 0, 0, maincanvasWidth, maincanvasHeight);
	cairo_fill(maincanvas_cs);

	setupscrolladjustments();

	draw_shapes();
}

void draw_shapes()
{
	unsigned long i;
	int j;
	double xRatio, yRatio;
	double x,y;
	cairo_text_extents_t te;

	if (pointHTSize != 0)
	{
		xRatio = floor((maincanvasWidth - (2 * CANVASWIDTHOFFSET)) / pointMaxXCoordinate);
		yRatio = floor((maincanvasHeight - (2 * CANVASHEIGHTOFFSET)) / pointMaxYCoordinate);
		ratio = (xRatio < yRatio) ? xRatio : yRatio;
		
		for (i = 0; i < pointHTSize; i++)
		{
			for (j = 0; j < get_point_hash_depth(i); j++)
			{
				// The Coordinates of the point //
				x = CANVASWIDTHOFFSET + (get_point_x_coord(i, j) * ratio)*zoomvalue - maincanvasOx;
				y = CANVASHEIGHTOFFSET + (get_point_y_coord(i, j) * ratio)*zoomvalue - maincanvasOy;
				
				// Draw the point //
				cairo_arc(maincanvas_cs, x, y, (POINT_DIAMETER * zoomvalue), 0, 2 * M_PI);
				cairo_set_source_rgb(maincanvas_cs, 1, 0, 0);
				cairo_fill(maincanvas_cs);

				cairo_set_source_rgb(maincanvas_cs, 1, 1, 1);
				cairo_select_font_face(maincanvas_cs, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
				cairo_set_font_size(maincanvas_cs, 15*zoomvalue);
				cairo_text_extents(maincanvas_cs, get_point_name(i, j), &te);
				cairo_move_to(maincanvas_cs, x, y);
				cairo_show_text(maincanvas_cs, get_point_name(i, j));
			}
		}
	}
}

// Size-Allocate Event Function for maincanvas // 
static void resizemaincanvas(GtkWidget *widget, GdkRectangle *gdkrect, gpointer data)
{
	int newmaincanvasWidth;
	int newmaincanvasHeight;

	#ifdef DEBUGMODE
	{
		printf("DEBUG: Resize Main Canvas.\r\n");
		printf("DEBUG: Current Width, Height = (%d, %d), New Width, Height = (%d, %d)\r\n", maincanvasWidth, maincanvasHeight, gdkrect->width, gdkrect->height);
	}
	#endif

	// resize code //
	newmaincanvasWidth = gdkrect->width;
	newmaincanvasHeight = gdkrect->height;

	maincanvasWidth = newmaincanvasWidth;
	maincanvasHeight = newmaincanvasHeight;
}

static gboolean maincanvasvscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
	// double maxmaincanvasOy;
	// int ivalue; // calibrated, inter y-offset value, based on vertical scrollbar value //
	double dvalue;
	dvalue = gtk_adjustment_get_value(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment));

	#ifdef DEBUGMODE
	{
		printf("DEBUG: Vertical Scroll\r\n");
		printf("DEBUG: Scroll value = %.3f\r\n", value);
	}
	#endif  

	// vertical scrollbar code here //
	maincanvasOy = (scaledvpagesize - maincanvasHeight)/2 + dvalue;
	vscrollvalue = dvalue;
	gtk_widget_queue_draw(maincanvas);

	return TRUE;
	
}

static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
	// double maxmaincanvasOx;
	// int ivalue; // calibrated, inter x-offset value, based on horizontal scrollbar value //
	double dvalue;
	dvalue = gtk_adjustment_get_value(GTK_ADJUSTMENT(maincanvashscrollbaradjustment));

	#ifdef DEBUGMODE
	{
		printf("DEBUG: Horizontal Scroll\r\n");      
		printf("DEBUG: Scroll value = %.3f\r\n", value);
	}
	#endif  

	// horizontal scrollbar code here //
	maincanvasOx = (scaledhpagesize - maincanvasWidth)/2 + dvalue;
	hscrollvalue = dvalue;
	gtk_widget_queue_draw(maincanvas);

	return TRUE;
}

static void scroll(GtkWidget *widget, GdkEventScroll *eev, gpointer data)
{
	#ifdef DEBUGMODE
	{
		printf("DEBUG: Canvas Mouse Scroll\r\n");
	}
	#endif

	// mouse wheel (scroll) code here //
	if (eev->direction == GDK_SCROLL_UP)
	{
		if (zoomvalue < BOUNDZOOMIN)
			zoomvalue += ZOOMSTEP;
	}
	if (eev->direction == GDK_SCROLL_DOWN)
	{
		if (zoomvalue > (BOUNDZOOMOUT + ZOOMSTEP))
			zoomvalue -= ZOOMSTEP;
	}

	gtk_widget_queue_draw(maincanvas);
}

void setupscrolladjustments()
{
  // setup horizontal and vertical scrollbar adjustments //
	double w, h;

	scaledhpagesize = maincanvasWidth * zoomvalue;
	scaledvpagesize = maincanvasHeight * zoomvalue;
	hstep = (scaledhpagesize - maincanvasWidth) / 10;
	vstep = (scaledvpagesize - maincanvasHeight) / 10;
	w = (scaledhpagesize - maincanvasWidth) / 2;
	h = (scaledvpagesize - maincanvasHeight) / 2;


  // use gtk_adjustment_configure() //
	if (zoomvalue <= 1)
	{
		gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvashscrollbaradjustment), 0, 0, 0, 0, 0, 0);
		gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment), 0, 0, 0, 0, 0, 0);
	}
	else
	{
		gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvashscrollbaradjustment), hscrollvalue, -w, w, hstep, 0, 0);
		gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment), vscrollvalue, -h, h, vstep, 0, 0);
	}
}

static void mousebutton(GtkWidget *widget, GdkEventButton *eev, gpointer data)
{
	double x, y;
	// #ifdef DEBUGMODE
	{
		printf("DEBUG: Mouse Button %d Pressed\r\n", eev->button);
	}
	// #endif

	if (eev->button == 1) // Left Mouse Button //
	{
		// Nearest Neighbor //
		// code here //
		GdkEventMotion* e=(GdkEventMotion*)eev;
		printf("Coordinates: (%u,%u)\r\n", (guint)e->x,(guint)e->y);

		if (ratio != 0)
		{
			x = ((guint)e->x - CANVASWIDTHOFFSET + maincanvasOx) / (ratio * zoomvalue) ;
			y = ((guint)e->y - CANVASHEIGHTOFFSET + maincanvasOy) / (ratio * zoomvalue) ;
			printf("New Coordinates: (%lf,%lf)\r\n", x, y);
			print_nearest(x, y);
		}
	}

	if (eev->button == 3) // Right Mouse Button //
	{
		// Reset //
		// code here //
		zoomvalue = 1;
		hscrollvalue = 0;
		vscrollvalue = 0;
	}

	gtk_widget_queue_draw(maincanvas);
}

static void quitaction()
{
	#ifdef DEBUGMODE
	{
		printf("DEBUG: Quit Action\r\n");
	}
	#endif

	// printf("Thank you. Bye now.\r\n");
	printf("Closing GUI...\r\n");
	gtk_main_quit();
}


void start_gui()
{  
	// *** Local Variables *** //

	GtkWidget *maincanvashbox; // contains maincanvas and vertical scrollbar //
	GtkWidget *maincanvasvbox; // contains maincanvashbox and horizontal scrollbar //

	GtkWidget *hpane; // horizontal pane //
	// GtkWidget *hpane2; // horizontal pane //

	GtkWidget *mainframe; // main canvas frame //
	GtkWidget *hierarchybrowserframe; // hierarchy browser frame //

	// *** Local Variables End *** //

	gtk_init(NULL,  NULL);

	mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(mainwindow), "KNN Demo");
	gtk_window_set_default_size(GTK_WINDOW(mainwindow), 300, 300); // default size //

	g_signal_connect(G_OBJECT(mainwindow), "destroy", G_CALLBACK(quitaction), mainwindow);

	maincanvashbox = gtk_hbox_new(FALSE, 0);
	maincanvasvbox = gtk_vbox_new(FALSE, 0);

	// Frames //
	mainframe = gtk_frame_new("Layout View");
	hierarchybrowserframe = gtk_frame_new("Other Frame");

	// Canvas Drawing Area, Frames and Associated Callbacks //
	maincanvas = gtk_drawing_area_new();
	//  maincanvas = gtk_layout_new(NULL, NULL);

	gtk_widget_set_size_request(maincanvas, maincanvasWidth, maincanvasHeight);

	// Application Paintable and Double-Buffering Flags for maincanvas //
	//  gtk_widget_set_app_paintable(maincanvas, FALSE);
	//  gtk_widget_set_double_buffered(maincanvas, FALSE);

	// maincanvas Event Handlers //
	g_signal_connect(G_OBJECT(maincanvas), "expose-event", G_CALLBACK(expose), maincanvas);

	gtk_widget_add_events(maincanvas, GDK_SCROLL);
	gtk_widget_add_events(maincanvas, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(maincanvas), "scroll-event", G_CALLBACK(scroll), maincanvas);
	g_signal_connect(G_OBJECT(maincanvas), "button-press-event", G_CALLBACK(mousebutton), maincanvas);

	maincanvasvscrollbaradjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	maincanvashscrollbaradjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

	// setup horizontal and vertical scrollbar adjustments //
	setupscrolladjustments();

	maincanvasvscrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment));
	maincanvashscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(maincanvashscrollbaradjustment));

	g_signal_connect(G_OBJECT(maincanvasvscrollbar), "value-changed", G_CALLBACK(maincanvasvscroll), maincanvasvscrollbar);
	g_signal_connect(G_OBJECT(maincanvashscrollbar), "value-changed", G_CALLBACK(maincanvashscroll), maincanvashscrollbar);

	// Two Boxes and one Pane Layout //

	gtk_box_pack_start(GTK_BOX(maincanvashbox), maincanvas, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(maincanvashbox), maincanvasvscrollbar, FALSE, FALSE, 0);

	gtk_box_set_child_packing(GTK_BOX(maincanvashbox), maincanvas, TRUE, TRUE, 0, GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(maincanvashbox), maincanvasvscrollbar, FALSE, FALSE, 0, GTK_PACK_END);

	gtk_box_pack_start(GTK_BOX(maincanvasvbox), maincanvashbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(maincanvasvbox), maincanvashscrollbar, FALSE, FALSE, 0);

	gtk_box_set_child_packing(GTK_BOX(maincanvasvbox), maincanvashbox, TRUE, TRUE, 0, GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(maincanvasvbox), maincanvashscrollbar, FALSE, FALSE, 0, GTK_PACK_END);

	gtk_container_add(GTK_CONTAINER(mainframe), maincanvasvbox);

	g_signal_connect(G_OBJECT(maincanvas), "size-allocate", G_CALLBACK(resizemaincanvas), maincanvas);

	hpane = gtk_hpaned_new();
	gtk_paned_add1(GTK_PANED(hpane), hierarchybrowserframe);
	gtk_paned_add2(GTK_PANED(hpane), mainframe);  

	gtk_container_add(GTK_CONTAINER(mainwindow), hpane);

	gtk_widget_show_all(mainwindow);

	gtk_main();

}

// gint main (gint argc, gchar **argv)
// {
// 	// you may call start_gui() from TCL, based on a TCL command //
// 	// alternatively, you may create a thread, but this is a lot more complex! //
// 	start_gui();

// 	return EXIT_SUCCESS;

// }