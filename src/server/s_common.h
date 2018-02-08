/**
 *       @file  s_common.h
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
 
#ifndef __S_COMMON_H__
#define __S_COMMON_H__


// TCPIP
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#include <string.h>

#include <signal.h>


#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>		//header file para gettimeofday e struct timeval

// Mensagens de erro
#include <errno.h>

#include "../defines_common.h"


#ifdef _MAIN_C_
	int infinite_loopG=1;
#else
	extern int infinite_loopG;
#endif


void ManageCTRL_C(int dummy);
void myerror(const char *msg);

 
#endif /* __S_COMMON_H__ */
 
 

