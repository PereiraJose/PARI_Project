/**
 *       @file  c_child.h
 *      @brief  Breve Descrição
 *
 * Descrição mais detalhada do ficheiro que até poderiam incluir links para imagens etc.
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

int child(int ppid, int argc, char *argv[]);
void ChildUSR2handler(int signum);
#endif /* __C_CHILD_H__ */
 
