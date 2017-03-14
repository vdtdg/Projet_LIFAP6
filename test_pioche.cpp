#include "pioche.hpp"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {

  srand(time(nullptr));
  
  //nombre de melanges testes
  int ntests = 1000000 ;
  //taille de la pioche
  int taille = 25 ;

  Pioche pioche ;
  
  //initialisation de la pioche pour contenir des entiers
  pioche_init(pioche, sizeof(int)) ;
  //remplissage de la pioche
  for(int i = 0; i < taille; ++i) {
    pioche_defausse(pioche, &i) ;
  }
  //test de la pioche : combien de fois 0 apparait avant taille-1
  //apres un melange. Devrait etre proche de ntests / 2
  int score = 0 ;
  pioche_melange(pioche) ; // ajout
  for(int t = 0; t < ntests; ++t) {
    //iteration sur la pioche
    for(int i = 0; i < taille; ++i) {
      int a ;
      
      pioche_pioche(pioche, &a) ;
      pioche_defausse(pioche, &a) ;
      //test si 0 ou taille-1 est trouve
      if(a == 0) {
        //0 est trouve, augmentation du score
        ++score ;
        //passage a un nouveau test
        pioche_melange(pioche) ;
        break ;
      }
      if(a == taille - 1) {
        //taille-1 est trouve
        //passage a un nouveau test
        pioche_melange(pioche) ;
        break ;
      }
    }
  }
  //statistiques obtenues, proche de 0.5 si correct
  printf("%f\n", (float) score / (float) ntests) ;

  //menage
  pioche_suppr(pioche) ;
  return 0 ;
}
