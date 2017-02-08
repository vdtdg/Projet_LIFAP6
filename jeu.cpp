#include "jeu.hpp"

#include <algorithm>
#include <iostream>

//initialisation et destruction de la partie

void jeu_init(Jeu& jeu) {
  //reinitialisation des tableaux
  jeu.nb_cartes.resize(0) ;
  jeu.info_objectifs.resize(0) ;
  jeu.noms_villes.resize(0) ;
  jeu.noms_couleurs.resize(0) ;
  jeu.liaisons.resize(0) ;
  jeu.index_ville.resize(0) ;
  jeu.joueurs.resize(0) ;
  jeu.actions.resize(0) ;
  //initialisation des pioches
  pioche_init(jeu.cartes, sizeof(Carte)) ;
  pioche_init(jeu.objectifs, sizeof(Objectif)) ;
  //ajout d'un joueur fictif car 0 => pas de joueur
  jeu.joueurs.push_back(0) ;
}

void jeu_suppr(Jeu& jeu) {
  //liberation de la memoire occupee par les pioches
  pioche_suppr(jeu.cartes) ;
  pioche_suppr(jeu.objectifs) ;
}

int jeu_ajout_ville(Jeu& jeu, const char* nom) {
  //enregistrement du nom
  jeu.noms_villes.push_back(nom) ;
  //initialisation du nombre de laisons pour la ville
  jeu.index_ville.push_back(0) ;
  //index de la ville
  return jeu.noms_villes.size() - 1 ;
}

int jeu_ajout_couleur(Jeu& jeu, const char* nom) {
  //enregistrement du nom
  jeu.noms_couleurs.push_back(nom) ;
  //stats des cartes de cette couleur
  jeu.nb_cartes.push_back(0) ;
  //index de la couleur
  return jeu.noms_couleurs.size() - 1 ;
}

void jeu_ajout_liaison(Jeu& jeu, const Liaison& liaison) {
  //enregistrement de la liaison en vrac
  jeu.liaisons.push_back(liaison) ;
  jeu.liaisons.back().proprietaire = 0 ;
  //enregistrement pour la seconde ville en echangeant les indices
  jeu.liaisons.push_back(liaison) ;
  jeu.liaisons.back().ville1 = liaison.ville2 ;
  jeu.liaisons.back().ville2 = liaison.ville1 ;
  jeu.liaisons.back().proprietaire = 0 ;
  //decompte du nombre de liaisons par ville
  ++jeu.index_ville[liaison.ville1] ;
  ++jeu.index_ville[liaison.ville2] ;
}

void jeu_ajout_objectif(Jeu& jeu, const Objectif& objectif) {
  //ajout par defausse dans la pioche
  pioche_defausse(jeu.objectifs, &objectif) ;
  //enregistrement pour les infos au joueur
  jeu.info_objectifs.push_back(objectif) ;
}

void jeu_ajout_carte(Jeu& jeu, const Carte& carte) {
  //comptabilisation des cartes
  ++jeu.nb_cartes[carte.couleur] ;
  //ajout par defausse dans la pioche
  pioche_defausse(jeu.cartes, &carte) ;
}

int jeu_ajout_joueur(Jeu& jeu, int mdp) {
  //enregistrement du mot de passe
  jeu.joueurs.push_back(mdp) ;
  //index du joueur
  return jeu.joueurs.size() - 1 ;
}

//fonctions utilitaires pour parcourir les liaisons d'une ville
//l'index stocke pour la ville est l'index "end" de la premiere liaison qui ne
//la concerne pas. "begin" est 0 pour la ville 0 ou l'index precedent sinon

typedef std::vector<Liaison>::iterator liaison_it ;
typedef std::vector<Liaison>::const_iterator const_liaison_it ;

const_liaison_it ville_const_begin(const Jeu& jeu, int ville) {
    int startindex = ville ? jeu.index_ville[ville-1] : 0 ;
    return jeu.liaisons.begin() + startindex ;
}

const_liaison_it ville_const_end(const Jeu& jeu, int ville) {
    return jeu.liaisons.begin() + jeu.index_ville[ville] ;
}

liaison_it ville_begin(Jeu& jeu, int ville) {
    int startindex = ville ? jeu.index_ville[ville-1] : 0 ;
    return jeu.liaisons.begin() + startindex ;
}

liaison_it ville_end(Jeu& jeu, int ville) {
    return jeu.liaisons.begin() + jeu.index_ville[ville] ;
}

//verification de l'identite du joueur
static bool joueur_check(const Jeu& jeu, int joueur, int mdp) {
  return joueur > 0 
    && joueur < (int) jeu.joueurs.size() 
    && jeu.joueurs[joueur] == mdp ;
}

//fonctions utilitaires pour gerer l'historique de la partie

static void log(Jeu& jeu, ActionType type, int joueur, int info1, int info2) {
  jeu.actions.push_back(Action{type, joueur, {info1, info2}}) ;
}

void jeu_debut_partie(Jeu& jeu) {
  //sectionnement du tableau de liaisons par ville
  for(unsigned int i = 1; i < jeu.index_ville.size(); ++i) {
    jeu.index_ville[i] += jeu.index_ville[i-1] ;
  }
  //repartition des liaisons en vrac dans les sections
  std::vector<Liaison> buf(jeu.liaisons.size()) ;
  for(const Liaison& l : jeu.liaisons) {
    buf[jeu.index_ville[l.ville1] - 1] = l ;
    --jeu.index_ville[l.ville1] ;
  }
  jeu.liaisons.swap(buf) ;
  //indices des zones de liaisons dans le tableau  pour chaque ville
  for(unsigned int i = 0; i < jeu.index_ville.size()-1; ++i) {
    jeu.index_ville[i] = jeu.index_ville[i+1] ;
  }
  jeu.index_ville.back() = jeu.liaisons.size() ;
  //tri des liaisons au sein des sections pour permettre la dichotomie
  //probablement une perte de temps pour 5-6 liaisons par ville.
  for(unsigned int i = 0; i < jeu.index_ville.size(); ++i) {
    std::sort(
        ville_begin(jeu, i), 
        ville_end(jeu, i),
        [](const Liaison& l1, const Liaison& l2) {
          return l1.ville2 < l2.ville2 ;
        }) ;
  }
  //initialisation des cartes visibles
  for(int i = 0; i < 5; ++i) {
    pioche_pioche(jeu.cartes, jeu.cartes_visibles + i) ;
  }
}

//interactions avec la partie

////informations generales

int jeu_nb_villes(const Jeu& jeu) {
  return jeu.noms_villes.size() ;
}

const char* jeu_nom_ville(const Jeu& jeu, int ville) {
  return jeu.noms_villes[ville].c_str() ;
}

int jeu_nb_couleurs(const Jeu& jeu) {
  return jeu.noms_couleurs.size() ;
}

const char* jeu_nom_couleur(const Jeu& jeu, int couleur) {
  return jeu.noms_couleurs[couleur].c_str() ;
}

int jeu_nb_joueurs(const Jeu& jeu) {
  return jeu.joueurs.size() - 1 ;
}

////liaisons

Liaison jeu_liaison(const Jeu& jeu, int ville1, int ville2) {
  //liaison requete et resultat
  Liaison query ;
  query.ville1 = ville1 ;
  query.ville2 = ville2 ;
  query.longueur = 0 ;
  query.couleur = 0 ;
  query.proprietaire = 0 ;
  //dichotomie
  const_liaison_it match = std::lower_bound(
      ville_const_begin(jeu, ville1),
      ville_const_end(jeu, ville1),
      query,
      [](const Liaison& l1, const Liaison& l2) {
        return l1.ville2 < l2.ville2 ;
      }) ;
  //verification de l'existence de la liaison
  if(match != ville_const_end(jeu, ville1) && match->ville2 == ville2) {
    return *match ;
  }
  return query ;
}

int jeu_ville_nb_liaisons(const Jeu& jeu, int ville) {
  //taille de la zone
  return ville_const_end(jeu, ville) - ville_const_begin(jeu, ville) ;
}

Liaison jeu_ville_liaison(const Jeu& jeu, int ville, int index) {
  //debut de la zone du tableau correspondant a ville
  return *(ville_const_begin(jeu, ville) + index) ;

}

void jeu_prendre_liaison(Jeu& jeu, int ville1, int ville2, int joueur, int mdp) {
  //verification du joueur
  if(!joueur_check(jeu, joueur, mdp)) return ;
  //liaison requete
  Liaison query ;
  query.ville2 = ville2 ;
  //dichotomie
  liaison_it match = std::lower_bound(
      ville_begin(jeu, ville1),
      ville_end(jeu, ville1),
      query,
      [](const Liaison& l1, const Liaison& l2) {
        return l1.ville2 < l2.ville2 ;
      }) ;
  //verification de l'existence de la liaison et de son proprietaire
  if(match != ville_end(jeu, ville1) && match->ville2 == ville2) {
    if(match->proprietaire == 0) {
      match->proprietaire = joueur ;
      if(ville1 < ville2){
        log(jeu, PRENDRE_LIAISON, joueur, ville1, ville2) ;
      } else {
        jeu_prendre_liaison(jeu, ville2, ville1, joueur, mdp) ;
      }
    }
  }
}

////cartes

int jeu_nb_cartes_couleur(const Jeu& jeu, int couleur) {
  return jeu.nb_cartes[couleur] ;
}

Carte jeu_carte_visible(const Jeu& jeu, int index) {
  return jeu.cartes_visibles[index] ;
}

Carte jeu_pioche_cache(Jeu& jeu, int joueur, int mdp) {
  //carte cible
  Carte carte ;
  carte.couleur = -1 ;
  //verification du joueur
  if(!joueur_check(jeu, joueur, mdp)) return carte;
  //pioche
  pioche_pioche(jeu.cartes, &carte) ;
  log(jeu, PIOCHE_CACHE, joueur, -2, -2) ;
  return carte ;
}

Carte jeu_pioche_visible(Jeu& jeu, int index, int joueur, int mdp) {
  //carte a retourner
  Carte carte = jeu.cartes_visibles[index] ;
  //verification du joueur
  if(!joueur_check(jeu, joueur, mdp)) {
    carte.couleur = -1 ;
    return carte ;
  }
  //remplacement de la carte
  pioche_pioche(jeu.cartes, jeu.cartes_visibles + index) ;
  log(jeu, PIOCHE_VISIBLE, joueur, carte.couleur, -2) ;
  return carte ;
}

void jeu_defausse(Jeu& jeu, const Carte& carte, int joueur, int mdp) {
  //verification du joueur
  if(!joueur_check(jeu, joueur, mdp)) return ;
  //defausse
  pioche_defausse(jeu.cartes, &carte) ;
  log(jeu, DEFAUSSE_CARTE, joueur, carte.couleur, -2) ;
}

////objectifs

int jeu_nb_objectifs(const Jeu& jeu) {
  return jeu.info_objectifs.size() ;
}

Objectif jeu_info_objectif(const Jeu& jeu, int index) {
  return jeu.info_objectifs[index] ;
}

Objectif jeu_pioche_objectif(Jeu& jeu, int joueur, int mdp) {
  //objectif cible
  Objectif objectif ;
  objectif.points = -1 ;
  //verification du joueur
  if(!joueur_check(jeu, joueur, mdp)) return objectif;
  //pioche
  pioche_pioche(jeu.objectifs, &objectif) ;
  log(jeu, PIOCHE_OBJECTIF, joueur, -2, -2) ;
  return objectif ;
}

void jeu_defausse_objectif(Jeu& jeu, const Objectif& objectif, int joueur, int mdp) {
  //verification du joueur
  if(!joueur_check(jeu, joueur, mdp)) return ;
  //defausse
  pioche_defausse(jeu.objectifs, &objectif) ;
  log(jeu, DEFAUSSE_OBJECTIF, joueur, -2, -2) ;
}

////historique

int jeu_nb_actions(const Jeu& jeu) {
  return jeu.actions.size() ;
}

Action jeu_action(const Jeu& jeu, int index) {
  return jeu.actions[index] ;
}

void action_affiche(const Jeu& jeu, const Action& action) {
  switch(action.type) {
    case PIOCHE_VISIBLE : 
      {
        std::cout << "pioche visible de " << jeu_nom_couleur(jeu, action.info[0]) 
                  << " par " << action.joueur << std::endl ;
        break ;
      }
    case PIOCHE_CACHE : 
      {
        std::cout << "pioche cachée par " << action.joueur << std::endl ;
        break ;
      }
    case DEFAUSSE_CARTE : 
      {
        std::cout << "défausse par " << action.joueur << std::endl ;
        break ;
      }
    case PIOCHE_OBJECTIF : 
      {
        std::cout << "pioche d'objectif par " << action.joueur << std::endl ;
        break ;
      }
    case DEFAUSSE_OBJECTIF : 
      {
        std::cout << "défausse d'objectif par " << action.joueur << std::endl ;
        break ;
      }
    case PRENDRE_LIAISON : 
      {
        std::cout << "prise de la liaison entre " << jeu_nom_ville(jeu,action.info[0])
                  << " et " << jeu_nom_ville(jeu,action.info[1])
                  << " par " << action.joueur << std::endl ;
        break ;
      }
  }
}

void jeu_affiche_historique(const Jeu& jeu) {
  for(const Action& action : jeu.actions) {
    action_affiche(jeu, action) ;
  }
}
