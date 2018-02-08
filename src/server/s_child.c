/**
 *       @file  s_child.c
 *      @brief  Breve Descrição
 *
 * Descrição mais detalhada do ficheiro que até poderiam incluir links para imagens etc.
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

#include "s_child.h"

/**
 * @brief  
 * @param  
 * @return 
 */
int cvRound(double value) {return(ceil(value));}

/**
 * @brief  
 * @param  
 * @return 
 */
int child(int ppid, int argc, char *argv[]){
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    int					server_Socket,
                        client_Socket,
                        //port = 4097;     
                        port = 0;                               

    struct  sockaddr_in server_Addr, client_Addr;
    int addrLen = sizeof(struct sockaddr_in);
    
    
    //To avoid defuncts due to the fork() below. 
	//It is actually the default action and occurs when Child stopped or terminated
	signal(SIGCHLD, SIG_IGN);

	//To catch CTRL_C and allow the closure of port
	signal(SIGINT, ManageCTRL_C);

    

	
    if ( (argc > 1) && (strcmp(argv[1],"-h") == 0) ) {
          /*
          std::cerr << "usage: server [port] [capture device]\n" <<
                       "port           : socket port (4097 default)\n" <<
                       "capture device : (0 default)\n" << std::endl;
          */
          exit(1);
    }

    if (argc == 2) port = atoi(argv[1]) + 1;
	printf("port %d\n", port);

    server_Socket = socket(AF_INET , SOCK_STREAM , 0);
	if(server_Socket < 0) myerror("ERROR opening socket");  

    server_Addr.sin_family 		= AF_INET;
    server_Addr.sin_addr.s_addr = INADDR_ANY;
    server_Addr.sin_port 		= htons( port );


    if( bind(server_Socket,(struct sockaddr *)&server_Addr, sizeof(server_Addr)) < 0) {
         myerror("Can't bind() socket");
    }
    
    //start waiting for connections on the socket up to a maximum of 2 connections
    listen(server_Socket , 1);
	
	printf("Waiting for connections...\n");
    
	//make socket non blocking so other events may be processed inside the infinite loop
	//An equivalent action could also be done at the socket creation...
	int flags = fcntl(server_Socket, F_GETFL, 0); fcntl(server_Socket, F_SETFL, flags | O_NONBLOCK); 	
	
	int pid;
    //accept connection from an incoming client
    while(infinite_loopG){
		client_Socket = accept(server_Socket, (struct sockaddr *)&client_Addr, (socklen_t*)&addrLen);  
		if(client_Socket < 0){  //some error occured.
			//simply absence of data on nonblocking socket. Not really an error. 
			if (errno == EAGAIN) continue;  
			myerror("ERROR on accept");
		}
		//else 
		//	break;
			
		printf("CHILD: New Client\n");
		display(client_Socket);
		close(client_Socket);
		/*
		//create a process child to manage the connection
		pid = fork();	                         
		if(pid < 0) myerror("ERROR on fork");//rare but may occur!
		
		//It is the child... proceed accordingly
		if(pid == 0){
			//child no longer needs the parent socket. Close it, after all it's simply a handle
			close(server_Socket);

			//Cf. http://stackoverflow.com/questions/14427898/socket-from-parent-to-child
			display(client_Socket);	 //Process client request (a new process)
			
			//Exits when finishing processing the client request
			exit(0);
		}
		//It is the parent... does nothing: closes the client socket and resume accepting...
		else
			close(client_Socket);
		*/
		
		
    }/* end of while */
    close(server_Socket);
    
    return 0;
}


/**
 * @brief  
 * @param  
 * @return 
 */
void display(int client_Socket){

    //OpenCV Code 
	IplImage *img = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	
	CvCapture *capture = cvCaptureFromCAM (0);	//Capture using 1st camera: i.e., no. 0
	
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, IMAGE_WIDTH );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, IMAGE_HEIGHT );
	
	img = cvQueryFrame (capture); // get a frame from camera
    

	int imgSize = img->imageSize;
    uchar *iptr = img->imageData;
    
    
    int bytes = 0;
    
    const int BUFFSIZE=16;
    char inbuffer[BUFFSIZE];
	
    while(infinite_loopG) {
		//Update frame. Its pointer does not change.
		cvQueryFrame (capture);
		
		//video processing here 
		//cvtColor(img, imgGray, CV_BGR2GRAY);

		//send processed image
		if ((bytes = send(client_Socket, img->imageData, imgSize, 0)) < 0){
			printf("CHILD: error sending!!!");
			break;
		}
		
		//verify if client alive
		bytes = recv(client_Socket, inbuffer, BUFFSIZE-1, 0);
		if(bytes < 0) break;
		if(!strncmp(inbuffer, "quit", 4)) break;
		
		//printf("CHILD: send OK");
		
		cvWaitKey (50);
		
	}
	
	 cvReleaseCapture(&capture);
	
	printf("CHILD: Exit Client\n");
	//exit(1);
	
	
}
