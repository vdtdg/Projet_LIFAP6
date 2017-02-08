#include "jeu_io.hpp"

#include <sstream>
#include <fstream>
#include <string>
#include <cassert>
#include <cmath>
#include <unordered_map>

enum ConfigSection {
  NO_SECTION,
  VILLE,
  COULEURS,
  LIAISONS,
  OBJECTIFS,
  CARTES
} ;

static ConfigSection check_section(std::string& str) {
  if(str == "[villes]") return VILLE ;
  if(str == "[couleurs]") return COULEURS ;
  if(str == "[liaisons]") return LIAISONS ;
  if(str == "[objectifs]") return OBJECTIFS ;
  if(str == "[cartes]") return CARTES ;
  return NO_SECTION ;
}

void jeu_import(Jeu& jeu, std::istream& input) {
  //read buffer
  std::string buf ;
  //parse input stream
  ConfigSection section = NO_SECTION ;
  int card_colors = 0 ;
  while(std::getline(input, buf)) {
    ConfigSection sec_parse = check_section(buf) ;
    if(sec_parse) {
      section = sec_parse ;
    } else {
      switch(section) {
        case VILLE : 
          {
            if(!buf.empty()) {
              std::string name ;
              std::stringstream ss(buf) ;
              ss >> name ;
              jeu_ajout_ville(jeu, name.c_str()) ;
            }
            break ;
          }
        case COULEURS : 
          {
            if(!buf.empty()) {
              std::string name ;
              std::stringstream ss(buf) ;
              ss >> name ;
              jeu_ajout_couleur(jeu, name.c_str()) ;
            }
            break ;
          }
        case LIAISONS : 
          {
            Liaison l ;
            std::stringstream ss(buf) ;
            ss >> l.ville1 >> l.ville2 >> l.couleur >> l.longueur ;
            if(!ss.fail()) {
              jeu_ajout_liaison(jeu, l) ;
            }
            break ;
          }
        case OBJECTIFS : 
          {
            Objectif o ;
            std::stringstream ss(buf) ;
            ss >> o.ville1 >> o.ville2 >> o.points ;
            if(!ss.fail()) {
              jeu_ajout_objectif(jeu, o) ;
            }
            break ;
          }
        case CARTES : 
          {
            std::stringstream ss(buf) ;
            int nb_cartes = 0 ;
            ss >> nb_cartes ;
            if(! ss.fail()) {
              Carte c ;
              c.couleur = card_colors ;
              for(int i = 0; i < nb_cartes; ++i) {
                jeu_ajout_carte(jeu, c) ;
              }
              ++card_colors ;
              if(card_colors == jeu_nb_couleurs(jeu)) {
                section = NO_SECTION ;
              }
            }
            break ;
          }
        case NO_SECTION :
          {
            break ;
          }
      }
    }
  }
}

void jeu_export(const Jeu& jeu, std::ostream& output) {
  //villes
  output << "[villes]" << std::endl ;
  for(int i = 0; i < jeu_nb_villes(jeu); ++i) {
    output << jeu_nom_ville(jeu, i) << std::endl ;
  }
  output << std::endl ;

  //couleurs
  output << "[couleurs]" << std::endl ;
  for(int i = 0; i < jeu_nb_couleurs(jeu); ++i) {
    output << jeu_nom_couleur(jeu, i) << std::endl ;
  }
  output << std::endl ;

  //liaisons
  output << "[liaisons]" << std::endl ;
  for(int i = 0; i < jeu_nb_villes(jeu); ++i) {
    for(int j = 0; j < jeu_ville_nb_liaisons(jeu, i); ++j) {
      Liaison l = jeu_ville_liaison(jeu, i, j) ;
      if(l.ville1 < l.ville2) {
        output << l.ville1 << " " 
          << l.ville2 << " "
          << l.couleur << " "
          << l.longueur << std::endl ;
      }
    }
  }
  output << std::endl ;

  //objectifs
  output << "[objectifs]" << std::endl ;
  for(int i = 0; i < jeu_nb_objectifs(jeu); ++i) {
    Objectif o = jeu_info_objectif(jeu, i) ;
    output << o.ville1 << " " 
      << o.ville2 << " " 
      << o.points << std::endl ;
  }
  output << std::endl ;

  //cartes
  output << "[cartes]" << std::endl ;
  for(int i = 0; i < jeu_nb_couleurs(jeu); ++i) {
      output << jeu_nb_cartes_couleur(jeu, i) << std::endl ;
  }
}

void jeu_gen_villes(
    Jeu& jeu ,
    int nb_villes,
    const std::string& filename
    ) {
  int n = 0 ;
  std::string buf ;
  std::fstream file(filename) ;
  std::vector< std::string > output(nb_villes) ;
  for(int i = 0; i < nb_villes ; ++i) {
    if(std::getline(file, buf)) {
      output[i].swap(buf) ;
      ++n ;
    } else {
      std::cout << "attention : " << n-1 << " villes maximum." << std::endl ;
      return ;
    }
  }
  while(std::getline(file, buf)) {
    int pos = rand() % n ;
    if(pos < nb_villes) {
      output[pos].swap(buf) ;
    }
    ++n ;
  }

  for(int i = 0; i < nb_villes; ++i) {
    jeu_ajout_ville(jeu, output[i].c_str()) ;
  }
}
 
void jeu_gen_couleurs(
    Jeu& jeu,
    int nb_couleurs
    ) {
  static const char* colors[10] = {
    "rouge",
    "bleu",
    "vert",
    "jaune",
    "violet",
    "orange",
    "brun",
    "rose",
    "blanc",
    "noir"
  } ;
  if(nb_couleurs > 10) {
    std::cout << "attention : 10 couleurs maximum." << std::endl ;
    nb_couleurs = 10 ;
  }

  for(int i = 0; i < nb_couleurs; ++i) {
    jeu_ajout_couleur(jeu, colors[i]) ;
  }
}

static int base_index(int i, int n) {
  return (2*n*i - i*i - i)/2 ;
}

static void index_to_pair(int i, int n, int& i1, int& i2) {
  i1 = (2*n - 1 - sqrt((1-2*n)*(1-2*n) - 8*i)) / 2 ;
  i2 = i1 + i - base_index(i1, n) + 1 ;
}

void jeu_gen_liaisons(
    Jeu& jeu,
    int nb_liaisons,
    int longueur_min,
    int longueur_max
    ) {
  int nb_villes = jeu_nb_villes(jeu) ;
  int nb_pairs = nb_villes*(nb_villes - 1) / 2 ;
  int nb_couleurs = jeu_nb_couleurs(jeu) ;

  if(nb_liaisons > nb_pairs) {
    std::cout << "attention : " << nb_pairs << " liaisons maximum." << std::endl ;
    nb_liaisons = nb_pairs ;
  }

  std::unordered_map<int,int> hits ;
  typedef std::unordered_map<int,int>::iterator hit_it ;

  Liaison buf ;
  buf.proprietaire = -1 ;
  for(int i = 0; i < nb_liaisons; ++i) {
    int index = rand() % nb_pairs ;
    int pair = index ;
    hit_it it = hits.find(pair) ;
    while(it != hits.end()) {
      pair = it->second ;
      it = hits.find(pair) ;
    }

    hits[index] = nb_pairs - i - 1 ;
    index_to_pair(pair, nb_villes, buf.ville1, buf.ville2) ;
    buf.couleur = rand() % nb_couleurs ;
    buf.longueur = longueur_min + rand() % (longueur_max - longueur_min + 1) ;
    jeu_ajout_liaison(jeu, buf) ;
  }
}

void jeu_gen_objectifs(
    Jeu& jeu,
    int nb_objectifs,
    int points_min,
    int points_max
    ){
  int nb_villes = jeu_nb_villes(jeu) ;
  int nb_pairs = nb_villes*(nb_villes - 1) / 2 ;

  if(nb_objectifs > nb_pairs) {
    std::cout << "attention : " << nb_pairs << " objectifs maximum." << std::endl ;
    nb_objectifs = nb_pairs ;
  }

  std::unordered_map<int,int> hits ;
  typedef std::unordered_map<int,int>::iterator hit_it ;

  Objectif buf ;
  for(int i = 0; i < nb_objectifs; ++i) {
    int index = rand() % nb_pairs ;
    int pair = index ;
    hit_it it = hits.find(pair) ;
    while(it != hits.end()) {
      pair = it->second ;
      it = hits.find(pair) ;
    }

    hits[index] = nb_pairs - i - 1 ;
    index_to_pair(pair, nb_objectifs, buf.ville1, buf.ville2) ;
    buf.points = points_min + rand() % (points_max - points_min + 1) ;
    jeu_ajout_objectif(jeu, buf) ;
  }
}
