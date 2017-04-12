#ifndef ARAIL_JOUEUR_ORGA_HPP
#define ARAIL_JOUEUR_ORGA_HPP

#include "jeu.hpp"


struct JoueurOrga {
  std::vector <Carte> wagons;
  Objectif obj; // const ?
  int points;
  int mdp;
  int indice;
  liste_liaison chemin_obj; // permet de ne pas refaire Dijkstra à chaque tour. Stocke des index de ville
  int poids_chemin_obj;
  std::priority_queue <Liaison_poids> chemin_strategique; // <index ville, poids>
} ;

// initialisation
void joueur_orga_init(JoueurOrga& joueur) ;

// suppression
void joueur_orga_suppr(JoueurOrga& joueur) ;

// acces au mot de passe a fournir au jeu. Le mot de passe sert a s'assurer que
// le joueur ne tente pas de se faire passer pour un autre. Il sera utilise par
// le joueur pour toutes les actions sur le jeu qui verifient l'identite du
// joueur.
int joueur_orga_mdp(JoueurOrga& joueur) ;

// prise en compte de l'indice du joueur fourni par le jeu. Le joueur pourra
// utiliser cet indice pour s'identifier aupres du jeu pour les actions qui le
// demandent : piocher, defausser, prendre des liaisons, ...
void joueur_orga_config_indice(JoueurOrga& joueur, int indice) ;

// un tour de jeu pour le joueur simple
void joueur_orga_tour(Jeu& jeu, JoueurOrga& joueur) ;

// retourne une liste d'index de liaison representant le chemin le plus court
// entre les points de l'objectif passé en paramètre.
// Ne prend pas en compte la liaison ville_a <---> ville_b dans la recherche
// du plus court chemin.
liste_liaison plus_court_chemin_orga(Jeu& jeu, const Objectif &obj, const int indice_joueur, const int ville_a, const int ville_b);

#endif
