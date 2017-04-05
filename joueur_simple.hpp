#ifndef ARAIL_JOUEUR_SIMPLE_HPP
#define ARAIL_JOUEUR_SIMPLE_HPP

#include "jeu.hpp"

typedef std::vector<int> liste_liaison;

struct Couple_fp {
  int ville;
  int poids;
} ;
  
struct JoueurSimple {
  std::vector <Carte> wagons;
  Objectif obj; // const ?
  int mdp;
  int indice;
  liste_liaison chemin_obj; // permet de ne pas refaire Dijkstra à chaque tour. Stocke des index de ville
} ;

// initialisation
void joueur_simple_init(JoueurSimple& joueur) ;

// suppression
void joueur_simple_suppr(JoueurSimple& joueur) ;

// acces au mot de passe a fournir au jeu. Le mot de passe sert a s'assurer que
// le joueur ne tente pas de se faire passer pour un autre. Il sera utilise par
// le joueur pour toutes les actions sur le jeu qui verifient l'identite du
// joueur.
int joueur_simple_mdp(JoueurSimple& joueur) ;

// prise en compte de l'indice du joueur fourni par le jeu. Le joueur pourra
// utiliser cet indice pour s'identifier aupres du jeu pour les actions qui le
// demandent : piocher, defausser, prendre des liaisons, ...
void joueur_simple_config_indice(JoueurSimple& joueur, int indice) ;

// un tour de jeu pour le joueur simple
void joueur_simple_tour(Jeu& jeu, JoueurSimple& joueur) ;

// retourne une liste d'index de liaison representant le chemin le plus court
// entre les points de l'objectif passé en paramètre.
liste_liaison plus_court_chemin(Jeu& jeu, const Objectif &obj, const int indice_joueur);

// Organise les couples<ville, poids> dans la file à priorité
bool operator<(const Couple_fp& a, const Couple_fp& b); 

#endif
