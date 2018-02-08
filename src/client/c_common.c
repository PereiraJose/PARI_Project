/**
 *       @file  c_common.c
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

#include "c_common.h"



 /**
 * @brief  Gets a shared memory and connects to it
 *
 * The function first creates a random key to identify
 * the memory and then connects to it with some specific permissions
 * @return int The Shared memory ID. Returns -1 in case of failure.
 */
int GetSharedMem(void){
    key_t key;                       /*it's a long int */
    int shm_id;
    key = ftok("PARI.glade", 'X');       /* generate a random key based on a file name and char */
    //key = ftok("/", 'X');
    //key = ftok("/boot", 'X');
    if(key == -1) { perror("ftok"); return -1; }

    /* now connect to (or possibly create) the shared memory segment with permission 644 */
    shm_id = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    if(shm_id == -1) { perror("shmget"); return -1; }

    return shm_id;
}
