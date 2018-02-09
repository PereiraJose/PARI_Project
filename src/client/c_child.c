/**
 *       @file  c_child.c
 *      @brief  Receção de imagem
 *
 * Comunicação TCP/IP com o servidor, receção da imagem, escrita na shared memory e sinal para o pai
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

#include "c_child.h"

 
/**
 * @brief  Inicializações
 comunicação com o servidor, receção da imagem, escrita na shared memory e sinal para o pai
 * @param  int ppid - id do processo pai
 * @param  int argc - numero de parâmetros introduzidos na linha de comandos
 * @param  char *argv[] - array com os parâmetros introduzidos na linha de comandos
 * @return 
 */
void child(int ppid, int argc, char *argv[]){
	printf("\nChild\n");
	signal(SIGUSR2, ChildUSR2handler);
	
	usleep(1000000); //Must give time to operating system to register signal, otherwise process exits prematurely if it receives it earlier
	
	//--------------------------------------------------------
    //networking stuff: socket , connect
    //--------------------------------------------------------
    int         soket;
    char*       serverIP;
    int         serverPort;


    serverIP   = argv[1];
    serverPort = atoi(argv[2]) + 1;

    struct  sockaddr_in address;

    if ((soket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed\n");
        return;
    }

    address.sin_family = PF_INET;
    address.sin_port = htons(serverPort);
    address.sin_addr.s_addr = inet_addr(serverIP);

    if (connect(soket, (struct sockaddr*) &address, sizeof(address)) < 0) {
        // std::cerr << "connect() failed!" << std::endl;
        printf("connect() failed!\n");
        return;
    }
	
	printf("Child connected\n");
	
    //----------------------------------------------------------
    //OpenCV Code
    //----------------------------------------------------------
    IplImage *img = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);

    int imgSize = img->imageSize;
    uchar *iptr = img->imageData;
    
    //printf("size image %d \n", imgSize); //230400
    
    int bytes = 0;
    int key;
	
	CvMat *s = cvCreateMat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
	CvMat *tmp = cvCreateMat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
	CvMat stub;

	//----------------------------------------------------------
    //Shared Memory
    //----------------------------------------------------------
	char *data; //generic pointer to serve as link for the shared memory
	int shm_id;

	shm_id = GetSharedMem();
	if(shm_id == -1) exit(-1);     //failure

	/* attach to the memory segment to get a pointer to it */
	data = shmat(shm_id, (void *) 0, 0);
	if(data == (char *) (-1)){
		perror("shmat");
		exit(1);
	}

	s->data.ptr = data;
	
    while (key != 'q' && infinite_loopG) {
		bytes = recv(soket, iptr, imgSize , MSG_WAITALL);
        if (bytes == -1) {
            printf("recv failed, received bytes = %d", bytes);
        }
        
        send(soket, "OK", strlen("OK"), 0);
        
        //printf("bytes %d\n", bytes);
        //cvShowImage ("CV Video Client", img);
		
        //write image to shared memory
        /* Get matrix from the image frame and write the matrix in shared memory*/
        tmp = cvGetMat(img, &stub, 0, 0);

        for (int row = 0; row < tmp->rows; row++) {
            const char* ptr = (const char*) (tmp->data.ptr + row * tmp->step);
            memcpy((char*)(s->data.ptr + row * s->step), ptr, tmp->step);
        }
        
        //sinal nova imagem
        kill(ppid, SIGUSR1);  //send signal to parent
        
        // if (key = cvWaitKey (30) >= 0) break;
        cvWaitKey (1);
    }
    
    //Envia informação para servidor para terminar conecção
	send(soket, "quit", strlen("quit"), 0);
	
	/* detach from the mem segment since it is leaving */
	if(  shmdt(data) == -1 ){
		perror("shmdt");
		exit(1);
	}
	
	//Allow elimination of shared memory
	if(shm_id > 0) shmctl(shm_id, IPC_RMID, NULL);
	
	//cvDestroyWindow("CV Video Client");
	
	/*
	cvReleaseImage (&img);
	cvReleaseMat (&s);
	cvReleaseMat (&tmp);
	*/
	
	//close the socket before exiting
    close(soket);
}

/**
 * @brief  Receção de callback do pai a indicar para terminar
 * @param  int signum
 * @return none
 */
void ChildUSR2handler(int signum){
	printf("Exit child.\n");
	infinite_loopG=0;
}
