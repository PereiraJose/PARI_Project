/**
 *       @file  c_child.h
 *      @brief  Declaração das funções de funções usadas no processo filho
 *
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

#ifndef __C_CHILD_H__
#define __C_CHILD_H__

#include "c_common.h"

void child(int ppid, int argc, char *argv[]);
void ChildUSR2handler(int signum);
#endif /* __C_CHILD_H__ */
 
