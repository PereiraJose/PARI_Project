/**
 *       @file  c_main.c
 *      @brief  Main aula 9b PARI
 *
 *
 *     @author  José Pereira, jose.paulo@ua.pt
 *
 *   @internal
 *     Created  25-Oct-2017
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2017, Jose
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>     //strlen
#include <stdlib.h>

#define _MAIN_C_

#include "c_child.h"
#include "c_parent.h"


/**
 * @brief  
 * @param  
 * @return 
 */
int main(int argc, char *argv[]){
	int pid = fork();
	
    if(pid == -1) { printf("Could not fork(). Exiting\n"); return -1; }

	/* The child */
    if(pid == 0) {
    	printf("\nChild\n");
        
        child( getppid(), argc, argv);
    	
		printf("Exit child\n");
		fflush(stdout);
    }
    /* The parent */
    else {
    
    	
    	printf("\nParent\n");
    	
    	
    	parent(pid, argc, argv);
		
		kill(pid, SIGUSR2);  //send signal to child
		
    	printf("Exit Parent\n");
    	fflush(stdout);
    }
	
	
	//cvReleaseImage(&dst_imageG);             //Release image (free pointer when no longer used)
	//cvReleaseImage(&src_imageG);             //Release image (free pointer when no longer used).

	
	printf("\nProgram TERMINATED\n");

    return 0;
}






