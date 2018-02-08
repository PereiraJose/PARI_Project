/**
 *       @file  s_parent.h
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

#ifndef __S_PARENT_H__
#define __S_PARENT_H__


#include "s_common.h"

int parent(int cpid, int argc, char *argv[]);
void processclient(int sock, char *clientID);
void RespondAlarm(int signum);
int InitTimer( void (* func)(int) );

#endif /* __S_PARENT_H__ */

