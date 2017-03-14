#include "pioche.hpp"

void pioche_init(Pioche& pioche, int taille_elt) {
   pioche.taille_elt = taille_elt;
   // les vectors s'initialisent avant, avec leur constructeurs.
}

void pioche_pioche(Pioche& pioche , void* cible) { // O(1)
  //std::cout << "Je piche une carte dans le tas" << std::endl;
  // Index du premier octet du dernier élément du tableau (qui répresente le dessus du tas de carte)
  int index = pioche.tas.size() - pioche.taille_elt;
  
  // Les données sont bien contiguës en mémoire (voir def vector)
  memcpy( cible, &pioche.tas.at(index) , pioche.taille_elt );
  // Suppression de la dernière carte en mémoire, on applique taille_elt fois la méthode pop_back
  /*for(int i = 0 ; i < pioche.taille_elt ; i++) {
    pioche.tas.pop_back();
    }*/
}


void pioche_defausse(Pioche& pioche, const void* carte) { // O(1) en temps amorti car  O(n) si besoin de realloc
  
  // APPROCHE 'SAFE' DU PB : O(1) ou O(n) si besoin de realloc
  //std::cout << "Je defausse une carte dans la defausse" << std::endl;
  char * temp = new char[pioche.taille_elt];
  memcpy( (void*) temp, carte, pioche.taille_elt);
  
  for(int i = 0; i < pioche.taille_elt ; i++) {
    pioche.defausse.push_back(temp[i]);
  }

  // Ici on ne gère pas la destruction de la carte
  delete[] temp;
}



void pioche_melange(Pioche& pioche) { // O(n)
  std::cout << "Je vais mélanger la pioche" << std::endl;
  char * temp = new char[pioche.taille_elt];
  
  char *p = pioche.tas.data(); // pointeur sur le premier element du tableau : c'est pratique, ça permet d'utiliser un vector exactement comme un tableau statique, et ça fait du bien au cerveau.

  int j = 0;
   
  // On met la defausse par dessus la pioche -- O(n)
  pioche.tas.insert(pioche.tas.end(), pioche.defausse.begin(), pioche.defausse.end());
   
  // on supprime le contenu de defausse -- O(n)
  pioche.defausse.erase(pioche.defausse.begin(), pioche.defausse.end());
   
  // On compte maintenant le nombre d'élément dans le tas
  int nb_elt_tas = pioche.tas.size() / pioche.taille_elt;


  for (int  i = 0; i < nb_elt_tas; i++) { // O(n) selon cours
    j = rand() % (nb_elt_tas - (i - 1));
    memcpy( (void *) temp, (void *) &pioche.tas.at(j*pioche.taille_elt) , pioche.taille_elt);
      
    memcpy( (void *) &pioche.tas.at(j*pioche.taille_elt), (void *) &pioche.tas.at(i*pioche.taille_elt) , pioche.taille_elt);
      
    memcpy( (void *) &pioche.tas.at(i*pioche.taille_elt) , (void *) temp , pioche.taille_elt);

  }
  delete[] temp;
}

void pioche_suppr(Pioche& pioche) {
  // No need, les vectors se détruisent d'eux mêmes, et le reste est dans la pile du main, donc delete à la fin.
}
