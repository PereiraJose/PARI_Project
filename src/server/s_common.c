/**
 *       @file  s_common.c
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

#include "s_common.h"

/**
 * @brief  
 * @param  
 * @return 
 */
void myerror(const char *msg){
	perror(msg);
	exit(1);
}


/**
 * @brief  
 * @param  
 * @return 
 */
void ManageCTRL_C(int dummy){
	printf("User CTRL-C pressed. Closing server.\n");
	infinite_loopG=0;
}
