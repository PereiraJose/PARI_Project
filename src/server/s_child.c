/**
 *       @file  s_child.c
 *      @brief  Breve Descrição
 *
 * Comunicação TCP/IP com o cliente, receção da imagem, escrita na shared memory e sinal para o pai
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
 * @brief  Fix problem opencv 3 and c
 // https://stackoverflow.com/questions/42918747/yolo-c-compilation-failure-with-debug-1 
 * @param  double value
 * @return int value	retorna valor arredondado para cima
 */
int cvRound(double value) {return(ceil(value));}

/**
 * @brief  Inicializações
 comunicação com o cliente, envio de imagem da câmara
 * @param  int ppid - id do processo pai
 * @param  int argc - numero de parâmetros introduzidos na linha de comandos
 * @param  char *argv[] - array com os parâmetros introduzidos na linha de comandos
 * @return none
 */
void child(int ppid, int argc, char *argv[]){
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    int					server_Socket,
                        client_Socket,
                        //port = 4097;     
                        port = 0;                               

    struct  sockaddr_in server_Addr, client_Addr;
    int addrLen = sizeof(struct sockaddr_in);
    char clntName[INET_ADDRSTRLEN];			// String to contain client name
    
    //To avoid defuncts due to the fork() below. 
	//It is actually the default action and occurs when Child stopped or terminated
	signal(SIGCHLD, SIG_IGN);

	//To catch CTRL_C and allow the closure of port
	signal(SIGINT, ManageCTRL_C);

    
	port = atoi(argv[1]) + 1;
	printf("port child %d\n", port);

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
	
	printf("Child waiting for connections...\n");
    
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
		
		/*notify that you know the client: this is optional */
		if(inet_ntop (AF_INET, &client_Addr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("Child: Client %s connected/%d\n", clntName, ntohs(client_Addr.sin_port));
		else
			printf("Unable to get client address\n");
			
		display(client_Socket, clntName);
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
    
    return;
}


/**
 * @brief  Leitura de imagem da câmara
 * @param  int client_Socket socket do cliente para enviar imagem
 * @return none
 */
void display(int client_Socket, char *clientID){

    //OpenCV Code 
	IplImage *img = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	
	CvCapture *capture = cvCaptureFromCAM (0);	//Capture using 1st camera: i.e., no. 0
	
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, IMAGE_WIDTH );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, IMAGE_HEIGHT );
	
	img = cvQueryFrame (capture); // get a frame from camera
    

	int imgSize = img->imageSize;
    uchar *iptr = img->imageData;
    
    
    int bytes = 0;
    
    const int BUFFSIZE=32;
    char inbuffer[BUFFSIZE];
	
    while(infinite_loopG) {
		//Update frame. Its pointer does not change.
		cvQueryFrame (capture);
		
		//video processing here 
		//cvtColor(img, imgGray, CV_BGR2GRAY);

		//send processed image
		if ((bytes = send(client_Socket, img->imageData, imgSize, 0)) < 0){
			printf("CHILD: error sending!!!\n");
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
	
	printf("\nCHILD: Connection to client %s terminated.\n", clientID);
}
