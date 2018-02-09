/**
 *       @file  s_child.h
 *      @brief  Declaração das funções de funções usadas no processo filho
 *
 *
 *     @author  Jose, jose.paulo@ua.pt
 *
 *   @internal
 *     Created  02-Fev-2018
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2018, Jose
 *
 * =====================================================================================
 */

#ifndef __S_CHILD_H__
#define __S_CHILD_H__

// OpenCV

#include <cv.h>
#include <highgui.h>
#include "opencv2/videoio/videoio_c.h"

#include <math.h>

#include "s_common.h"


void child(int ppid, int argc, char *argv[]);
void display(int client_Socket);

// https://stackoverflow.com/questions/42918747/yolo-c-compilation-failure-with-debug-1
int cvRound(double value);


#endif /* __S_CHILD_H__ */

