#include "pioche.hpp"

void pioche_init(Pioche& pioche, int taille_elt)
{
   pioche.taille_elt = taille_elt;


}

void pioche_pioche(Pioche& pioche , void* cible)
{
  //std::memcpy(cible , pioche.tas ,pioche.taille_elt); // pas fini
}


void pioche_defausse(Pioche& pioche, const void* carte)
 {
  //... votre code ici
}



void pioche_melange(Pioche& pioche)
 {
   char * temp =  new char [pioche.taille_elt];
   int taille_tab = pioche.tas.size() / pioche.taille_elt;
   pioche.tas.insert(pioche.tas.end(), pioche.defausse.begin(), pioche.defausse.end());
   for (int  i = 0; i <  taille_tab ; i++)
    {
      int j = rand() % taille_tab - i - 1;
      memcpy((void *) temp, (void *) pioche.tas[j*pioche.taille_elt] , pioche.taille_elt);
      memcpy((void *) pioche.tas[j*pioche.taille_elt], (void *) pioche.tas[i*pioche.taille_elt] , pioche.taille_elt);
      memcpy( (void *) pioche.tas[i*pioche.taille_elt] , (void *) temp , pioche.taille_elt);

   }

   delete [] temp;
}

void pioche_suppr(Pioche& pioche)
{

}
