#ifndef _MYF_H
#define _MYF_H

#include <gtk/gtk.h>
#include <cv.h>
#include <highgui.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <gtk/gtk.h>

#define MAXFFBUFFER 1000
typedef struct {
  int count;
  double x[MAXFFBUFFER];
  double y[MAXFFBUFFER];
} free_form;

#ifdef _MAIN_C_
	int count_parentG = 0;
	int count_childG = 0;
	int pid_childG;
	GtkBuilder *builderG;
	double xoffG=100;
	double yoffG=120;
	free_form ffG={0};   //init count to zero. Do not affect x and y.
	double fontSizeFactorG=1;
	GtkBuilder *builderG;
     IplImage *dst_imageG , *src_imageG;
     CvCapture *captureG;
#else
//Variáveis globais (externas)
	extern int count_parentG;
	extern int count_childG;
	extern int pid_childG;
	extern GtkBuilder *builderG;
	extern double xoffG;
	extern double yoffG;
	extern free_form ffG;
	extern double fontSizeFactorG;
	extern GtkBuilder *builderG;
     extern IplImage *dst_imageG , *src_imageG;
     extern CvCapture *captureG;
#endif

/* Some definitions*/
#define NAME_IMG_ORG "ImagemOriginal"
#define NAME_IMG_BIN "ImagemBinarizada"
#define NAME_IMG_TAB "ImagemTabuleiro"
#define NAME_IMG_PRC "ImagemProcessada"

#define SHM_SIZE 128 /**< 128 bytes for shared memory */
#define PARENTDELAY 300000   /**<parent period in us*/
#define CHILDDELAY 900000    /**<child delay in us*/

#define ID "Ruben Esteves, 86297"
#define TIC tictoc(0)
#define TOC tictoc(1)

typedef int byte;
typedef struct
{
	double r, i;
} complexo;
typedef struct
{
	int horas, minutos, segundos;
} tempo;
typedef struct
{
	int totA, totE, totI, totO, totU, totY;
} vStat;

typedef enum {imBIN=0, imCANNY, imERODE, imROTATE} im_operation;



#include "prototypes.h"

#endif /* _MYF_H */
