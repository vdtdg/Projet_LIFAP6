#include "jeu.hpp"
#include "jeu_io.hpp"
#include "joueur_simple.hpp"
#include "joueur_organise.hpp"

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
  JoueurSimple joueurA ;
  joueur_simple_init(joueurA) ;
  //ajout du joueur à la partie
  int index = jeu_ajout_joueur(jeu, joueur_simple_mdp(joueurA)) ;
  joueur_simple_config_indice(joueurA, index) ;
  
  //joueur organisé
  JoueurOrga joueurB ;
  joueur_orga_init(joueurB) ;
  index = jeu_ajout_joueur(jeu, joueur_orga_mdp(joueurB)) ;
  joueur_orga_config_indice(joueurB, index) ;
  
  /* Pas encore dev.
 
  //joueur opportuniste
  JoueurOpport joueurC ;
  joueur_opport_init(joueurC) ;
  index = jeu_ajout_joueur(jeu, joueur_opport_mdp(joueurC)) ;
  joueur_ooport_config_indice(joueurC, index) ;
  
  //joueur polyvalent
  JoueurPoly joueurD ;
  joueur_poly_init(joueurD) ;
  index = jeu_ajout_joueur(jeu, joueur_poly_mdp(joueurD)) ;
  joueur_poly_config_indice(joueurD, index) ;
  
  //joueur vicieux
  JoueurVice joueurE ;
  joueur_vice_init(joueurE) ;
  index = jeu_ajout_joueur(jeu, joueur_vice_mdp(joueurE)) ;
  joueur_vice_config_indice(joueurE, index) ;
  
  */

  //jeu
  while(jeu_get_tour(jeu) < 12) { // TODO : nb de tour en argument
    if(DEBUG) { std::cout << std::endl << "----------------Tour n°" << jeu_get_tour(jeu) << "----------------" << std::endl; }
    if(DEBUG) { std::cout << "-- Joueur simple --" << std::endl; }
    joueur_simple_tour(jeu, joueurA) ;
    if(DEBUG) { std::cout << std::endl << "-- Joueur organisé --" << std::endl; }
    joueur_orga_tour(jeu, joueurB) ;
    /* Pas encore dev
    if(DEBUG) { std::cout << "-- Joueur opportuniste --" << std::endl; }
    joueur_opport_tour(jeu, joueurC) ;
    if(DEBUG) { std::cout << "-- Joueur polyvalent --" << std::endl; }
    joueur_poly_tour(jeu, joueurD) ;
    if(DEBUG) { std::cout << "-- Joueur vicieux --" << std::endl; }
    joueur_vice_tour(jeu, joueurE) ;
    */
    
    jeu_tour_incremente(jeu);
  }
  if(DEBUG) { std::cout << "################# FIN DU JEU #################" << std::endl; }

  //historique
  if(DEBUG) { std::cout << std::endl << "######### AFFICHAGE DE L'HISTORIQUE ##########" << std::endl; }
  jeu_affiche_historique(jeu) ;

  //menage
  joueur_simple_suppr(joueurA) ;
  joueur_orga_suppr(joueurB) ;
  /* Pas encore dev
  joueur_opport_suppr(joueurC) ;
  joueur_poly_suppr(joueurD) ;
  joueur_vice_suppr(joueurE) ;
  */
  jeu_suppr(jeu) ;

  return 0 ;
}
