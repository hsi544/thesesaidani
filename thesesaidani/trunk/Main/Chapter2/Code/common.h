/* fichier : common.h
 * contient toutes les structures
 * definies et les macros
 * ainsi que les redefinitions de types
 */
#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdlib.h>
#define SPU_THREADS 8
#define SIZEX 1024
#define SIZEY 1024
#define MEM_ALIGN 128
#define VSIZE (SIZEX*SIZEY/4)
#define SIZE_CB    128
#define ALIGN_LOG2 7
#define DMA_MAX    16384
/* la stucture contient des informations qui servent 
 * a la creation des threads dans les SPE
 */
typedef struct ppu_pthread_data {
    spe_context_ptr_t speid;
    pthread_t pthread;
    void *argp;
} ppu_pthread_data_t;
/* union servant a acceder une adresse 64-bit
 * de deux manieres differentes
 */
typedef union
{
  unsigned long long ull;
  unsigned int ui[2];
} addr64;
/* structure definissant le control block qui contient
 * toutes les informations necessaires a l'execution
 * du code SPU
 */
typedef struct _control_block {
  unsigned long long source1_ad;
  unsigned long long source2_ad;
  unsigned long long target_ad;
  unsigned int size;
  unsigned int tileh;
  unsigned int tilew;
  unsigned char pad[92];   // padding pour remplir une ligne de cache (128 octets)
} control_block_t;
typedef vector float vFloat32;
typedef unsigned int uint32;
typedef unsigned long long uint64;
#endif /* __COMMON_H__ */
