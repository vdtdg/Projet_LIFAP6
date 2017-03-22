#include "joueur_simple.hpp"

void joueur_simple_init(JoueurSimple& joueur) {
  joueur.points = 0;
  mdp = rand() % 2400 + 1 ;
  indice = -1;
  
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
  // DEBUT du tour.
  // Si j'ai pas d'objectif, j'en pioche un, et j'applique dijkstra dessus -> chemin (liste de liaison)
  // Je vérifie que les liaisons de chemin n'ai pas de propriétaire, sinon j'applique Dijkstra.
  // Je regarde le nombre de carte wagon nécessite la prochaine liaison
  // Si j'ai assez de cartes, je la prend, et je l'enlève de ma liste
  // Sinon je regarde dans les cartes découvertes si elles correspondent aux cartes que je veux, je prends un maximum de deux. Je pioche 2-nb de carte déjà prises.
  // FIN du tour.
}
