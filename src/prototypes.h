/*File generated automatically in CM-Vostro-5568 by carlosmiranda on Qua Nov 29 12:45:08 WET 2017*/
#ifdef __cplusplus
extern "C" {
#endif
/* main.c */
int main (int argc, char *argv[]);
/* myf.c */
void CTRLChandler (int signum);
int ChangeBaudRate (int fd, int new_baud);
void ChildALARMhandler (int signum);
int ChildMain (void);
int ChildMain2 (int ppid);
int ChildMain3 (void);
void ChildUSR1handler (int signum);
void ChildUSR2handler (int signum);
int GetSharedMem ();
void InterceptCTRL_C (int a);
void MessageFromChild (char *str);
void MessageFromParent (char *str);
int OpenPort (char *port_name, char *text);
void ParentALARMhandler (int signum);
int ParentMain (void);
int ParentMain2 (int pid_child);
int ParentMain3 (void);
void ParentUSR1handler (int signum);
void ParentUSR2handler (int signum);
int ReadPortUntilChar (int fd);
void ReverseString (char *str);
int SerialEchoer (int fd);
int SetTimerOff (void);
int addN (int a, int b);
int divN (int a, int b);
void goodbye (int xxx);
int initTimer (void (*func) (int));
int mulN (int a, int b);
int myInits ();
int performOperation (int (*oper) (int, int), int a, int b);
int subN (int a, int b);
void vive (int xxx);
void vive1 (int xxx);
void vive2 (int xxx);
/* callbacks.c */
void on_button1_clicked (GtkWidget * widget, gpointer user_data);
void on_button2_clicked (GtkWidget * widget, gpointer user_data);
void on_drawingarea1_draw (GtkWidget * widget, cairo_t * cr, gpointer user_data);
void on_drawingarea1_motion_notify_event (GtkWidget * widget, GdkEventMotion * event, gpointer user_data);
void on_drawingarea1_scroll_event (GtkWidget * widget, GdkEventScroll * event, gpointer user_data);
void on_mouse_button_on (GtkWidget * widget, GdkEventButton * event, gpointer user_data);
gboolean p_ForceRefreshDA (gpointer user_data);
void p_InitTimer ();
void p_RedrawFreeForm (cairo_t * cr);
void p_RedrawText (cairo_t * cr);
gboolean pari_delete_event (GtkWidget * window, GdkEvent * event, gpointer data);
/* pcolor.c */
void HighLightText (void);
void PrintRedLine (void);
void ResetTextColors (void);
void TextColor (int attr, int fg, int bg);
#ifdef __cplusplus
}
#endif
