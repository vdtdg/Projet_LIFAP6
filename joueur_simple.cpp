#include "joueur_simple.hpp"

void joueur_simple_init(JoueurSimple& joueur) {
  joueur.points = 0;
  joueur.mdp = rand() % 2400 + 1 ;
  joueur.indice = -1;
  // obj nul
  joueur.obj.ville1 = 0;
  joueur.obj.ville2 = 0;
  joueur.obj.points = 0;
}

void joueur_simple_suppr(JoueurSimple& joueur) {
  // nothing to do.
}

int joueur_simple_mdp(JoueurSimple& joueur) {
  return joueur.mdp;
}

void joueur_simple_config_indice(JoueurSimple& joueur, int indice) {
  joueur.indice = indice;
}

void joueur_simple_tour(Jeu& jeu, JoueurSimple& joueur) {
  liste_liaison mon_pcc; // mon plus court chemin
  int couleur_w_l; // couleur de la prochaine liaison
  int nombre_w_l; // longueur de la prochaine liaison
  int nombre_w_p; // nombre de wagons de cette couleur que je possède.

  int nb_carte_piochee = 0;
  // DEBUT du tour.
  // Si j'ai pas d'objectif :
  //     J'en pioche un
  //     Je le stocke dans ma structure
  if(joueur.obj.ville1 == 0 && joueur.obj.ville2 == 0) {
    pioche_pioche(jeu.objectifs, joueur.obj);
  }
  
  // J'applique dijkstra sur l'objectif -> chemin (liste de liaison)
  mon_pcc = plus_court_chemin(jeu, joueur.obj);
  
  // Je regarde le nombre de carte wagon nécessite la prochaine liaison
  couleur_w = jeu.liaisons[mon_pcc[0]].couleur;
  nombre_w = jeu.liaisons[mon_pcc[0]].longueur;
  for(int i = 0; i < joueur.wagons.size(); i++) {
    if(joueur.wagons[i].couleur == couleur_w) {
      nombre_w_p++;
    }
  }
  
  // Si j'ai assez de cartes :
  if(nombre_w - nombre_w_p <= 0) {
  //     Je prend la liaison
    void jeu_prendre_liaison(Jeu& jeu, int ville1, int ville2, int joueur, int mdp) ;
    jeu_prendre_liaison(jeu, jeu.liaisons[mon_pcc[0]].ville1, jeu.liaisons[mon_pcc[0]].ville2, joueur.indice, joueur.mdp);
  //     Je l'enlève de ma liste des liaisons à atteindre en changeant l'objectif !
  //     La ville de départ de l'objectif devient celle atteinte dernièrement.
    obj.ville1 = jeu.liaisons[mon_pcc[0]].ville2;
  }
  // Sinon
  else {
  //     Je parcours les cartes de la pioche découverte
    for(int i = 0; i < 4; i++) {
  //     Si la carte correspond à la carte que je veux
      if(jeu.carte_visibles[i] == couleur_w) {
  //          Je la prend
	jeu_pioche_visible(jeu, i, joueur.indice, joueur.mdp);
	nb_carte_piochee ++;
  //          Si nb_carte_piochee == 2 :
  //             FIN du tour.
	if(nb_carte_piochee ==2) {
	  break;
	}
      }
    }
  //     Je pioche 2-nb_carte_piochee déjà prises.
    for(int i = 0 ; i < 2 - nb_carte_piochee ; i++) {
      joueur.wagons.resize(joueur.wagons.size() + sizeof(Carte));
      pioche_pioche(jeu.cartes, joueur.wagons[joueur.wagons.size() - sizeof(Carte)]);
    }
  
  // FIN du tour.
}

liste_liaison plus_court_chemin(Jeu& jeu, const Objectif &obj) {
  liste_liaison liste_plus_court_chemin;
  


  return liste_plus_court_chemin;
}
