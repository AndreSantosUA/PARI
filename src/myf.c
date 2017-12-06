/**
 *       @file  myf.c
 *      @brief  Breve Descrição
 *
 * 	 ...
 *
 *     @André Santos Drak, santos.andre@ua.pt
 *
 *   @internal
 *     Created  22-Nov-2017
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2017, Drak
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include "pcolor.h"
#include "myf.h"

#define NN 100


/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int myInits()
{
	static int firstTimeS = 1;

	if (firstTimeS)
	{
		printf("First time\n");
		firstTimeS = 0;
		return 1;
	}
	else
	{
		printf("Not the first time\n");
		return 0;
	}

}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int addN(int a, int b)
{
	return a + b;
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int divN(int a, int b)
{
	if (b)
		return a / b;
	return 0;
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int mulN(int a, int b)
{
	return a * b;
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int subN(int a, int b)
{
	return a - b;
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int performOperation(int (*oper) (int, int), int a, int b)
{
	return oper(a, b);
}

void vive(int xxx)
{
	printf("CTRL + C pressed!\n");
}

void vive1(int xxx)
{
	printf("CTRL + Z pressed!\n");
}

void vive2(int xxx)
{
	printf("CTRL + \\ pressed!\n");
}

void goodbye(int xxx)
{
	printf("Termination signal arrived. Exiting!\n");
	exit(1);
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int initTimer(void (*func) (int))
{
/* Notes: 
 * void (*func) (int) is a pointer to a function which returns void
 * and accepts an integer
 */

	// Timers decrement from it_value to zero, generate a signal, and reset to it_interval.
	// A timer which is set to zero (it_value is zero or the timer expires and it_interval is zero) stops.

	const struct itimerval vValue = { {0, 100000},	// it_interval: next value (sec,usecs) - interval of repetition (1 sec) 
	{1, 0}			// it_value: initial value (sec,usecs) - initial delay (1 second here)
	};
	int a = setitimer(ITIMER_REAL, &vValue, NULL);
	if (a)
	{
		perror("Setting timer:");
		return 0;	// failure
	}

	signal(SIGALRM, func);	// prepare the appropriate callback upon signal occurrence
	return 1;		// success
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int SetTimerOff(void)
{
	const struct itimerval vValue = { {0, 0},
	{0, 0}
	};

	int a = setitimer(ITIMER_REAL, &vValue, NULL);
	//int a = setitimer(ITIMER_REAL, &vValue, NULL);
	if (a)
	{
		perror("Setting timer off:");
		return 0;	// failure
	}

	return 1;		// success
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int ChildMain(void)
{
	int n;
	char str[100];

	signal(SIGINT, CTRLChandler);

	for (n = 0; n < 100; n++)
	{
		usleep(200000);
		sprintf(str, "Luke counts: %03d    \r", n);
		MessageFromChild(str);
		fflush(stdout);
	}

	return 0;
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
int ParentMain(void)
{
	int n;
	char str[100];

	for (n = 0; n < 100; n++)
	{
		usleep(300000);
		sprintf(str, "\t\t\t\tLord Vader counts: %03d    \r", n);
		MessageFromParent(str);
		fflush(stdout);
	}

	return 0;
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
void MessageFromChild(char *str)
{
	TextColor(RESET, GREEN, WHITE);	//from pcolor
	printf("%s", str);
	ResetTextColors();	//from pcolor
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
void MessageFromParent(char *str)
{
	TextColor(RESET, RED, WHITE);	//from pcolor
	printf("%s", str);
	ResetTextColors();	//from pcolor
}

/**
 * @brief  Function
 * @param  Inputs
 * @return Outputs
 */
void CTRLChandler(int signum)
{
	printf("\nCTRL+C pressed but ignored\n");
}

int ChildMain2(int ppid)
{
	signal(SIGALRM, ChildALARMhandler);
	alarm(2);
	do
	{
	
	} while (count_childG != 5);
	signal(SIGALRM, NULL);	//remove signal handling
	kill(ppid, SIGUSR2);	//Chance for parent to issue a farewell msg
	MessageFromChild("Child leaving\n");
	return 0;
}

void ChildUSR1handler(int signum)
{
	count_childG ++;
	char str[100];
	sprintf(str, "Another hit received in child: %d\n", count_childG);
	MessageFromChild(str);
}

void ChildUSR2handler(int signum)
{
	MessageFromChild("Child Terminated\n");
	exit(0);
}

void ChildALARMhandler(int signum)
{
	kill(getppid(), SIGUSR1);	//send signal to parent
	MessageFromChild("----signal sent to parent\n");
	alarm(2);		//launch a new alarm in 2 secs
}

void ParentUSR1handler(int signum)
{
	count_parentG ++;
	char str[100];
	sprintf(str, "Parent received another hit: %d\n", count_parentG);
	MessageFromParent(str);
}

int ParentMain2(int pid_child)
{
	pid_childG = pid_child;
	signal(SIGALRM, ParentALARMhandler);
	alarm(2);

	do{ }while(count_parentG != 5);
	signal(SIGALRM, NULL); // remove signal handling
	kill(pid_child, SIGUSR2); // farewell msg

	MessageFromParent("Parent leaving\n");
	return 0;
}

void ParentALARMhandler(int signum)
{
	kill(pid_childG, SIGUSR1);
	MessageFromParent("signal SIGUSR1 sent to child\n");
	alarm(2);
}

void ParentUSR2handler(int signum)
{
	MessageFromParent("Parent Terminated\n");
	exit(0);
}

int ChildMain3(void)
{
    int n, shm_id;
    char *data;      //generic pointer to serve as link for the shared memory
    char str[100];   //string to put a message
	usleep(CHILDDELAY);
    shm_id = GetSharedMem();
    if(shm_id == -1) return -1;     //failure

    /* attach to the memory segment to get a pointer to it */
    data = (char *)shmat(shm_id, (void *) 0, 0);
    if(data == (char *) (-1))
    {
            perror("shmat");
            exit(1);
    }

    /*data now points to a shared area */

    for(n = 0; n < 10; n++)   //read 10 messages from the shared memory (with some delay)
    {
            usleep(CHILDDELAY);
            sprintf(str,"\t\tChild iteration: %03d  Message read from parent:%s\n", n, data);
            MessageFromChild(str);
    }

    /* detach from the mem segment */
    if( shmdt((void *)data) == -1)
    {
            perror("shmdt");
            exit(1);
    }
    return 0;
}

int ParentMain3(void)
{
    int n, shm_id;
    char *data; //generic pointer to serve as link for the shared memory
    char str[200];   //string to put a message
	usleep(PARENTDELAY);
    shm_id = GetSharedMem();
    if(shm_id == -1) return -1;     //failure

    /* attach to the memory segment to get a pointer to it */
    data = shmat(shm_id, (void *) 0, 0);
    if(data == (char *) (-1))
    {
            perror("shmat");
            exit(1);
    }
    /*'data' now points to a shared area */

    for(n = 0; n < 12; n++)  //write 12 messages in the shared memory (with some delay)
    {
            usleep(PARENTDELAY);
            sprintf(data, "Msg no. %d to my son", n);
            sprintf(str, "Parent iteration %03d. Message sent: %s\n", n, data);
            MessageFromParent(str);
    }

    /* detach from the mem segment since it is leaving */
    if(  shmdt(data) == -1 )
    {
            perror("shmdt");
            exit(1);
    }
    return shm_id;

}

/**
 * @brief  Gets a shared memory and connects to it
 *
 * The function first creates a random key to identify
 * the memory and then connects to it with some specific permissions
 * @return int The Shared memory ID. Returns -1 in case of failure.
 */
int GetSharedMem()
{
        key_t key;                       /*it's a long int */
        int shm_id;
        key = ftok("main.c", 'X');       /* generate a random key based on a file name and char */
        if(key == -1) { perror("ftok"); return -1; }

        /* now connect to (or possibly create) the shared memory segment with permission 644 */
        shm_id = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
        if(shm_id == -1) { perror("shmget"); return -1; }

        return shm_id;
}

int OpenPort( char *port_name, char* text)
{
    /* ... code  */
    int fd;
    fd=open(port_name, O_RDWR | O_NDELAY ); /*O_RDONLY, O_WRONLY*/
    if(fd == -1) 
    	return -1; 
	else if(text != NULL)
	    write( fd, text, strlen(text) );
    return fd;
}

int ReadPortUntilChar(int fd)
{
	int n;
    char ch;
    do
    {
        n=read( fd, &ch, 1);
        if( n == -1 ) continue;   //perror("Err:");
        if(n>0) { printf("%c", ch); fflush(stdout);}
    } while( ch != '<');

    return 0;
}

int ChangeBaudRate( int fd, int new_baud)
{
    /*... código...*/
    int oldS=0;
    oldS=oldS;
    struct termios options;

    /** Get the current options for the port...  */
    tcgetattr(fd, &options);
    oldS=cfgetispeed(&options);

    /** Set the baud rates to newBaud for example B19200...  */
    cfsetispeed(&options, new_baud);
    cfsetospeed(&options, new_baud);

    /** Enable the receiver and set local mode...  */
    options.c_cflag |= (CLOCAL | CREAD);

    /** Set the new options for the port...  */
    tcsetattr(fd, TCSANOW, &options);

    return 1;
}

int SerialEchoer(int fd)
{
    int n = 0;
    char buf[30];
    char ch;
    
    do
    {
        n=read(fd, &ch, 1);
        if(n > 0 )
        {
                // ...
                write(fd, & buf, 1);
        }
    } while(ch != 'a');
    
    return 1;
}



/**
 * @brief  Reverses the characters in a string
 * @param  str string pointer
 * @return void
 */
void ReverseString(char *str)
{
	char c, *t1, *t2, *t3 ;
	for(t1 = str, t2 = str + strlen(str); t1 < t2;) //notice that 3rd param of for is empty
		c = *t1, *t1++ = *--t2, *t2 = c;

		
	/*scan all bytes and reverse each multibyte character */
	for(t1 = t2 = str; (c = *t2++);) //notice the parenthesis on "condition" to avoid compiler warning!
	{
		if((c & 0x80) == 0)   // It is a pure ASCII char, no need for more processing!
			t1 = t2;
		else if((c & 0xc0) == 0xc0)   // It is the start of a multibyte, check how many bytes per UTF-8 character
		{
			t3 = t2;
			switch (t2 - t1)
			{
				case 4:
					c = *t1, *t1++ = *--t2, *t2 = c;    // fallthrough
				case 3:                               // fallthrough
				case 2:
					c = *t1, *t1++ = *--t2, *t2 = c;
			}
			t2 = t1 = t3;
		}
	}
}


/**
 * @brief  Callback to catch CTRL-C
 */
void InterceptCTRL_C(int a)
{
  g_print("Sair por CTRL-C\n");
  gtk_main_quit();
}



CvCapture *pari_StartImageAcquisition()
{
        CvCapture *capture = cvCaptureFromCAM(0);              //Capture using 1st camera: i.e., no. 0
        IplImage *frame=cvQueryFrame(capture);                 //Update frame. Pointer does not change.
        src_imageG = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);//create image for color (8 bits, 3 channels)
        dst_imageG = cvClone(src_imageG);                       //create clone of source image
        return capture;
}

void pari_PerformImageAcquisition(CvCapture *capt)
{
        static IplImage *frame;
        frame=cvQueryFrame(capt);               //Update frame pointer.
        cvCopy(frame, src_imageG, NULL);        //copy frame as it is (preserve color)
        cvCopy(src_imageG, dst_imageG, NULL);   //copy src into dst and ready to process (admitt similar image structure)
}


// Exemplo de código de uma função que converte
// uma IplImage num pixbuf para inserir numa interface GTK
// e ajusta-a com certas dimensões passadas como argumentos

GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage * image, int dst_w, int dst_h)
{
    IplImage *gtkMask=image;
    GdkPixbuf *pix, *rpix;
    cvCvtColor(image, gtkMask, CV_BGR2RGB);
    pix = gdk_pixbuf_new_from_data((guchar *) gtkMask->imageData,
                       GDK_COLORSPACE_RGB,
                       FALSE,
                       gtkMask->depth,
                       gtkMask->width,
                       gtkMask->height,
                       gtkMask->widthStep, NULL, NULL);
    rpix = gdk_pixbuf_scale_simple(pix, dst_w, dst_h, GDK_INTERP_BILINEAR); //rescale image
    g_object_unref(pix);  //free the intermediate pixbuf...
    return rpix;
}

//copy from IplImage to pixbuf and paint DA
void pari_RefreshDrawingArea( char * widgetName, IplImage *img)
{
        GtkWidget *da=GTK_WIDGET(gtk_builder_get_object (builderG, widgetName));
        if( ! da )
        {
          printf("failed\n");
          return;
        }
        GdkPixbuf *pix=pari_ConvertOpenCv2Gtk(img, da->allocation.width, da->allocation.height );
        cairo_t *cr = gdk_cairo_create (gtk_widget_get_window(da));
        gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
        cairo_paint(cr);
        cairo_fill(cr);
        cairo_destroy(cr);
        g_object_unref(pix);  //free the pixbuf...
}
