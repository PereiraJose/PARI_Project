/**
 *       @file  c_main.c
 *      @brief  Ficheiro principal do programa do cliente
 * 
 * Verifica quantidade de argumentos se é a correta
 * Print de mensagem informativa de como usar
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
 * @brief  Programa divide-se em dois
 ver child(ppid, argc, argv) e parent(cpid, argc, argv)
 * @param  int argc - numero de parâmetros introduzidos na linha de comandos
 * @param  char *argv[] - array com os parâmetros introduzidos na linha de comandos
 * @return 0
 */
int main(int argc, char *argv[]){

	//http://www.network-science.de/ascii/
	printf("___  _________  _____  \n");
	printf("|  \\/  || ___ \\/  ___| \n");
	printf("| .  . || |_/ /\\ `--.  \n");
	printf("| |\\/| ||    /  `--. \\ \n");
	printf("| |  | || |\\ \\ /\\__/ / \n");
	printf("\\_|  |_/\\_| \\_|\\____/  \n\n");
	
	if (argc < 3) {
        printf("Usage: client <serverIP> <serverPort>\n");
    	return 0;
    }


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
	
	printf("\nProgram TERMINATED\n");
    return 0;
}






