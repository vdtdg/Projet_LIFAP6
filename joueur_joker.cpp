#include "joueur_joker.hpp"
#include <queue>  // std::priority_queue
#include <limits> // std::numeric_limits
#include <iostream> // debuging purpose.

void joueur_simple_init(JoueurSimple& joueur) {
  joueur.points = 0;
  joueur.mdp = rand() % 2400 + 1 ;
  joueur.indice = -1;
  // objectif initialisé à 0
  joueur.obj.ville1 = 0;
  joueur.obj.ville2 = 0;
  joueur.obj.points = 0;
  // rien a faire pour joueur.chemin_obj ni pour wagons : vector.
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
  int nombre_w_p; // NOMBRE de Wagons de cette couleur que je Possède.
  int nb_carte_piochee = 0;
  
  // DEBUT du tour.
  
  if(joueur.chemin_obj.empty()) { // Si le chemin vers l'objectif est vide
    if(joueur.obj.ville1 == 0 && joueur.obj.ville2 == 0) { // Si il n'y a pas d'objectif (ville1=ville2=0) 
      joueur.obj = jeu_pioche_objectif(jeu, joueur.indice, joueur.mdp);
      std::cout << "Message de debug : arrive après piocher un obj" << std::endl;
      joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice); // on cherche son plus court chemin
    }
    else if(joueur.obj.ville1 == joueur.obj.ville2) { // Si le précédent obj a été rempli
      // defausse de l'obj
      jeu_defausse_objectif(jeu, joueur.obj, joueur.indice, joueur.mdp);
      // pioche d'un nouvel obj
      joueur.obj = jeu_pioche_objectif(jeu, joueur.indice, joueur.mdp);
      // calcul du plus court chemin vers celui ci
      joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice); // On pourrait factoriser ça après le if/else en fait (TODO)
    }
    else{ // Si on a un obj mais pas de chemin (normalement n'arrive pas)
      std::cout << "Je ne devrais pas arriver" << std::endl;
      joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice); // on cherche son plus court chemin
    }
  }
  else { // Sinon, on vérifie si le chemin est encore valide.
    std::cout << " Vérification du chemin " << std::endl;
    for(unsigned int i = 0; i < joueur.chemin_obj.size() - 1; i++) { // On parcourt le chemin
      Liaison temp = jeu_liaison(jeu, joueur.chemin_obj[i], joueur.chemin_obj[i+1]);
      if(temp.proprietaire != 0 && temp.proprietaire != joueur.indice) { // Si une des liaisons a un proprietaire different de moi, on cherche un nouveau chemin
	joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice);
	break; // et on sort de la boucle
      }
    }
  }

  std::cout << "Je compte le nombre de carte que j'ai" << std::endl;
  // Je regarde le nombre de carte wagon nécessite la prochaine liaison
  int ville1 = joueur.chemin_obj.back(); // dernier elem
  int ville2 = joueur.chemin_obj[joueur.chemin_obj.size() - 1]; //Avant dernier elem
  Liaison prochain = jeu_liaison(jeu, ville1, ville2);

  // Je compte le nombre de carte de cette couleur que je possède
  for(unsigned int i = 0; i < joueur.wagons.size(); i++) {
    if(joueur.wagons[i].couleur == prochain.couleur) {
      nombre_w_p++;
    }
  }
  
  // Si j'ai assez de cartes :
  if(prochain.longueur - nombre_w_p <= 0) {
    std::cout << "J'ai assez de carte ! ;)" << std::endl;
    // Je prend la liaison
    jeu_prendre_liaison(jeu, ville1, ville2, joueur.indice, joueur.mdp);
    // J'enlève les cartes wagons correspondant à mon achat
    
    // Je l'enlève de ma liste des liaisons à atteindre en changeant l'objectif !
    joueur.chemin_obj.pop_back();
    // La ville de départ de l'objectif devient celle atteinte dernièrement. NON MAUVAISE IDEE : PAS DE MODIF DE L'OBJECTIF
    // joueur.obj.ville1 = ville2;  
  }
  // Sinon
  else {
    std::cout << "Je n'ai pas assez de carte ! :(" << std::endl;
  //     Je parcours les cartes de la pioche découverte
    for(int i = 0; i < 5; i++) {
  //     Si la carte correspond à la carte que je veux
      Carte temp = jeu_carte_visible(jeu, i);
      if(temp.couleur == prochain.couleur) {
  //          Je la prend
	joueur.wagons.push_back(jeu_pioche_visible(jeu, i, joueur.indice, joueur.mdp));
	nb_carte_piochee ++;
  //          Si nb_carte_piochee == 2 :
  //             FIN du tour.
	if(nb_carte_piochee ==2) {
	  break; // On sort de la boucle quand on a pioché 2 cartes.
	}
      }
    }
    
    //     Je pioche 2-nb_carte_piochee déjà prises.
    for(int i = 0 ; i < 2 - nb_carte_piochee ; i++) { // s'éxecute 2, 1 ou 0 fois.
      joueur.wagons.push_back(jeu_pioche_cache(jeu, joueur.indice, joueur.mdp));
    }
  }
  // FIN du tour.
}

liste_liaison plus_court_chemin(Jeu& jeu, const Objectif &obj, const int indice_joueur) { // Algo de Dijkstra (voir cours)
  std::cout << "Je m'aprete à calculer un pcc " << std::endl;
  liste_liaison pcc; // pcc = plus court chemin.
  std::priority_queue<Couple_fp> fp; // <index ville, poids>
  int ville_de_depart = obj.ville1;
  const int nb_de_ville = jeu_nb_villes(jeu);
  
  //int poids[nb_de_ville] = {std::numeric_limits<int>::max()};
  std::vector<int> poids(nb_de_ville);
  poids.assign(nb_de_ville, std::numeric_limits<int>::max());
  poids[ville_de_depart] = 0;
  //int precedent[nb_de_ville] = {0};
  std::vector<int> precedent(nb_de_ville);
  precedent.assign(nb_de_ville, 0);
  
  Couple_fp vdd; // vdd = ville de départ
  vdd.ville = ville_de_depart;
  vdd.poids = poids[ville_de_depart]; // = 0
  fp.push(vdd);

  while(!fp.empty()) { //tant que la file n'est pas vide
    Couple_fp s = fp.top();
    int nb_de_voisin_de_s = jeu_ville_nb_liaisons(jeu, s.ville);
    for(int i=0; i<nb_de_voisin_de_s; i++) { // /!\ bornes à verif
      Liaison temp = jeu_ville_liaison(jeu, s.ville, i);
      if(temp.proprietaire == 0 || temp.proprietaire == indice_joueur) {// On s'assure que la liaison n'appartient à personne ! Pas besoin de calculer le plus court chemin via cette liaison si elle est dejà prise !
	if(s.poids + temp.longueur < poids[temp.ville2]) {
	  poids[temp.ville2] = s.poids + temp.longueur; // 2 fois le mm calcul
	  precedent[temp.ville2] = s.ville;
	  Couple_fp voisin;
	  voisin.ville = temp.ville2;
	  voisin.poids=poids[temp.ville2];
	  fp.push(voisin);
	}
      }
    }
    fp.pop(); // enlève s puisque s à forcément un poids plus faible que les autres. (pb si poids < 0)
  }
  
  // La premiere liaison à effectuer doit être au top du vector
  std::cout<< "Le chemin allant de "<< obj.ville2 << " à " << obj.ville1 << " est :"; // à l'envers
  int prec;
  
  pcc.push_back(obj.ville2);
  std::cout << " " << obj.ville2;
  prec = precedent[obj.ville2];
  
  while(prec != obj.ville1) {
    std::cout << " " << prec;
    pcc.push_back(prec);
    prec = precedent[prec];
  }

  pcc.push_back(prec); // On oublie pas la derniere valeurs.
  std::cout << " " << prec << std::endl;
  return pcc;
}
 
// avec l'aide de stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects
 bool operator<(const Couple_fp& a, const Couple_fp& b) {
   return a.poids > b.poids; // Ainsi, la plus petite priorité sera au sommet de la file.
 }
