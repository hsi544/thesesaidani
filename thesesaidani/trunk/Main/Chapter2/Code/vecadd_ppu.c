/* fichier : vecadd_ppu.c 
 * addition de deux vecteurs
 * code source du PPU
 */                                 
#include "common.h"
#include <sched.h>
#include <libspe2.h>
#include <pthread.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
/* cette fonction est executee a la creation du thread */
void *ppu_pthread_function(void *arg){
    ppu_pthread_data_t *datap = (ppu_pthread_data_t *)arg;
    int rc;
    unsigned int entry = SPE_DEFAULT_ENTRY;
    if ((rc = spe_context_run(datap->speid, &entry, 0, datap->argp, NULL, NULL)) < 0) {
        fprintf (stderr, "Failed spe_context_run(rc=%d, errno=%d, strerror=%s)\n", rc, errno, strerror(errno));
        exit (1);
    }
    pthread_exit(NULL);
}
/* Ceci est le pointeur vers le code SPU
 * il doit etre utilise a la creation 
 * du thread
 */
extern spe_program_handle_t vecadd_spu;
/* Ce sont les handles retournes par 
 * "spe_context_create" et "pthread_create"
 */
ppu_pthread_data_t datas[SPU_THREADS];
int main(int argc, char *argv[])
{
  int i,rc;
  unsigned int chunkw,chunkh,chunksize, chunksizev;
  unsigned int tilew, tileh, tilesize, nbtiles;
  vFloat32 *vect1, *vect2, *vect3;
  float *sptr_vect1, *sptr_vect2, *sptr_vect3;
  control_block_t* cb;
  uint64 *ad_input1, *ad_input2, *ad_output;
  // chunk size computing
  chunkw=SIZEY;
  chunkh=SIZEX/SPU_THREADS;
  chunksize=chunkw*chunkh;
  chunksizev=chunksize/4;
  tilew=SIZEY;
  tileh=4;
  tilesize=tileh*tilew;
  nbtiles=chunksize/(tilesize);
  // allocation des vecteurs   
  vect1=(vFloat32*)memalign(MEM_ALIGN,VSIZE*sizeof(vFloat32));
  vect2=(vFloat32*)memalign(MEM_ALIGN,VSIZE*sizeof(vFloat32));
  vect3=(vFloat32*)memalign(MEM_ALIGN,VSIZE*sizeof(vFloat32));
  // allocation des control blocks	
  cb = (control_block_t*)memalign(MEM_ALIGN,SPU_THREADS*sizeof(control_block_t));
  ad_input1 = (uint64*)memalign(MEM_ALIGN,SPU_THREADS*sizeof(uint64));
  ad_input2 = (uint64*)memalign(MEM_ALIGN,SPU_THREADS*sizeof(uint64));
  ad_output = (uint64*)memalign(MEM_ALIGN,SPU_THREADS*sizeof(uint64));
  // initialisation du control blocks
  for (i = 0; i < SPU_THREADS; i++){
  ad_input1[i] = (uint64)((vFloat32*)vect1+i*chunksizev);
	ad_input2[i] = (uint64)((vFloat32*)vect2+i*chunksizev);
  ad_output[i] = (uint64)((vFloat32*)vect3+i*chunksizev);
	cb[i].source1_ad = ad_input1[i];
	cb[i].source2_ad = ad_input2[i];
	cb[i].target_ad = ad_output[i];
	cb[i].size =chunksize;
	cb[i].tilex=tileh;
	cb[i].tiley=tilew;
	cb[i].count=nbtiles;
  }
  // pointeurs scalaires sur buffers vectoriels
  sptr_vect1 = (float*)vect1;
  sptr_vect2 = (float*)vect2;
  sptr_vect3 = (float*)vect3;
  // initialisation des buffers
  for (int i = 0; i < SIZEX*SIZEY; i++){
    sptr_vect1[i] = 1.0f;
    sptr_vect1[i] = 2.0f;
    sptr_vect3[i] = 0.0f;
  }
  /* allocation des taches SPE */
  for (i = 0; i < SPU_THREADS; i++) {
    // creation du contexte
    if ((datas[i].speid = spe_context_create (0, NULL)) == NULL){
      fprintf (stderr, "Failed spe_context_create(errno=%d strerror=%s)\n", errno, strerror(errno));
      exit (3+i);
	  }
    // chargement du programme
	  if ((rc = spe_program_load (datas[i].speid, &vecadd_spu)) != 0){
      fprintf (stderr, "Failed spe_program_load(errno=%d strerror=%s)\n", errno, strerror(errno));
      exit (3+i);
    }
	  // initialisation du pointeur argp
    datas[i].argp = (unsigned long long *)(&cb[i]);
    // creation du thread
	  if ((rc = pthread_create (&datas[i].pthread, NULL, &ppu_pthread_function, &datas[i])) != 0){
      fprintf (stderr, "Failed pthread_create(errno=%d strerror=%s)\n", errno, strerror(errno));
      exit (3+i);
    }
  }
  /* attendre que tous les SPEs aient fini */
  for (i=0; i<SPU_THREADS; ++i){
	  // jointure des threads
	  if ((rc = pthread_join (datas[i].pthread, NULL)) != 0){
	    fprintf (stderr, "Failed pthread_join(rc=%d, errno=%d strerror=%s)\n", rc, errno, strerror(errno));
	    exit (1);
	  }
	  // destruction du contexte
    if ((rc = spe_context_destroy (datas[i].speid)) != 0){
      fprintf (stderr, "Failed spe_context_destroy(rc=%d, errno=%d strerror=%s)\n", rc, errno, strerror(errno));
      exit (1);
    }
  }
  /* barriere de synchronisation */
  __asm__ __volatile__ ("sync" : : : "memory");
  // deallocation de la memoire
  free((void*)vect1);
  free((void*)vect2);
  free((void*)vect3);
  free((void*)ad_input1);
  free((void*)ad_input2);
  free((void*)ad_output);
  return 0;
}
