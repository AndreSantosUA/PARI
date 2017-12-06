#include "myf.h"




/**
 * @brief  Callback to process the delete_event (usually, window kill request)
 *
 * @param  window - The widget that generated the event (not used here)
 * @param  event - The event description (not used here)
 * @param  data - generic user data (not used here)
 * @return FALSE - destroy window but does not leave gtk_main loop, and generates a "destroy" event that you can intersect with another callback.
 * @return TRUE - ignore this delete_event request and does nothing. This can be used to give a second chance to user before closing window!
 * The same effect of <b>TRUE</b> is obtained by returning no value, that is using a <b>void</b> type of function.
 */
gboolean pari_delete_event(GtkWidget * window, GdkEvent * event, gpointer data)
{
        puts("Pedido de delete event");
        //return FALSE; //destroy window but does not leave gtk_main loop, and generates a "destroy" event
        return TRUE; //ignore this delete_event request and does nothing. Same effect as returning 'void'
}



void on_button2_clicked(GtkWidget *widget, gpointer user_data)
{
	gboolean a=gtk_switch_get_active( GTK_SWITCH(user_data) ); //user data points to switch
	if (a == FALSE)
		printf("Must enable the Quit switch before leaving\n");
	else
		gtk_main_quit();

}


/**
 * @brief  Function to process the text from entry and label
 *
 * @param widget - the one that originated the call
 * @param user_data - Custom data with a widget ID to fetch some info
 * @return void
 */
void on_button1_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkEntry *ts = GTK_ENTRY(user_data);
	GtkLabel *lbl= GTK_LABEL(gtk_builder_get_object(builderG,"label3")); //Get some specific label ID to change it
	if(ts)
	{
		char* tss = (char*) gtk_entry_get_text( ts ); //get the text from entry1 widget
        	
        	
		ReverseString( tss);
		
		printf("Text available in entry1: %s\n", tss);
		gtk_label_set_text(lbl, tss);   //set entry text in "label3" widget
		//gtk_entry_set_text(ts,tss);
		
		gtk_widget_hide( user_data );
		gtk_widget_show( user_data );
		
	}
}



void on_drawingarea1_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
        p_RedrawText(cr);
        p_RedrawFreeForm(cr);
}

void on_mouse_button_on(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
        if( event->button == 2 )
        {
          // a simple instruction to reset the counting of the free-form segments
					ffG.count=0;
          // which "clears" the line :-)
          /* ... */
        }
        gtk_widget_queue_draw(widget);   //do not forget to force refresh of the widget - draw
}

void on_drawingarea1_scroll_event(GtkWidget *widget, GdkEventScroll *event, gpointer user_data)
{
        switch(event->direction)
        {
                case GDK_SCROLL_UP:
                        fontSizeFactorG += 0.1;  //increase factor
                        break;

                case GDK_SCROLL_DOWN:
                        fontSizeFactorG -= 0.1;;  //decrease factor
                        break;

                case GDK_SCROLL_LEFT:
                case GDK_SCROLL_RIGHT:
                case GDK_SCROLL_SMOOTH:
                        break;
        }
        gtk_widget_queue_draw(widget);  //force redrawing of area
}

void on_drawingarea1_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
        if( event->state & GDK_BUTTON3_MASK) //button 3 move text
        {
                xoffG = event->x;  /*mouse coordinate x */
                yoffG = event->y;  /*mouse coordinate y */
        }
        

				if( event->state & GDK_BUTTON1_MASK)
				{
						  if( ffG.x[ffG.count] != event->x && ffG.y[ffG.count] != event->y)		//se o ponto atual for diferente da coordenada anterior guarda a coordenada nova
						  {
						          ffG.x[ffG.count]   = event->x;
						          ffG.y[ffG.count++] = event->y;
						          ffG.count %= MAXFFBUFFER;			//se o contador chegar a 1000 pontos (limite), contador volta a 0
						  }
				}
	
	gtk_widget_queue_draw(widget);  //force redraw the widget
}

void p_RedrawText(cairo_t *cr){

	const char *font="Courier";                //name of the font to use.
  char string[200];                       //aux string to put some text
  cairo_set_source_rgb(cr, 1, 0, 0);      //set color of the text (in RGB)

  //Below, a simple function to write text. If you need more sophisticated text rendering use pango instead.
  //// cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 20.0*fontSizeFactorG);          //adjust font size
  ////sprintf(string,"Já me tinhas dito!");  //the actual string to render
 

	
  time_t timep=time(NULL);
  struct tm *tm=localtime(&timep);
  cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	if(tm->tm_sec%2)	//se for impar dá 1
  	sprintf(string,"%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
	else
	 	sprintf(string,"%02d %02d %02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

	cairo_move_to(cr, xoffG, yoffG);        //place the text on the "surface"
  cairo_show_text(cr, string);            //finally draw the text on the "surface"

}

void p_RedrawFreeForm(cairo_t * cr){

        cairo_set_source_rgb(cr, 0, 1, 0);  //define a cor (rgb)	verde
        cairo_set_line_width(cr, 1);        //impõe largura de linha

        int i;
        cairo_move_to(cr, ffG.x[0], ffG.y[0]);  //inicia o path
        for(i = 1; i < ffG.count; i++)          //percorre os pontos todos acumulados
        {
                cairo_line_to(cr, ffG.x[i], ffG.y[i]);  //atualiza o "path" segmento a segmento
        }
        cairo_stroke(cr);     //"imprime" o path na "surface" (neste caso será a drawing area)
}

void p_InitTimer()
{
  //Get widget ID for "drawingarea1"
  GtkWidget *da=GTK_WIDGET(gtk_builder_get_object (builderG, "drawingarea1"));
  //define timeout (timer): interval, callback to execute, and pass the window ID to refresh
  g_timeout_add (200, p_ForceRefreshDA, da);
}

gboolean p_ForceRefreshDA(gpointer user_data)
{
        GtkWidget *da;
				//if nao necessario, se nao meter nada o gajo mete
        if( ! user_data)	
           da = GTK_WIDGET(gtk_builder_get_object (builderG, "drawingarea1"));
        else
           da=GTK_WIDGET(user_data);

        //gdk_window_invalidate_rect (gtk_widget_get_window(da), NULL, FALSE); //would make draw parent window
        gtk_widget_queue_draw(da);  //make draw the widget

        return TRUE;  //continue running
}




gboolean pari_UpdateImageAreas(gpointer data)
{
        //generate an expose event (draw event) on drawingarea1
        GtkWidget *da1 = GTK_WIDGET(gtk_builder_get_object(builderG, "drawingarea1"));
        gtk_widget_queue_draw(da1);
        return TRUE;
}

