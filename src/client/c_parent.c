/**
 *       @file  c_parent.c
 *      @brief  Interface gráfica
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

#include <X11/Xlib.h>
    

#include "c_parent.h"


/**
 * @brief  Inicializações
 Comunicação de controlo com o servidor remoto
 Inicialização da interface gráfica
 * @param  int cpid - id do processo filho
 * @param  int argc - numero de parâmetros introduzidos na linha de comandos
 * @param  char *argv[] - array com os parâmetros introduzidos na linha de comandos
 * @return none
 */
void parent(int cpid, int argc, char *argv[]){
	signal(SIGUSR1, ParentUSR1handler);
	usleep(100000); //Must give time to operating system to register signal, otherwise process exits prematurely if it receives it earlier
	
	
	//https://stackoverflow.com/questions/18647475/threading-problems-with-gtk
	XInitThreads();
	
	// OpenCV init

	s = cvCreateMat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
	cvZero(s);
    image = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	cvZero(image);
	
	
	
	
	// Shared memory	
		
	int shm_id;
	shm_id = GetSharedMem();
	if(shm_id == -1) exit(-1);     //failure

	/* attach to the memory segment to get a pointer to it */
	data_p_img = shmat(shm_id, (void *) 0, 0);
	if(data_p_img == (char *) (-1)){
	    perror("shmat");
	    exit(1);
	}
	
	s->data.ptr = data_p_img;
	
	
	// Connect to server to send control data
	
	int socket_desc;
    int ret;
    struct sockaddr_in server;
    
    
    char *ip = argv[1];
    int  port =	atoi(argv[2]);
    
    printf("PORT: %d\n", port);
    
    //Create a socket for communications
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) printf("Could not create socket\n");

    //fill server details
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
		
	
    //Connect to remote server using the created socket
    ret = connect(socket_desc, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0) { puts("connect error\n"); return; }

    printf("Parent connected\n");
	
	
	
	// Start GTK
	
	gtk_init(&argc, &argv);

	/* load the interface after a configuration file*/
	builderG = gtk_builder_new();
	gtk_builder_add_from_file(builderG, "PARI.glade", NULL);

	/* connect the signals in the interface */
	gtk_builder_connect_signals(builderG, NULL);

	/* get main window Widget ID and connect special signals */
	GtkWidget *t = GTK_WIDGET(gtk_builder_get_object(builderG, "window1"));
	if(t){
		g_signal_connect(G_OBJECT(t), "delete_event", G_CALLBACK(pari_delete_event), NULL);
	}


	img = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	
	// assossiate callback to send control to server every 50 miliseconds
	int *ptr_socket;
	ptr_socket = &socket_desc;
	g_timeout_add(50, (GSourceFunc) send_info, (gpointer) ptr_socket);
	
	
	// keyboard input
	// https://stackoverflow.com/questions/10134956/in-simple-gtk-key-press-event-example-gdk-shift-mask-seems-to-be-ignored
	g_signal_connect (G_OBJECT(t), "key_press_event", G_CALLBACK (on_key_press), NULL);
	//https://stackoverflow.com/questions/43127356/in-gtk3-c-while-connected-to-key-release-event-how-to-avoid-initial-return-pr
	g_signal_connect (G_OBJECT(t), "key_release_event", G_CALLBACK (on_key_release), NULL);
	// keyboard
	// https://git.gnome.org/browse/gtk+/plain/gdk/gdkkeysyms.h
	
	
	/* start the event loop */
	gtk_main();


	//Send message to close connection to server
	char message[16];
	sprintf(message, "quit");
    ret = send(socket_desc, message, strlen(message) , 0);
    if( ret < 0) { puts("Send failed");}
	
	
    /* detach from the mem segment since it is leaving */
	if(  shmdt(data_p_img) == -1 ){
	    perror("shmdt");
	    exit(1);
	}
	
	//Allow elimination of shared memory
	if(shm_id > 0) shmctl(shm_id, IPC_RMID, NULL);
	
	//close the socket before exiting
    close(socket_desc);
	
	return;
}


/**
 * @brief  Exemplo de código de uma função que converte uma IplImage num pixbuf para inserir numa interface GTK e ajusta-a com certas dimensões passadas como argumentos
 * @param  IplImage * image - Imagem
 * @param  int dst_w - largura janela de destino
 * @param  int dst_h - altura janela de destino
 * @return GdkPixbuf *
 */
GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage * image, int dst_w, int dst_h){
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


/**
 * @brief  copy from IplImage to pixbuf and paint DA
 * @param  char * widgetName Nome do widget
 * @param  IplImage *img Imagem
 * @return void
 */
void pari_RefreshDrawingArea( char * widgetName, IplImage *img){
	GtkWidget *da = GTK_WIDGET(gtk_builder_get_object (builderG, widgetName));
    if( ! da ){
      printf("failed\n");
      return;
    }
    //GdkPixbuf *pix=pari_ConvertOpenCv2Gtk(img, da->allocation.width, da->allocation.height );
    GdkPixbuf *pix = pari_ConvertOpenCv2Gtk(img, gtk_widget_get_allocated_width (da), gtk_widget_get_allocated_height (da) );
    cairo_t *cr = gdk_cairo_create (gtk_widget_get_window(da));
    gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
    cairo_paint(cr);
    cairo_fill(cr);
    cairo_destroy(cr);
    g_object_unref(pix);  //free the pixbuf...
}

