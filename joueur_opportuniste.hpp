#ifndef ARAIL_JOUEUR_OPPORT_HPP
#define ARAIL_JOUEUR_OPPORT_HPP

#include "jeu.hpp"


struct JoueurOpport {
  std::vector <std::vector <Carte> > wagons; // tableau de tableau de carte, un vector pour chaque couleur
  Objectif obj; // const ?
  int points;
  int mdp;
  int indice;
  liste_liaison chemin_obj; // permet de ne pas refaire Dijkstra à chaque tour. Stocke des index de ville
  int poids_chemin_obj;
  std::priority_queue <Liaison_poids> chemin_strategique; // <index ville, poids>
} ;

// initialisation
void joueur_opport_init(JoueurOpport& joueur) ;

// suppression
void joueur_opport_suppr(JoueurOrga& joueur) ;

// acces au mot de passe a fournir au jeu. Le mot de passe sert a s'assurer que
// le joueur ne tente pas de se faire passer pour un autre. Il sera utilise par
// le joueur pour toutes les actions sur le jeu qui verifient l'identite du
// joueur.
int joueur_opport_mdp(JoueurOpport& joueur) ;

// prise en compte de l'indice du joueur fourni par le jeu. Le joueur pourra
// utiliser cet indice pour s'identifier aupres du jeu pour les actions qui le
// demandent : piocher, defausser, prendre des liaisons, ...
void joueur_opport_config_indice(JoueurOpport& joueur, int indice) ;

// un tour de jeu pour le joueur simple
void joueur_opport_tour(Jeu& jeu, JoueurOpport& joueur) ;

// retourne une liste d'index de liaison representant le chemin le plus court
// entre les points de l'objectif passé en paramètre.
// Ne prend pas en compte la liaison ville_a <---> ville_b dans la recherche
// du plus court chemin.
liste_liaison plus_court_chemin_opport(Jeu& jeu, const Objectif &obj, const int indice_joueur, const int ville_a, const int ville_b);

#endif


