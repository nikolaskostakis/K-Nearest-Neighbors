#include "ui-scrolled-canvas-skeleton.h"

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

// Expose-Event Paint Function for maincanvas //
static void expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	#ifdef DEBUGMODE
	{
		printf("DEBUG: Canvas Expose\n");
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
	int width = maincanvasWidth;// - (2 * CANVASWIDTHOFFSET);
	int height = maincanvasHeight;// - (2 * CANVASHEIGHTOFFSET);
	
	// Draw Canvas
	cairo_set_source_rgb(maincanvas_cs, 0, 0, 0);
	cairo_rectangle(maincanvas_cs, 0, 0, width, height);
	cairo_fill(maincanvas_cs);

	// cairo_arc(maincanvas_cs, 100, 100, 5, 0, 2 * M_PI);
	// cairo_set_source_rgb(maincanvas_cs, 1, 0, 0);
	// cairo_fill(maincanvas_cs);
	draw_shapes();
}

void draw_shapes()
{
	// cairo_text_extents_t te;
	
	// int i;//, xi;
	// double ratio, xRatio, yRatio;
	// double xMid;

	// xRatio = floor((maincanvasWidth - (2 * CANVASWIDTHOFFSET)) / xMax);
	// yRatio = floor((maincanvasHeight - (2 * CANVASHEIGHTOFFSET)) / yMax);
	// ratio = (xRatio < yRatio) ? xRatio : yRatio;

	// for (i = 0; i < treeSize; i++)
	// {
	// 	if (tree[i].level == 1)
	// 	{
	// 		printf("%lf\n", tree[i].x);
	// 		xMid = (tree[i].x * ratio);
	// 		// xi = i;
	// 		break;
	// 	}
	// }

	// for (i = 0; i < treeSize; i++)
	// {
	// 	if (tree[i].level == 1)
	// 	{
	// 		cairo_move_to(maincanvas_cs, (tree[i].x * ratio), 0);
	// 		cairo_set_source_rgb(maincanvas_cs, 0, 1, 0);
	// 		cairo_set_line_width(maincanvas_cs, 2);
	// 		cairo_rel_line_to(maincanvas_cs, 0, maincanvasHeight);
	// 		cairo_stroke(maincanvas_cs);
	// 	}
	// 	else if (tree[i].level == 2)
	// 	{
	// 		printf("x,y: %lf, %lf (%lf)\n\n", tree[i].x, tree[i].y,xMid);
	// 		cairo_set_source_rgb(maincanvas_cs, 0, 0, 1);
	// 		cairo_set_line_width(maincanvas_cs, 2);
	// 		if ((xMid/ratio) > tree[i].x)
	// 		{
	// 			cairo_move_to(maincanvas_cs, 0, (tree[i].y * ratio));
	// 			cairo_rel_line_to(maincanvas_cs, xMid, 0);
	// 			cairo_stroke(maincanvas_cs);
	// 		}
	// 		else
	// 		{
	// 			printf("xaxaxaxaxa\n");
	// 			cairo_move_to(maincanvas_cs, xMid, (tree[i].y * ratio));
	// 			cairo_rel_line_to(maincanvas_cs, maincanvasWidth - xMid, 0);
	// 			cairo_stroke(maincanvas_cs);
	// 		}
	// 	}

	// 	cairo_arc(maincanvas_cs, (tree[i].x * ratio), (tree[i].y * ratio), 2.5, 0, 2 * M_PI);
	// 	cairo_set_source_rgb(maincanvas_cs, 1, 0, 0);
	// 	cairo_fill(maincanvas_cs);
	// }

	/*
	// Red filled Square
	cairo_rectangle(maincanvas_cs, 40, 40, 100, 100);
	cairo_set_source_rgb(maincanvas_cs, 1, 0, 0);
	cairo_fill(maincanvas_cs);

	cairo_set_source_rgb(maincanvas_cs, 1, 1, 1);
	cairo_select_font_face (maincanvas_cs, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (maincanvas_cs, 15);
	cairo_text_extents (maincanvas_cs, "Square", &te);
	cairo_move_to (maincanvas_cs, (40 + 50) - te.width / 2 - te.x_bearing,
	    (40 + 50) - te.height / 2 - te.y_bearing);
	cairo_show_text (maincanvas_cs, "Square");

	// Green-ish Rectangle
	cairo_rectangle(maincanvas_cs, 340, 430, 100, 40);
	cairo_set_source_rgb(maincanvas_cs, 153/255, 255/255, 204/255);
	cairo_stroke(maincanvas_cs);
	
	cairo_set_source_rgb(maincanvas_cs, 153/255, 255/255, 204/255);
	cairo_select_font_face (maincanvas_cs, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (maincanvas_cs, 15);
	cairo_text_extents (maincanvas_cs, "Rectangle", &te);
	cairo_move_to (maincanvas_cs, (340 + 50) - te.width / 2 - te.x_bearing,
	    (430 + 20) - te.height / 2 - te.y_bearing);
	cairo_show_text (maincanvas_cs, "Rectangle");
	*/

}

// Size-Allocate Event Function for maincanvas // 
static void resizemaincanvas(GtkWidget *widget, GdkRectangle *gdkrect, gpointer data)
{
	int newmaincanvasWidth;
	int newmaincanvasHeight;

	#ifdef DEBUGMODE
	{
		printf("DEBUG: Resize Main Canvas.\n");
		printf("DEBUG: Current Width, Height = (%d, %d), New Width, Height = (%d, %d)\n", maincanvasWidth, maincanvasHeight, gdkrect->width, gdkrect->height);
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

	// #ifdef DEBUGMODE
	{
		printf("DEBUG: Vertical Scroll\n");
		printf("DEBUG: Scroll value = %.3f\n", value);
	}
	// #endif  

	// vertical scrollbar code here //
	return TRUE;
	
}

static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
	// double maxmaincanvasOx;
	// int ivalue; // calibrated, inter x-offset value, based on horizontal scrollbar value //

	// #ifdef DEBUGMODE
	{
		printf("DEBUG: Horizontal Scroll\n");      
		printf("DEBUG: Scroll value = %.3f\n", value);
	}
	// #endif  

	// horizontal scrollbar code here //
	return TRUE;
}

static void scroll(GtkWidget *widget, GdkEventScroll *eev, gpointer data)
{
	// #ifdef DEBUGMODE
	{
		printf("DEBUG: Canvas Mouse Scroll\n");
	}
	// #endif

	// mouse wheel (scroll) code here //
}

void setupscrolladjustments()
{
  // setup horizontal and vertical scrollbar adjustments //

  // use gtk_adjustment_configure() //
}

static void mousebutton(GtkWidget *widget, GdkEventButton *eev, gpointer data)
{
	// #ifdef DEBUGMODE
	{
		printf("DEBUG: Mouse Button %d Pressed\n", eev->button);
	}
	// #endif

	if (eev->button == 1) // Left Mouse Button //
	{
		// code here //
	}

	if (eev->button == 3) // Right Mouse Button //
	{
		// code here //
	}
  
}

static void quitaction()
{
	#ifdef DEBUGMODE
	{
		printf("DEBUG: Quit Action\n");
	}
	#endif

	printf("Thank you. Bye now.\n");
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