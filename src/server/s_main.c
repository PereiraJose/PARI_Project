/**
 *       @file  s_main.c
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



#include <string.h>     //strlen


#define _MAIN_C_

#include "s_child.h"
#include "s_parent.h"


/**
 * @brief  
 * @param  
 * @return 
 */
int main(int argc, char *argv[]){
	int pid = fork();
	
    if(pid == -1) { printf("Could not fork(). Exiting\n"); return -1; }
	
	
	/* The child */ // envia imagens para o client
    if(pid == 0) {
    	printf("\nChild\n");
        
        child( getppid(), argc, argv);
    	
		printf("Exit child\n");
		fflush(stdout);
    }
    /* The parent */ // recebe mensagens de controlo do client
    else {
    
    	
    	printf("\nParent\n");
    	
    	parent(pid, argc, argv);
		
    	printf("Exit Parent\n");
    	fflush(stdout);
    }
	
		
	printf("\nProgram TERMINATED\n");

    return 0;
}
