#include "jeu.hpp"
#include "jeu_io.hpp"
#include "joueur_simple.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
  
  //verification qu'un fichier est fourni
  if(argc < 2) {
    std::cout << "usage : " << argv[0] << " <config file>" << std::endl ;
    return 1 ;
  }

  //chargement du fichier
  std::ifstream file(argv[1]) ;

  //initialisation
  Jeu jeu ;
  jeu_init(jeu) ;

  //lecture de la configuration
  jeu_import(jeu, file) ;

  //finalisation
  jeu_debut_partie(jeu) ;

  //joueur simple
  JoueurSimple joueur ;
  joueur_simple_init(joueur) ;

  //ajout du joueur à la partie
  int index = jeu_ajout_joueur(jeu, joueur_simple_mdp(joueur)) ;
  joueur_simple_config_indice(joueur, index) ;

  //jeu
  for(int i = 0; i < 30; ++i) {
    std::cout << "Tour n°" << i << std::endl;
    joueur_simple_tour(jeu, joueur) ;
  }

  //historique
  jeu_affiche_historique(jeu) ;

  //menage
  joueur_simple_suppr(joueur) ;
  jeu_suppr(jeu) ;

  return 0 ;
}
