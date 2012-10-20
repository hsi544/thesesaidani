/* fichier : harris_spmd_ppe.c
 */
// inclusion des fichiers d'entete standard
...
/* la stucture contient des informations qui servent 
 * a la creation des threads dans les SPE
 */
typedef struct ppu_pthread_data {
    spe_context_ptr_t speid;
    pthread_t pthread;
    void *argp;
} ppu_pthread_data_t;
/* cette fonction est executee à la creation du thread */
void *ppu_pthread_function(void *arg){
    ppu_pthread_data_t *datap = (ppu_pthread_data_t *)arg;
    int rc;
    unsigned int entry = SPE_DEFAULT_ENTRY;
    if ((rc = spe_context_run(datap->speid, &entry, 0, datap->argp, NULL, NULL)) < 0){
        fprintf (stderr, "Failed spe_context_run(rc=%d, errno=%d, strerror=%s)\n", rc, errno, strerror(errno));
        exit (1);
    }
    pthread_exit(NULL);
}
//pointeurs sur les threads SPE
extern spe_program_handle_t spe_spmd;
ppu_pthread_data_t* datas;

int main(int argc,char **argv)
{
	// declarations des variables et des pointeurs
    ...
	// allocation du control block et de la structure contenant son adresse
	...
	// initialisation des parametres
	...
	// allocation des buffers entrees/sorties 
    ...
	// initialisation des buffers
	...
	// initialisation du control block
    ...	
    /* allocation des taches SPE */
 	for (i = 0; i < spu_threads; i++){
   	  // creation du contexte
      if ((datas[i].speid = spe_context_create (0, NULL)) == NULL){
        fprintf (stderr, "Failed spe_context_create(errno=%d strerror=%s)\n", errno, strerror(errno));
		exit (3+i);
      }
      // chargement du programme
      if ((rc = spe_program_load (datas[i].speid, &spe_spmd)) != 0){
         fprintf (stderr, "Failed spe_program_load(errno=%d strerror=%s)\n", errno, strerror(errno));
		 exit (3+i);
       }
	   // initialisation du poiteur qui contient l'adresse des control blocks
	   datas[i].argp = (unsigned long long *)(&element[i]);
       // creation des thread
       if ((rc = pthread_create (&datas[i].pthread, NULL, &ppu_pthread_function, &datas[i])) != 0){
         fprintf (stderr, "Failed pthread_create(errno=%d strerror=%s)\n", errno, strerror(errno));
         exit (3+i);
       }
    }
    /* Barriere de synchronisation des SPE */
    for (i=0; i<spu_threads; ++i){
	  // pthread join
      if ((rc = pthread_join (datas[i].pthread, NULL)) != 0){
        fprintf (stderr, "Failed pthread_join(rc=%d, errno=%d strerror=%s)\n", rc, errno, strerror(errno));
        exit (1);
	  }
      // Destruction du contexte
      if ((rc = spe_context_destroy (datas[i].speid)) != 0){
        fprintf (stderr, "Failed spe_context_destroy(rc=%d, errno=%d strerror=%s)\n", rc, errno, strerror(errno));
		exit (1);
      }
	}
	//Liberation de la mémoire
    ...
    return (0);
}

