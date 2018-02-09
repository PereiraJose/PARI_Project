/**
 *       @file  c_common.h
 *      @brief  Ficheiro com includes e funções comums aos vários ficheiros, declaração das variáveis globais
 *
 *
 *     @author  Jose, jose.paulo@ua.pt
 *
 *   @internal
 *     Created  26-Jan-2018
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2018, Jose
 *
 * =====================================================================================
 */
 
#ifndef __C_COMMON_H__
#define __C_COMMON_H__


// GTK
#include <gtk/gtk.h>
#include <cairo.h>

// Shared memory
#include <sys/ipc.h>
#include <sys/shm.h>

// OpenCV
#include <cv.h>
#include <highgui.h>

#include <math.h>

// TCPIP
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

#include "../defines_common.h"

#define SHM_SIZE IMAGE_WIDTH*IMAGE_HEIGHT*3


#if defined (_MAIN_C_)
	GtkBuilder *builderG;
	//IplImage *dst_imageG , *src_imageG;
	
	CvMat *s = NULL;
	IplImage* image = NULL;
	IplImage *img = NULL;
	char move = 's'; // f - forward b - back
	char direction = 's'; // l - left r - right
	int add_servo1 = 0;
	int add_servo2 = 0;
	int servo1 = 90;
	int servo2 = 90;
	
	int infinite_loopG = 1;
	
	char *data_p_img; //generic pointer to serve as link for the shared memory
	
#else
	extern GtkBuilder *builderG;
	//extern IplImage *dst_imageG , *src_imageG;
	
	extern CvMat *s;
	extern IplImage* image;
	extern IplImage* img;
	extern char move; // f - forward b - back
	extern char direction; // l - left r - right
	
	extern int add_servo1;
	extern int add_servo2;
	extern int servo1; // angle servo1
	extern int servo2; // angle servo2
	
	extern char *data_p_img;
	
	extern int infinite_loopG;
#endif

// https://stackoverflow.com/questions/42918747/yolo-c-compilation-failure-with-debug-1
int cvRound(double value);

int GetSharedMem(void); 
 
#endif /* __C_COMMON_H__ */
 
 

