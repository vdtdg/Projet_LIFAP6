#include "joueur_simple.hpp"
#include <queue>  // std::priority_queue
#include <limits> // std::numeric_limits
#include <iostream> // debuging purpose.

void joueur_simple_init(JoueurSimple& joueur) {
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
  int nombre_w_p = 0; // NOMBRE de Wagons de cette couleur que je Possède.
  int nb_carte_piochee = 0;
  
  // DEBUT du tour.
  
  if(joueur.chemin_obj.empty() || joueur.chemin_obj.size() == 1) { // Si le chemin vers l'objectif est vide 
    if(joueur.obj.ville1 == 0 && joueur.obj.ville2 == 0) { // Si il n'y a pas d'objectif (ville1=ville2=0) 
      joueur.obj = jeu_pioche_objectif(jeu, joueur.indice, joueur.mdp);
     if(DEBUG) { std::cout << "Je pioche un objectif car je n'en avais pas avant." << std::endl; }
      joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice); // on cherche son plus court chemin
    }
    else  { // Si le précédent obj a été rempli
   		if(DEBUG)  { std::cout << "Je pioche un objectif car j'ai terminé l'objectif précédent." << std::endl; }
      // defausse de l'obj
      jeu_defausse_objectif(jeu, joueur.obj, joueur.indice, joueur.mdp);
      // pioche d'un nouvel obj
      joueur.obj = jeu_pioche_objectif(jeu, joueur.indice, joueur.mdp);
      // calcul du plus court chemin vers celui ci
      joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice); // On pourrait factoriser ça après le if/else en fait (TODO)
    }
  }
  else { // Sinon, on vérifie si le chemin est encore valide. A TESTER
  if(DEBUG) {  std::cout << " Vérification du chemin " << std::endl;}
    for(unsigned int i = 0; i < joueur.chemin_obj.size() - 1; i++) { // On parcourt le chemin
      Liaison temp = jeu_liaison(jeu, joueur.chemin_obj[i], joueur.chemin_obj[i+1]);
      if(temp.proprietaire != 0 && temp.proprietaire != joueur.indice) { // Si une des liaisons a un proprietaire different de moi, on cherche un nouveau chemin
	joueur.chemin_obj = plus_court_chemin(jeu, joueur.obj, joueur.indice);
	break; // et on sort de la boucle
      }
    }
  }

   if(DEBUG) {  std::cout << "Je compte le nombre de carte que j'ai" << std::endl; }
  // Je regarde le nombre de carte wagon nécessite la prochaine liaison
  int ville1 = joueur.chemin_obj.back(); // dernier elem
  int ville2 = joueur.chemin_obj[joueur.chemin_obj.size() - 2]; //Avant dernier elem ON aurait pu faire un truc propre avec un iterateur ? NON : pas de calcul intermédiaire inutile.
  Liaison prochain = jeu_liaison(jeu, ville1, ville2);
  if(DEBUG) {  std::cout << "La liaison que je vais prendre va de " << prochain.ville1 << " à " << prochain.ville2 << " et fait " << prochain.longueur << ". Elle appartient à " << prochain.proprietaire << " et est de la couleur " << prochain.couleur << "." << std::endl; }

  while(prochain.proprietaire == joueur.indice) { // Si prochain est à moi, je passe à celle d'après
    joueur.chemin_obj.pop_back();
    if(joueur.chemin_obj.size() == 1) { // Si j'ai fini l'objectif, je termine le tour. On pourrait faire un goto mais on va se faire taper sur les doigts !
  if(DEBUG) {      std::cout << "Mon objectif est terminé ! je vais attendre le prochain tour." << std::endl; }
      return;
    }
    ville1 = joueur.chemin_obj.back(); 
    ville2 = joueur.chemin_obj[joueur.chemin_obj.size() - 2]; 
    prochain = jeu_liaison(jeu, ville1, ville2);
   if(DEBUG) {   std::cout << "La liaison que je vais prendre va de " << prochain.ville1 << " à " << prochain.ville2 << " et fait " << prochain.longueur << ". Elle appartient à " << prochain.proprietaire << " et est de la couleur " << prochain.couleur << "." << std::endl; }
  }
  
  // Je compte le nombre de carte de cette couleur que je possède
  for(unsigned int i = 0; i < joueur.wagons.size(); i++) {
   if(DEBUG) {   std::cout << "Comptage de carte : Je cherche des cartes de couleur " << prochain.couleur << " et joueur.wagons["<<i<<"].couleur = " << joueur.wagons[i].couleur << std::endl; }
    if(joueur.wagons[i].couleur == prochain.couleur) {
   if(DEBUG) {     std::cout << "    => Donc je l'ajoute !" << std::endl; }
      nombre_w_p++;
    }
  }
  
  // Si j'ai assez de cartes :
  if(prochain.longueur - nombre_w_p <= 0) {
  if(DEBUG) {    std::cout << "Je possède " << nombre_w_p << " et j'en ai besoin de " << prochain.longueur << "wagons." << std::endl; }
    // Je prend la liaison
    jeu_prendre_liaison(jeu, ville1, ville2, joueur.indice, joueur.mdp);
   if(DEBUG) {   std::cout << "Je prends la liaison !" << std::endl; }
    // J'enlève les cartes wagons correspondant à mon achat
    // Defaussage des cartes wagons de ma main
    for(unsigned int i = 0; i < joueur.wagons.size(); i++) {
      if(joueur.wagons[i].couleur == prochain.couleur) {
	jeu_defausse(jeu, joueur.wagons[i], joueur.indice, joueur.mdp);
      }
    }
    // Suppression de ces dernières de ma main !
    for(unsigned int i = 0; i < joueur.wagons.size(); i++) {
      if(joueur.wagons[i].couleur == prochain.couleur) {
	joueur.wagons.erase(joueur.wagons.begin() + i);
	i--; // .erase() réorganise le tableau, et on ne veut pas sauter d'indice.
      }
    }
        
    // Je l'enlève de ma liste des liaisons à atteindre
    joueur.chemin_obj.pop_back();
   if(DEBUG) {   std::cout << "Liste des liaisons à atteindre :"; 
    for(unsigned int i = 0; i < joueur.chemin_obj.size(); i++) {
      std::cout << " " << joueur.chemin_obj[i];
    }}
  if(DEBUG) {   std::cout << " et donc joueur.chemin_obj.size() = " << joueur.chemin_obj.size() << std::endl;}
  }
  // Sinon
  else {
  if(DEBUG) {  std::cout << "Je possède " << nombre_w_p << " cartes de couleur " << prochain.couleur << " et j'en ai besoin de " << prochain.longueur 
  << std::endl; }
    //     Je parcours les cartes de la pioche découverte
   for(int i = 0; i < 5; i++) {
    if(DEBUG) {   std::cout << "Je regarde la pioche découverte " << std::endl;}
      //     Si la carte correspond à la carte que je veux
      Carte temp = jeu_carte_visible(jeu, i);
      if(temp.couleur == prochain.couleur) {
	 if(DEBUG) { std::cout << "    ==> et je trouve la carte que je veux !" << std::endl; }
	//          Je la prend
	joueur.wagons.push_back(jeu_pioche_visible(jeu, i, joueur.indice, joueur.mdp));
	nb_carte_piochee ++;
	//          Si nb_carte_piochee == 2 :
	//             FIN du tour.
	if(nb_carte_piochee == 2) {
	  break; // On sort de la boucle quand on a pioché 2 cartes.
	}
      }
    }
    
    //     Je pioche 2-nb_carte_piochee déjà prises.
    for(int i = 0 ; i < 2 - nb_carte_piochee ; i++) { // s'éxecute 2, 1 ou 0 fois.
      if(DEBUG) {  std::cout << "Je pioche une carte dans la pioche cachee ! "; }
      Carte temp = jeu_pioche_cache(jeu, joueur.indice, joueur.mdp);
     if(DEBUG) {  std::cout << "| Verification de la carte : temp.couleur = "<< temp.couleur << std::endl; }
      joueur.wagons.push_back(temp);
    }
  }
  // FIN du tour.
}

liste_liaison plus_court_chemin(Jeu& jeu, const Objectif &obj, const int indice_joueur) { // Algo de Dijkstra (voir cours)
  liste_liaison pcc; // pcc = plus court chemin.
  std::priority_queue<Couple_fp> fp; // <index ville, poids>
  int ville_de_depart = obj.ville1;
  const int nb_de_ville = jeu_nb_villes(jeu);
  
  std::vector<int> poids(nb_de_ville);
  poids.assign(nb_de_ville, std::numeric_limits<int>::max());
  poids[ville_de_depart] = 0;
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
   if(DEBUG) { std::cout<< "Le chemin allant de "<< obj.ville2 << " à " << obj.ville1 << " est :";} // à l'envers 
  int prec;
  
  pcc.push_back(obj.ville2);
  prec = precedent[obj.ville2];
  while(prec != obj.ville1) {
    pcc.push_back(prec);
    prec = precedent[prec];
  }
  pcc.push_back(prec); // On oublie pas la derniere valeurs.
 if(DEBUG) {
  for(unsigned int i = 0 ; i < pcc.size() ; i++) { // le chemin est bon.
    std::cout << " " << pcc[i];
  }
  std::cout << std::endl;
}
  return pcc;
}
 
// avec l'aide de stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects
bool operator<(const Couple_fp& a, const Couple_fp& b) { // A VERIF
   return a.poids > b.poids; // Ainsi, la plus petite priorité sera au sommet de la file.
 }
