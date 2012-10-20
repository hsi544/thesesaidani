/* fichier : harris_spmd_spe.c
 */
// inclusion des fichiers d'entete standard
...
int main (unsigned long long spu_id __attribute__ ((__unused__)), addr64 parm)
{
	// declaration des buffers de donnees
	vFloat32 *B0, *B1, *B2, *B3;
	vFloat32 *B4, *B5, *B6, *B7, *B8;
	// copie du control block dans le SPE
    ...
  	// copie du control block de la memoire vers le SPE
    ...
	// allocation des buffers de donnees
    ...
	// filtrage de Sobel sur toutes les tuiles
	for (i=0;i<nbtiles;i++)
	{
      // transfert d'une tuile de memoire ext vers le SPE
      get_tile(source0, B5, tile1_h*tile1_w,0);
      //calcul du filtrage Sobel	
      sobel_RED_SPE_1F32(B5+tile1_w,tile2_h,tile2_ws,B0,B1);
      // transfert d'une tuile Ix vers la memoire ext
      put_tile(source10,B0,tile2_h*tile2_w,1);
      // transfert d'une tuile Iy vers la memoire ext
      put_tile(source11,B1,tile2_h*tile2_w,2);
      // tuiles suivantes
      source0+=(tile2_h*tile2_w);
	  source10+=(tile2_h*tile2_w);
      source11+=(tile2_h*tile2_w);
	}
	// reinitialisation du poiteur vers les données source
	source10=(vFloat32*)elem.ad_source10;
	source11=(vFloat32*)elem.ad_source11;
	source20+=tile2_w;
	source21+=tile2_w;
	source22+=tile2_w;
	for (i=0;i<nbtiles;i++)
	{
      // transfert d'une tuile Ix de la memoire ext
      get_tile(source10, B2,tile2_h*tile2_w,3);
      // transfert d'une tuile Iy de la memoire ext
      get_tile(source11, B3,tile2_h*tile2_w,4);
      // calcul du produit Ixx=Ix*Ix
      mul_SPE_1F32(B2,B2,tile2_h,tile2_ws, B0);
      // calcul du produit Ixy=Ix*Iy
      mul_SPE_1F32(B2,B3,tile2_h,tile2_ws, B1);
      // calcul du produit Iyy=Iy*Iy
      mul_SPE_1F32(B3,B3,tile2_h,tile2_ws, B4);
      // transfert d'une tuile Ixx vers la memoire ext
      put_tile(source20, B0,tile2_h*tile2_w,5);
      // transfert d'une tuile Ixy vers la memoire ext
      put_tile(source21, B1,tile2_h*tile2_w,6);
      // transfert d'une tuile Iyy vers la memoire ext
      put_tile(source22, B4,tile2_h*tile2_w,7);
      // tuiles suivantes
      source10+=(tile2_h*tile2_w);
      source11+=(tile2_h*tile2_w);
      source20+=(tile2_h*tile2_w);
      source21+=(tile2_h*tile2_w);
      source22+=(tile2_h*tile2_w);
	}
	// reinitialisation du poiteur vers les données source
	source20=(vFloat32*)elem.ad_source20;
	source21=(vFloat32*)elem.ad_source21;
	source22=(vFloat32*)elem.ad_source22;
	for (i=0;i<nbtiles;i++)
	{
	  // transfert d'une tuile Ixx de la memoire ext
      get_tile(source20, B5,tile1_h*tile1_w,8);
      // transfert d'une tuile Ixy de la memoire ext
      get_tile(source21, B6,tile1_h*tile1_w,9);
	  // transfert d'une tuile Iyy de la memoire ext
      get_tile(source22, B7,tile1_h*tile1_w,10);
      // calcul du Gaussien Sxx
      gauss_RED_SPE_1F32(B5+tile2_w,tile2_h, tile2_ws, B0);
      // calcul du Gaussien Sxy
	  gauss_RED_SPE_1F32(B6+tile2_w,tile2_h, tile2_ws, B1);
      // calcul du Gaussien Syy
      gauss_RED_SPE_1F32(B7+tile2_w,tile2_h, tile2_ws,B2);
      // transfert d'une tuile Sxx vers la memoire ext
      put_tile(source30, B0,tile2_h*tile2_w,11);
      // transfert d'une tuile Sxy vers la memoire ext
      put_tile(source31, B1,tile2_h*tile2_w,12);
      // transfert d'une tuile Syy vers la memoire ext
      put_tile(source32, B2,tile2_h*tile2_w,13);
      // tuiles suivantes
      source20+=(tile2_h*tile2_w);
	  source21+=(tile2_h*tile2_w);
      source22+=(tile2_h*tile2_w);
      source30+=(tile2_h*tile2_w);
      source31+=(tile2_h*tile2_w);
      source32+=(tile2_h*tile2_w);
	}
	// reinitialisation du poiteur vers les donnees source
	source30=(vFloat32*)elem.ad_source30;
	source31=(vFloat32*)elem.ad_source31;
	source32=(vFloat32*)elem.ad_source32;
	for (i=0;i<nbtiles;i++)
	{
	  //transfert d'une tuile Sxx de la memoire ext
      get_tile(source30, B3,tile2_h*tile2_w,14);
      // transfert d'une tuile Sxx de la memoire ext
      get_tile(source31, B4,tile2_h*tile2_w,15);
	  // transfert d'une tuile Syy de la memoire ext
      get_tile(source32, B0,tile2_h*tile2_w,16);
	  // calcul de la coarsite Sxx*Syy-Sxy*Sxy
      harris_SPE_1F32(B3, B4, B0, tile2_h, tile2_ws, B8);
	  // transfert d'une tuile K vers la memoire ext
      put_tile(target30, B8,tile2_h*tile2_w,17);
      // tuiles suivantes
      source30+=(tile2_h*tile2_w);
      source31+=(tile2_h*tile2_w);
      source32+=(tile2_h*tile2_w);
      target30+=(tile2_h*tile2_w);
	}
	// deallocation de la memoire
    ...	
	return 0;
}
