/**
 *       @file  c_callback_parent.c
 *      @brief  Processamento de callback
 *
 * Processa o fecho da interface gráfica
 * Processamento de teclas precionadas
 * Processamento de teclas libertadas
 * Processamento do sinal de nova imagem
 * Envio de mensagem de controlo
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

#include "c_callback_parent.h"


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
gboolean pari_delete_event(GtkWidget * window, GdkEvent * event, gpointer data){
	//puts("Interface gráfica fechada.");
	gtk_main_quit();
    
	//puts("Pedido de delete event");
	return FALSE; //destroy window but does not leave gtk_main loop, and generates a "destroy" event
           
	//return TRUE; //ignore this delete_event request and does nothing. Same effect as returning 'void'
}


/**
 * @brief	Callback de key key_press
 	Processa imput do utilizador, actualiza variáveis
 	srvo1, servo2
 	move, direction
 * @param	GtkWidget *widget
 * @param	GdkEventKey *event
 * @param	gpointer user_data
 * @return	gboolean
 */
gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data){

	switch (event->keyval){
		case GDK_KEY_w:
			printf("key pressed - w\n");
			if(servo1<180){
				servo1++;
			}
			break;
		case GDK_KEY_s:
			printf("key pressed - s\n");
			if(servo1){
				servo1--;
			}
			break;
		case GDK_KEY_a:
			printf("key pressed - a\n");
			if(servo2){
				servo2--;
			}
			break;
		case GDK_KEY_d:
			printf("key pressed - d\n");
			if(servo2<180){
				servo2++;
			}
			break;
		case GDK_KEY_Up:
			printf("key pressed - Up\n");
			move = 'f';
			break;
		case GDK_KEY_Down:
			printf("key pressed - Down\n");
			move = 'b';
			break;
		case GDK_KEY_Left:
			printf("key pressed - Left\n");
			direction = 'l';
			break;
		case GDK_KEY_Right:
			printf("key pressed - Right\n");
			direction = 'r';
			break;
		default:
			return FALSE; 
	}
	fflush(stdout);
	return FALSE; 
}


/**
 * @brief	Callback de key release
	Processa imput do utilizador, actualiza variáveis
 	srvo1, servo2
 	move, direction
 * @param	GtkWidget *widget
 * @param	GdkEventKey *event
 * @param	gpointer user_data
 * @return	gboolean
 */
gboolean on_key_release (GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	switch (event->keyval){
		case GDK_KEY_w:
			printf("key release - w\n");
			break;
		case GDK_KEY_s:
			printf("key release - s\n");
			break;
		case GDK_KEY_a:
			printf("key release - a\n");
			break;
		case GDK_KEY_d:
			printf("key release - d\n");
			break;
		case GDK_KEY_Up:
			printf("key release - Up\n");
			move = 's';
			break;
		case GDK_KEY_Down:
			printf("key release - Down\n");
			move = 's';
			break;
		case GDK_KEY_Left:
			printf("key release - Left\n");
			direction = 's';
			break;
		case GDK_KEY_Right:
			printf("key release - Right\n");
			direction = 's';
			break;
		default:
			return FALSE; 
	}
	fflush(stdout);
	return FALSE; 
}


/**
 * @brief  Callback de receção de nova imagem
 * @param  int signum
 * @return none
 */
void ParentUSR1handler(int signum){
	// https://stackoverflow.com/questions/3906437/opencv-matrix-into-shared-memory
    
    // Converter Mat to IplImage
    // http://opencv-users.1802565.n2.nabble.com/Convert-cvMat-to-IplImage-td7472881.html
    cvGetImage( s, image );
    
    //printf("size image %d \n", img->imageSize); 
    
    cvCopy(image, img, 0);
    
    // cvShowImage("Parent image", img);
    
    pari_RefreshDrawingArea("drawingarea1", img );
}


/**
 * @brief  Callback por timeout - Envio da mensagem de controlo para o sistema remoto
 * @param  int *socket_desc - socket do cliente remoto
 * @return 1 - Continue timeout active
 */
int send_info(int *socket_desc){
	char message[32];
	sprintf(message, "control%c%c%.3d%.3d", move, direction, servo1, servo2);
    int ret = send(*socket_desc, message, strlen(message) , 0);
    if( ret < 0) { puts("Send failed"); }
    return 1;
}

