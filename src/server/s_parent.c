/**
 *       @file  s_parent.c
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

#include "s_parent.h"


#ifdef OrangePI
// https://github.com/zhaolei/WiringOP
#include<wiringPi.h>
#include<softServo.h>


#define FORWARD 0
#define REVERSE 7
#define LEFT    8
#define RIGHT   9

#endif

char move = 's';
char direction = 's';
int servo1 = 0;
int servo2 = 0;

int update = 0;


/**
 * @brief  
 * @param  
 * @return 
 */
int parent(int cpid, int argc, char *argv[]){

#ifdef OrangePI
	if (wiringPiSetup () == -1){
		fprintf (stdout, "oops: %s\n", strerror (errno)) ;
		return 1 ;
	}
	softServoSetup (1, 1, 2, 3, 1, 1, 1, 1) ;
	pinMode(FORWARD, OUTPUT);
	pinMode(REVERSE, OUTPUT);
	pinMode(LEFT, OUTPUT);
	pinMode(RIGHT, OUTPUT);
#endif




	int r=InitTimer( RespondAlarm );  //Prepare timer and set RespondAlarm as the callback to be executed
    if (! r ) return 0;               //failed in creating the timer

	int sockfd, clientsockfd, portno, pid;
	int ret;
	char clntName[INET_ADDRSTRLEN];			// String to contain client name
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	//To avoid defuncts due to the fork() below. 
	//It is actually the default action and occurs when Child stopped or terminated
	//signal(SIGCHLD, SIG_IGN);

	//To catch CTRL_C and allow the closure of port
	signal(SIGINT, ManageCTRL_C);

	if(argc < 2){
		fprintf(stderr, "Syntax: %s <port_num>\n", argv[0]);
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//create a INET socket to communicate
	if(sockfd < 0) myerror("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));	//fill with zeros
	portno                    = atoi(argv[1]);	//get port number from command line
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	serv_addr.sin_port        = htons(portno);
	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));	//bind the name to the socket
	if(ret < 0) myerror("ERROR on binding");
	
	
	//start waiting for connections on the socket up to a maximum of 20 connections
	listen(sockfd, 1);
	clilen = sizeof(cli_addr);

	//make socket non blocking so other events may be processed inside the infinite loop
	//An equivalent action could also be done at the socket creation...
	int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);  

	while(infinite_loopG){
		//accept the connection
		clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);	
		if(clientsockfd < 0){  //some error occured.
			//simply absence of data on nonblocking socket. Not really an error. 
			if (errno == EAGAIN) continue;  
			myerror("ERROR on accept");
		}
		//else
		//	break;

		/*notify that you know the client: this is optional */
		if(inet_ntop (AF_INET, &cli_addr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("PARENT: Client %s connected/%d\n", clntName, ntohs(cli_addr.sin_port));
		else
			printf("Unable to get client address\n");
	
		processclient(clientsockfd, clntName);
		close(clientsockfd);	
		
		/*
		//create a process child to manage the connection
		pid = fork();	                         
		if(pid < 0) myerror("ERROR on fork");//rare but may occur!
		
		//It is the child... proceed accordingly
		if(pid == 0){
			//child no longer needs the parent socket. Close it, after all it's simply a handle
			close(sockfd);

			//Cf. http://stackoverflow.com/questions/14427898/socket-from-parent-to-child
			processclient(clientsockfd, clntName);	 //Process client request (a new process)
			
			//Exits when finishing processing the client request
			exit(0);
		}
		//It is the parent... does nothing: closes the client socket and resume accepting...
		else
			close(clientsockfd);
		*/
	}/* end of while */

	close(sockfd);
	
	/* we only get here when someone breaks the infinite loop */
	return 0;
}


/**
 * @brief  Function to process client request
 *
 * There will be a separate instance of this function 
 * for each connection established by clients. It handles 
 * all communications once a connection has been established.
 *
 * @param  sock The socket ID
 * @param  clientID The client ID (normally the IP)
 * @return  Nothing. It will return to terminate immediatly
 */
void processclient(int sock, char *clientID){
	const int BUFFSIZE=64;		/*If this is small, enlarge it or use dynamic allocation */
	int n, numBytesSent, bufLen;
	char inbuffer[BUFFSIZE], outbuffer[BUFFSIZE], *str, lastbuffer[BUFFSIZE];
	int keepGoing=1;

	while(keepGoing && infinite_loopG){
		bzero(inbuffer, BUFFSIZE);	//empty buffer
		n = recv(sock, inbuffer, BUFFSIZE-1,0);
		if(n == BUFFSIZE-1){	//means read the entire buffer... there is the chance that more data may be waiting
			printf("Buffer filled\n");
			//mysocketflush(sock);
			//in that case additional memory should be allocated
			//to read the remainder of the message and build it entirely to deal with it.
			//Not done in this example but may be necessary if large bunchs of data were sent from client.
		}
		if(n < 0) myerror("ERROR reading from socket");

		
		// printf("Message received from the client %s: %s\n", clientID, inbuffer);
		
		if(!strncmp(inbuffer, "quit", 4)){
			sprintf(outbuffer, "Client %s, you have requested to close the connection!", clientID);
			bufLen = strlen(outbuffer);
			keepGoing=0;
		}
		else if(!strncmp(inbuffer, "control", 7)){
			update = 0;
		
			char *ss = &inbuffer[9];
			//printf("number %d", atoi(ss));
			
			move = inbuffer[7];
			direction = inbuffer[8];
			servo1 = (int) atoi(ss)/1000;
			servo2 = atoi(ss) - servo1*1000;
			
			if(strncmp(inbuffer, lastbuffer, 15)){
				printf("move %c\n", move);
				printf("direction %c\n", direction); 
				printf("servo1 %.3d\n", servo1);
				printf("servo2 %.3d\n", servo2);
			}
		}
		else{
			sprintf(outbuffer, "Client %s, your message is not recognized!", clientID);
			bufLen = strlen(outbuffer);
		}
		strcpy (lastbuffer, inbuffer);
		//here send the reply to the client
		//numBytesSent = send(sock, outbuffer, bufLen, 0);
		//if(numBytesSent < 0) myerror("send()/write() failed");
	}
	printf("\nPARENT: Connection to client %s terminated.\n", clientID);
}


/**
 * @brief  
 * @param  
 * @return 
 */
int InitTimer( void (* func)(int) ){
	// Timers decrement from it_value to zero, generate a signal, and reset to it_interval.
	// A timer which is set to zero (it_value is zero or the timer expires and it_interval is zero) stops.

	const struct itimerval vValue = { {0, 50000}, //it_interval: next value (sec, usecs) - interval of repetition (1 sec)
                                          {1,0}  //it_value: initial value (sec, usecs) - initial delay (1 seconds here)
                                    };
	int a=setitimer(ITIMER_REAL, & vValue, NULL);
	if(a){
		perror("Setting timer:");  //Show what was the error
		return 0;  //failure
	}
	signal(SIGALRM, func);   //prepare the appropriate callback upon signal occurence
	
	return 1;   //success
}



/**
 * @brief  
 * @param  
 * @return 
 */
#ifdef OrangePI

void RespondAlarm(int signum){
	if(update > 6){
		if(update==7){
	        printf("reset\n");
	        fflush(stdout);
	        move = 's';
	        direction = 's';
	        update=200;
		}
	}
	else
		update++;
	
	
	if (servo1)
		softServoWrite (2, servo1*1400/180+800);
	else
		softServoWrite (2, 800);
	if (servo2)
		softServoWrite (3, servo2*1400/180+800);
	else
		softServoWrite (3, 800);
	
	
	
	//activate io
	if(move=='f'){
		digitalWrite (FORWARD, LOW) ;
		digitalWrite (REVERSE, HIGH) ;
	}
	else if(move=='b'){
		digitalWrite (FORWARD, HIGH) ;
		digitalWrite (REVERSE, LOW) ;
	}
	else{
		digitalWrite (FORWARD, HIGH) ;
		digitalWrite (REVERSE, HIGH) ;
	}

	if(direction=='l'){
		digitalWrite (LEFT, HIGH) ;
		digitalWrite (RIGHT, LOW) ;
	}
	else if(direction=='r'){
		digitalWrite (LEFT, LOW) ;
		digitalWrite (RIGHT, HIGH) ;
	}
	else{
		digitalWrite (LEFT, LOW) ;
		digitalWrite (RIGHT, LOW) ;
	}
}
#else
void RespondAlarm(int signum){
	static int a = 0;
	static int b = 0;
	
	if(a>100){
		printf("Live %d\n", b);
		b++;
		a=0;
	}
	else
		a++;
		
    if(update > 1){
    	
    	if(update==2)
    		printf("Reset output");
    	if(update<3)
    		update++;
    	//desactivate io
    }	
    else{
    	update++;
    	//activate io
    }
    
    fflush(stdout);
}
#endif
