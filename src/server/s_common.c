/**
 *       @file  s_common.c
 *      @brief  Funções comums aos vários ficheiros de código
 *
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

#include "s_common.h"

/**
 * @brief  Print na consola da mensagem de erro e termina programa
 * @param  const char *msg - mensagem de erro
 * @return none
 */
void myerror(const char *msg){
	perror(msg);
	exit(1);
}


/**
 * @brief  Callback que interpreta CTRL-C para terminar o servidor e fechar os sockets
 * @param  int dummy
 * @return none
 */
void ManageCTRL_C(int dummy){
	printf("User CTRL-C pressed. Closing server.\n");
	infinite_loopG=0;
}
