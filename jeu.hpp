#ifndef ARAIL_GAME_HPP
#define ARAIL_GAME_HPP
#define DEBUG 1
#define JOKER 0

#include "pioche.hpp"
#include <vector>
#include <string>
#include <queue>  // std::priority_queue
#include <limits> // std::numeric_limits

typedef std::vector<int> liste_liaison;

struct Couple_fp {
  int ville;
  int poids;
} ;

struct Liaison_poids {
  int ville1;
  int ville2;
  int poids;
} ;
  

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
  int tour;
} ;

// Ajout d'opération sur les struct.
bool operator<(const Couple_fp& a, const Couple_fp& b);
bool operator<(const Liaison_poids& a, const Liaison_poids& b);

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

void jeu_tour_incremente(Jeu& jeu) ; // incremente le numéro de tour du jeu.

int jeu_get_tour(Jeu& jeu) ; // get jeu.tour

////informations generales

/* nombre de villes */
int jeu_nb_villes(const Jeu& jeu) ;

/* nom de la ville depuis son indice
 *   - ville est l'index de la ville entre 0 et nb_villes - 1
 */
const char* jeu_nom_ville(const Jeu& jeu, int ville) ;

/* nombre de couleurs */
int jeu_nb_couleurs(const Jeu& jeu) ;

/* nom d'une couleur depuis son indice
 *   - couleur est l'index de la couleur entre 0 et nb_couleurs - 1
 */
const char* jeu_nom_couleur(const Jeu& jeu, int couleur) ;

/* nombre de joueurs */
int jeu_nb_joueurs(const Jeu& jeu) ;

////liaisons

/* liaison entre deux villes
 *   - ville1 et ville2 sont les deux villes
 *   - la liaison retournee a une longueur nulle si elle n'existe pas
 */
Liaison jeu_liaison(const Jeu& jeu, int ville1, int ville2) ;

/* nombre de liaisons autour d'une ville
 *   - ville est la ville d'où partent les liaisons
 */
int jeu_ville_nb_liaisons(const Jeu& jeu, int ville) ;

/* liaison autour d'une ville
 *   - ville est la ville d'où part la liaison
 *   - index est l'index de la liaison autour de la ville entre 0 et
 *     ville_nb_liaison - 1 (attention, bornes non verifiees)
 */
Liaison jeu_ville_liaison(const Jeu& jeu, int ville, int index) ;

/* appropriation une liaison
 *   - ville1 et ville2 sont les extremites de la liaison
 *   - joueur et mdp sont les parametres du joueur
 *   - ne fait rien si les parametres du joueur sont mauvais
 */
void jeu_prendre_liaison(Jeu& jeu, int ville1, int ville2, int joueur, int mdp) ;

////cartes

/* nombre de cartes initiales (et non restantes) pour une couleur */
int jeu_nb_cartes_couleur(const Jeu& jeu, int couleur) ;

/* carte visible
 *   - index est l'index de la carte visible entre 0 et 4
 */
Carte jeu_carte_visible(const Jeu& jeu, int index) ;

/* pioche d'une carte
 *   - joueur et mdp sont les parametres du joueur
 *   - la valeur de retour est non predictibles si les parametres du joueur 
 *     sont mauvais
 */
Carte jeu_pioche_cache(Jeu& jeu, int joueur, int mdp) ;

/* pioche d'une carte visible
 *   - index est l'index de la carte
 *   - joueur et mdp sont les parametres du joueur
 *   - la valeur de retour est non predictibles si les parametres du joueur 
 *     sont mauvais
 */
Carte jeu_pioche_visible(Jeu&, int index, int joueur, int mdp) ;

/* defausse d'une carte
 *   - carte est la carte defaussee
 *   - joueur et mdp sont les parametres du joueur
 *   - ne fait rien si les parametres du joueur sont mauvais
 */
void jeu_defausse(Jeu& jeu, const Carte& carte, int joueur, int mdp) ;

////objectifs

/* nombre de cartes objectif initial */
int jeu_nb_objectifs(const Jeu& jeu) ;

/* informations sur l'objectif
 *   - index est l'index de l'objectif entre 0 et nb_objectifs - 1
 */
Objectif jeu_info_objectif(const Jeu& jeu, int index) ;

/* pioche d'un objectif
 *   - joueur et mdp sont les parametres du joueur
 *   - la valeur de retour est non predictibles si les parametres du joueur 
 *     sont mauvais
 */
Objectif jeu_pioche_objectif(Jeu& jeu, int joueur, int mdp) ;

/* defausse d'un objectif
 *   - objectif est l'objectif defausse
 *   - joueur et mdp sont les parametres du joueur
 *   - ne fait rien si les parametres du joueur sont mauvais
 */
void jeu_defausse_objectif(Jeu& jeu, const Objectif& objectif, int joueur, int mdp) ;

////historique

/* nombre d'actions effectuees par les joueurs
 *   - les actions sont les fonctions demandant les parametres du joueur
 */
int jeu_nb_actions(const Jeu& jeu) ;

/* informations sur l'action
 *   - index est l'index de l'action entre 0 et nb_actions - 1
 */
Action jeu_action(const Jeu& jeu, int index) ;

/* affichage d'une action
 *   - action est l'action a afficher
 */
void action_affiche(const Jeu& jeu, const Action& action) ;

/* affichage de tout l'historique de la partie
 */
void jeu_affiche_historique(const Jeu& jeu) ;

#endif
