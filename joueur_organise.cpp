#include "joueur_organise.hpp"

#include <iostream> // debuging purpose.

void joueur_orga_init(JoueurOrga& joueur) {
  joueur.poids_chemin_obj = 0;
  joueur.mdp = rand() % 2400 + 1 ;
  joueur.indice = -1;
  // objectif initialisé à 0
  joueur.obj.ville1 = 0;
  joueur.obj.ville2 = 0;
  joueur.obj.points = 0;
  // rien a faire pour joueur.chemin_obj ni pour wagons : vector.
}

void joueur_orga_suppr(JoueurOrga& joueur) {
  // nothing to do.
}

int joueur_orga_mdp(JoueurOrga& joueur) {
  return joueur.mdp;
}

void joueur_orga_config_indice(JoueurOrga& joueur, int indice) {
  joueur.indice = indice;
}

void joueur_orga_tour(Jeu& jeu, JoueurOrga& joueur) {
  int nombre_w_p = 0; // NOMBRE de Wagons de cette couleur que je Possède.
  int nb_carte_piochee = 0;
  bool recalcul = false;
  
  // DEBUT du tour.
  // uniquement le premier tour.
  if(jeu_get_tour(jeu) == 0) {
    // On pioche 4 cartes.
    if(DEBUG) { std::cout << "C'est le premier tour, je pioche 4 cartes" << std::endl; }
    for(int firstround = 0 ; firstround < 4 ; firstround++) {
      Carte temp = jeu_pioche_cache(jeu, joueur.indice, joueur.mdp);
      joueur.wagons.push_back(temp);
    }
  }
  
  if(joueur.chemin_obj.empty() || joueur.chemin_strategique.empty()) { // Si le chemin vers l'objectif est vide
    if(joueur.obj.ville1 == 0 && joueur.obj.ville2 == 0) { // Si il n'y a pas d'objectif (ville1=ville2=0)
      if(DEBUG) { std::cout << "Je pioche un objectif car je n'en avais pas avant." << std::endl; }
      joueur.obj = jeu_pioche_objectif(jeu, joueur.indice, joueur.mdp);
      // on cherche son plus court chemin sans enlever de liaison (-1, -1)
      joueur.chemin_obj = plus_court_chemin_orga(jeu, joueur.obj, joueur.indice, -1, -1); 
    }
    else  { // Si le précédent obj a été rempli
      if(DEBUG) { std::cout << "Je pioche un objectif car j'ai terminé l'objectif précédent." << std::endl; }
      // defausse de l'obj
      jeu_defausse_objectif(jeu, joueur.obj, joueur.indice, joueur.mdp);
      // pioche d'un nouvel obj
      joueur.obj = jeu_pioche_objectif(jeu, joueur.indice, joueur.mdp);
      // calcul du plus court chemin vers celui ci
      joueur.chemin_obj = plus_court_chemin_orga(jeu, joueur.obj, joueur.indice, -1, -1);
      // reinitialisation de la valeur du chemin de l'obj. (calculé après)
      joueur.poids_chemin_obj = 0;
    }
  }
  else { // Sinon, on vérifie si le chemin est encore valide. (i.e. qu'il n'y a pas de proprio etc.)
    if(DEBUG) {  std::cout << " Vérification du chemin "; }
    for(unsigned int i = 0; i < joueur.chemin_obj.size() - 1; i++) { // On parcourt le chemin
      Liaison temp = jeu_liaison(jeu, joueur.chemin_obj[i], joueur.chemin_obj[i+1]);
      if(temp.proprietaire != 0 && temp.proprietaire != joueur.indice) { // Si une des liaisons a un proprietaire different de moi, on cherche un nouveau chemin
	if(DEBUG) { std::cout << "Une des liaisons a été prise par un adversaire. Je recalcule mon chemin." << std::endl; }
	joueur.chemin_obj = plus_court_chemin_orga(jeu, joueur.obj, joueur.indice, -1, -1);
	recalcul = true;
      }
    }
    if(DEBUG && !recalcul) {  std::cout << "...OK" << std::endl; }
  }
  
  /* Changement par rapport au joueur_simple */

  // Ici, on calcule l'importance de chaque liaison du chemin vers l'objectif

  // Si tout cela n'a pas été encore calculé (on vient de recevoir l'obj par ex), ou
  // encore que le chemin ait changé (un adversaire prend une liaison de notre chemin
  // on calcule tout cela, sinon, pas besoin, on passe à la suite !
  if(joueur.poids_chemin_obj == 0 || recalcul == true) { 
    for(unsigned int i = 0; i < joueur.chemin_obj.size()-1; i++) {
      Liaison temp = jeu_liaison(jeu, joueur.chemin_obj[i], joueur.chemin_obj[i+1]); 
      joueur.poids_chemin_obj += temp.longueur;
    }
    if(DEBUG) { std::cout << "Le poids du plus court chemin est " << joueur.poids_chemin_obj << std::endl; }
  
    // Crée une liste des liaisons à prendre, trier par ordre d'importance.
    for(unsigned int i = 0; i < joueur.chemin_obj.size()-1; i++) {
      liste_liaison liste_temp;
      int ville_a = joueur.chemin_obj[i];
      int ville_b = joueur.chemin_obj[i+1];

      if(DEBUG) { std::cout << "Le plus court chemin sans la liaison allant de " << ville_a << " à " << ville_b << " est : " << std::endl; }
      liste_temp = plus_court_chemin_orga(jeu, joueur.obj, joueur.indice, ville_a, ville_b);
      
      Liaison_poids nextone;
      nextone.ville1 = ville_a;
      nextone.ville2 = ville_b;

      // Ici deux solutions : soit sans cette liaison, pas de chemin possible, à
      // ce moment la elle est très importante et on lui attribue le poids +infini,
      // soit il y a un autre chemin et à ce moment la on calcule son poids pour
      // ensuite faire la différence etc...
      if(liste_temp.back() == -1) {
	nextone.poids = std::numeric_limits<int>::max();
	if(DEBUG) { std::cout << "La liaison " << ville_a << " -> " << ville_b << " est essentielle ! Son poids est +inf" << std::endl; }
      }
      else {
	if(DEBUG) { std::cout << "La liaison " << ville_a << " -> " << ville_b << " a pour poids "; }
	int poids_temp = 0;
	
	for(unsigned int j = 0; j < liste_temp.size()-1 ; j++) {
	  Liaison temp = jeu_liaison(jeu, liste_temp[j], liste_temp[j+1]);
	  poids_temp += temp.longueur;
	  if(DEBUG) { std::cout << temp.longueur << " + "; }
	}
	// Plus le poids est grand, plus la liaison est importante à prendre, car sans
	// celle-ci, il faudra faire un plus grand trajet.
	nextone.poids  = poids_temp - joueur.poids_chemin_obj;
	if(DEBUG) { std::cout << "(-" << joueur.poids_chemin_obj << ") = " << nextone.poids << std::endl; }
      }
      joueur.chemin_strategique.push(nextone);
    }
  }

    
  // Je regarde quelle est la prochaine liaison à prendre
  Liaison_poids prio = joueur.chemin_strategique.top();
  int ville1 = prio.ville1;
  int ville2 = prio.ville2;
  Liaison prochain = jeu_liaison(jeu, ville1, ville2);
  /* FIN du changement. */
  
  if(DEBUG) { std::cout << "La liaison que je vais prendre va de " << prochain.ville1 << " à " << prochain.ville2 << " et est de longueur " << prochain.longueur << ". Elle appartient à " << prochain.proprietaire << " et est de la couleur " << prochain.couleur << "." << std::endl; }
  
  while(prochain.proprietaire == joueur.indice) { // Si prochain est à moi, je passe à celle d'après
    joueur.chemin_strategique.pop();
    if(joueur.chemin_strategique.size() == 0) { // Si j'ai fini l'objectif, je termine le tour. 
      if(DEBUG) { std::cout << "Mon objectif est terminé ! je vais attendre le prochain tour." << std::endl; }
      return;
    }
    prio = joueur.chemin_strategique.top();
    ville1 = prio.ville1; 
    ville2 = prio.ville2;
    prochain = jeu_liaison(jeu, ville1, ville2);
    if(DEBUG) { std::cout << "La liaison que je vais prendre va de " << prochain.ville1 << " à " << prochain.ville2 << " et fait " << prochain.longueur << ". Elle appartient à " << prochain.proprietaire << " et est de la couleur " << prochain.couleur << "." << std::endl; }
  }
  
  // Je compte le nombre de carte de cette couleur que je possède
  for(unsigned int i = 0; i < joueur.wagons.size(); i++) {
    if(DEBUG) { std::cout << "Comptage de carte : Je cherche des cartes de couleur " << prochain.couleur << " et joueur.wagons["<<i<<"].couleur = " << joueur.wagons[i].couleur; }
    if(joueur.wagons[i].couleur == prochain.couleur) {
      if(DEBUG) { std::cout << "    => En voila une !" << std::endl; }
      nombre_w_p++;
    }
    else {
      if(DEBUG) { std::cout << std::endl; }
    }
  }
  
  // Si j'ai assez de cartes :
  if(prochain.longueur - nombre_w_p <= 0) {
    if(DEBUG) { std::cout << "Je possède " << nombre_w_p << " carte(s) de couleur "<<prochain.couleur<<" et j'en ai besoin de " << prochain.longueur << "." << std::endl; }
    // Je prend la liaison
    jeu_prendre_liaison(jeu, ville1, ville2, joueur.indice, joueur.mdp);
    if(DEBUG) { std::cout << "Je prends la liaison !" << std::endl; }
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
    joueur.chemin_strategique.pop();
  }
  // Sinon
  else {
    if(DEBUG) { std::cout << "Je possède " << nombre_w_p << " cartes de couleur " << prochain.couleur << " et j'en ai besoin de " << prochain.longueur << std::endl; }
    // Je parcours les cartes de la pioche découverte
    for(int i = 0; i < 5; i++) {
      if(DEBUG) { std::cout << "Je regarde la pioche découverte " << std::endl; }
      // Si la carte correspond à la carte que je veux
      Carte temp = jeu_carte_visible(jeu, i);
      if(temp.couleur == prochain.couleur) {
	if(DEBUG) { std::cout << "    ==> et je trouve la carte que je veux !" << std::endl; }
	// Je la prend
	joueur.wagons.push_back(jeu_pioche_visible(jeu, i, joueur.indice, joueur.mdp));
	nb_carte_piochee ++;
	// On sort de la boucle quand on a pioché 2 cartes.
	if(nb_carte_piochee == 2) {
	  break;
	}
      }
    }
    
    // Je pioche 2-nb_carte_piochee déjà prises.
    for(int i = 0 ; i < 2 - nb_carte_piochee ; i++) { // s'éxecute 2, 1 ou 0 fois.
      if(DEBUG) { std::cout << "Je pioche une carte dans la pioche cachee ! "; }
      Carte temp = jeu_pioche_cache(jeu, joueur.indice, joueur.mdp);
      if(DEBUG) { std::cout << "| Verification de la carte : temp.couleur = "<< temp.couleur << std::endl; }
      joueur.wagons.push_back(temp);
    }
  }
  // FIN DU TOUR
}

liste_liaison plus_court_chemin_orga(Jeu& jeu, const Objectif &obj, const int indice_joueur, const int ville_a, const int ville_b) { // Algo de Dijkstra (voir cours)
  liste_liaison pcc; // pcc = plus court chemin.
  std::priority_queue<Couple_fp> fp; // <index ville, poids>
  int ville_de_depart = obj.ville1;
  const int nb_de_ville = jeu_nb_villes(jeu);
  
  // Tableau de poids entier, poids[n° ville] = poids;
  std::vector<int> poids(nb_de_ville);
  poids.assign(nb_de_ville, std::numeric_limits<int>::max());
  poids[ville_de_depart] = 0;
  // Tableau de precedence, precedent[n° ville] = n° autreville;
  std::vector<int> precedent(nb_de_ville);
  precedent.assign(nb_de_ville, -1); // Initalisé à -1 : detection de chemin inaccessible.
  
  Couple_fp vdd; // vdd = ville de départ
  vdd.ville = ville_de_depart;
  vdd.poids = 0; // = poids[ville_de_depart]
  fp.push(vdd);

  while(!fp.empty()) { //tant que la file n'est pas vide
    Couple_fp s = fp.top();
    int nb_de_voisin_de_s = jeu_ville_nb_liaisons(jeu, s.ville);
    for(int i=0; i<nb_de_voisin_de_s; i++) { // /!\ bornes ok
      Liaison temp = jeu_ville_liaison(jeu, s.ville, i);
      if(!((temp.ville1 == ville_a && temp.ville2 == ville_b) || (temp.ville1 == ville_b && temp.ville2 == ville_a))) { // Il faut que la liaison ne soit pas celle qu'on cherche à éviter.
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
    } 
    fp.pop();
  }
  
  // La premiere liaison à effectuer doit être au top du vector
  if(DEBUG) { std::cout<< "Le chemin allant de "<< obj.ville2 << " à " << obj.ville1 << " est :"; }
  int prec;
  
  pcc.push_back(obj.ville2);
  if(DEBUG) { std::cout << " " << obj.ville2; }
  prec = precedent[obj.ville2];

  // Si la ville d'arrivée n'a pas de précédence, on renvoie une liste avec -1
  if(prec == -1) {
    if(DEBUG) { std::cout << " -> Pas de chemin possible !" << std::endl; }
    pcc.push_back(-1);
    return pcc;
  }
  
  while(prec != obj.ville1) {
    if(DEBUG) { std::cout << " " << prec; }
    pcc.push_back(prec);
    prec = precedent[prec];
  }

  pcc.push_back(prec); // On oublie pas la derniere valeurs.
  if(DEBUG) { std::cout << " " << prec << std::endl; }
  return pcc;
}
