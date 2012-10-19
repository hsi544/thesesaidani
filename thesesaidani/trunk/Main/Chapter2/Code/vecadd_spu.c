// fichier  : vec_add_spu.c
// addition de deux vecteurs
// code source du SPU
#include <spu_mfcio.h>
#include <stdio.h>
#include <malloc_align.h>
#include <free_align.h>
#include <spu_intrinsics.h>
/* control block local */
control_block_t m_cb __attribute__ ((aligned (SIZE_CB)));
int main(unsigned long long speid, addr64 argp, addr64 envp)
{
	float *m_source1_ad, *m_source2_ad, *m_target_ad;
	unsigned int m_tileh, m_tilew, m_tilewv;
	unsigned int count, rest, offset;
	vFloat32 *bufvect0, *bufvect1, *bufvect2;
	const int tag0 = 10;
	const int tag1 = 11;
	const int tag2 = 12;
	const int tag3 = 13;
	unsigned int sizev = sizeof(vFloat32);
	/* tranfert du control block de la memoire centrale vers celle du SPU */
	spu_mfcdma32((void*)(&cb), (unsigned int)argp.ui[1], SIZE_CB, tag0, MFC_GET_CMD);
	mfc_write_tag_mask(1<<tag0);
  mfc_read_tag_status_all();
  /* initialisation des adresses des buffers et de leurs tailles */
	m_source1_ad = (vFloat32*)m_cb.source1_ad;
	m_source2_ad = (vFloat32*)m_cb.source2_ad;
	m_target_ad  = (vFloat32*)m_cb.target_ad;
	m_tileh  = m_cb.tileh;
	m_tilew  = m_cb.tilew;
	m_tilewv = m_tilew/4;
	/* allocation de la memoire SPU */
	bufvect0 = (vFloat32*)_malloc_align(m_tileh*m_tilewv*sizev, ALIGN_LOG2);
	bufvect1 = (vFloat32*)_malloc_align(m_tileh*m_tilewv*sizev, ALIGN_LOG2);
	bufvect2 = (vFloat32*)_malloc_align(m_tileh*m_tilewv*sizev, ALIGN_LOG2);
  // calcul du nombre de tuiles
	offset = DMA_MAX/sizev;
	count  = (m_tileh*m_tilewv)/offset;
	rest   = (m_tileh*m_tilewv)%offset;
	/* transfert du premier operande de la memoire centrale */
	// premiers transferts
	for (int i=0; i<count; i++){
		mfc_get((void*)((uint32)bufvect0+i*offset*sizev,(uint64)m_source1_ad+i*offset*sizev, DMA_MAX, tag1, 0, 0);
	}
	// dernier DMA si pas multiple de 16Ko
	if(rest != 0){
		mfc_get((void*)((uint32)bufvect0+count*offset*sizev,(uint64)m_source1_ad+count*offset*sizev, rest*sizev, tag1, 0, 0);
	}
	mfc_write_tag_mask(1<<tag1);
    mfc_read_tag_status_all();
	/* transfert du second operande de la memoire centrale */
	// premiers transferts
	for (int i=0; i<count; i++){
		mfc_get((void*)((uint32)bufvect1+i*offset*sizev,(uint64)m_source2_ad+i*offset*sizev, DMA_MAX, tag2, 0, 0);
	}
	// dernier DMA si pas multiple de 16Ko
	if(rest != 0){
		mfc_get((void*)((uint32)bufvect1+count*offset*sizev,(uint64)m_source2_ad+count*offset*sizev, rest*sizev, tag2, 0, 0);
	}
	mfc_write_tag_mask(1<<tag2);
    mfc_read_tag_status_all();
	/* addition des deux tuiles */
	for(int i=0;i<m_tileh*m_tilewv;i++){
	  bufvect2[i]=spu_add(bufvect0[i],bufvect1[i]);
	}
	/* transfert du buffer de sortie vers la memoire externe */
	// premiers transferts
	for (int i=0; i<count; i++){
	  mfc_put((void*)((uint32)bufvect2+i*offset*sizev,(uint64)m_target_ad+i*offset*sizev, DMA_MAX, tag3, 0, 0);
	}
	// dernier DMA si pas multiple de 16Ko
	if(rest != 0){
	  mfc_put((void*)((uint32)bufvect2+count*offset*sizev,(uint64)m_target_ad+count*offset*sizev, rest*sizev, tag3, 0, 0);
	}
	mfc_write_tag_mask(1<<tag3);
	mfc_read_tag_status_all();
  /* deallocation de la memoire SPU */
	_free_align((void*)bufvect0);
	_free_align((void*)bufvect1);
	_free_align((void*)bufvect2);
    return 0;
}
