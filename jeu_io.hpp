#ifndef ARAIL_JEU_IO_HPP
#define ARAIL_JEU_IO_HPP

#include "jeu.hpp"

#include <iostream>
#include <vector>
#include <string>

/* import / export */

void jeu_import(Jeu& jeu, std::istream& input) ;

void jeu_export(const Jeu& jeu, std::ostream& output) ;

/* generation */

void jeu_gen_villes(
    Jeu& jeu,
    int nb_villes,
    const std::string& filename = "villes.txt"
    ) ;

void jeu_gen_couleurs(
    Jeu& jeu,
    int nb_couleurs
    ) ;

void jeu_gen_liaisons(
    Jeu& jeu,
    int nb_liaisons,
    int longueur_min,
    int longueur_max
    ) ;

void jeu_gen_objectifs(
    Jeu& jeu,
    int nb_objectifs,
    int points_min,
    int points_max
    ) ;

#endif
