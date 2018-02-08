/**
 *       @file  c_parent.h
 *      @brief  Breve Descrição
 *
 * Descrição mais detalhada do ficheiro que até poderiam incluir links para imagens etc.
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

#ifndef __C_PARENT_H__
#define __C_PARENT_H__

#include "c_callback_parent.h"
#include "c_common.h"




int parent(int cpid, int argc, char *argv[]);

GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage * image, int dst_w, int dst_h);
void pari_RefreshDrawingArea( char * widgetName, IplImage *img);
 
#endif /* __C_PARENT_H__ */
 
