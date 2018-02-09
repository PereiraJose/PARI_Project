/**
 *       @file  s_main.c
 *      @brief  Ficheiro principal do programa do servidor remoto
 *
 * Verifica quantidade de argumentos se é a correta
 * Print de mensagem informativa de como usar
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
	
	if (argc < 2) {
        printf("  Usage: server <serverPort>\n");
        printf("Example: server 5000\n");
    	return 0;
    }
    
    printf("Terminar programa com CTRL-C\n");


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
