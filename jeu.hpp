#ifndef ARAIL_GAME_HPP
#define ARAIL_GAME_HPP

#include "pioche.hpp"

#include <vector>
#include <string>

struct Liaison {
  int ville1 ;
  int ville2 ;
  int couleur ;
  int longueur ; //0 si la liaison n'existe pas
  int proprietaire ; //0 si personne sinon indice joueur
} ;

struct Carte {
  int couleur ;
} ;

enum ActionType {
  PIOCHE_VISIBLE,
  PIOCHE_CACHE,
  DEFAUSSE_CARTE,
  PIOCHE_OBJECTIF,
  DEFAUSSE_OBJECTIF,
  PRENDRE_LIAISON
} ;

struct Action {
  ActionType type ;
  int joueur ;
  int info[2] ;
} ;

struct Objectif {
  int ville1 ;
  int ville2 ;
  int points ;
} ;

struct Jeu {
  //pioches
  Pioche cartes ;
  Pioche objectifs ;
  Carte cartes_visibles[5] ;
  //info
  std::vector<int> nb_cartes ;
  std::vector<Objectif> info_objectifs ;
  //noms
  std::vector<std::string> noms_villes ;
  std::vector<std::string> noms_couleurs ;
  //liaisons
  std::vector<Liaison> liaisons ;
  std::vector<int> index_ville ;
  //joueurs
  std::vector<int> joueurs ;
  //historique
  std::vector<Action> actions ;
} ;

//initialisation et destruction de la partie

void jeu_init(Jeu& jeu) ;

void jeu_suppr(Jeu& jeu) ;

int jeu_ajout_ville(Jeu& jeu, const char* nom) ;

int jeu_ajout_couleur(Jeu& jeu, const char* nom) ;

void jeu_ajout_liaison(Jeu& jeu, const Liaison& liaison) ;

void jeu_ajout_objectif(Jeu& jeu, const Objectif& objectif) ;

void jeu_ajout_carte(Jeu& jeu, const Carte& carte) ;

int jeu_ajout_joueur(Jeu& jeu, int mdp) ;

void jeu_debut_partie(Jeu& jeu) ;

//interactions avec la partie

////informations generales

int jeu_nb_villes(const Jeu& jeu) ;

const char* jeu_nom_ville(const Jeu& jeu, int ville) ;

int jeu_nb_couleurs(const Jeu& jeu) ;

const char* jeu_nom_couleur(const Jeu& jeu, int couleur) ;

int jeu_nb_joueurs(const Jeu& jeu) ;

////liaisons

Liaison jeu_liaison(const Jeu& jeu, int ville1, int ville2) ;

int jeu_ville_nb_liaisons(const Jeu& jeu, int ville) ;

Liaison jeu_ville_liaison(const Jeu& jeu, int ville, int index) ;

void jeu_prendre_liaison(Jeu& jeu, int ville1, int ville2, int joueur, int mdp) ;

////cartes

int jeu_nb_cartes_couleur(const Jeu& jeu, int couleur) ;

Carte jeu_carte_visible(const Jeu& jeu, int index) ;

Carte jeu_pioche_cache(Jeu& jeu, int joueur, int mdp) ;

Carte jeu_pioche_visible(Jeu&, int index, int joueur, int mdp) ;

void jeu_defausse(Jeu& jeu, const Carte& carte, int joueur, int mdp) ;

////objectifs

int jeu_nb_objectifs(const Jeu& jeu) ;

Objectif jeu_info_objectif(const Jeu& jeu, int index) ;

Objectif jeu_pioche_objectif(Jeu& jeu, int joueur, int mdp) ;

void jeu_defausse_objectif(Jeu& jeu, const Objectif& objectif, int joueur, int mdp) ;

////historique

int jeu_nb_actions(const Jeu& jeu) ;

Action jeu_action(const Jeu& jeu, int index) ;

void action_affiche(const Jeu& jeu, const Action& action) ;

void jeu_affiche_historique(const Jeu& jeu) ;

#endif
