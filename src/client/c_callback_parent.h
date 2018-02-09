/**
 *       @file  c_callback_parent.h
 *      @brief  Declaração das funções de callback do cliente
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


#ifndef __C_CALLBACK_PARENT_H__
#define __C_CALLBACK_PARENT_H__

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "c_common.h"
#include "c_parent.h"


gboolean pari_delete_event(GtkWidget * window, GdkEvent * event, gpointer data);
gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean on_key_release (GtkWidget *widget, GdkEventKey *event, gpointer user_data);

int send_info(int *socket_desc);
void ParentUSR1handler(int signum);

#endif /* __C_CALLBACK_PARENT_H__ */

