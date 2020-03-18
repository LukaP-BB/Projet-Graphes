/*
	Gestion de la base de personnes décédées.
 */
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include "base_deces.hpp"
 // #include "base_prenoms.hpp"

//##############################################################################
//Représentation du graphe des prénoms :

//Fonction lisant le fichier des décès et produisant un vecteur de prénoms associés
//entrée : fichier et vecteur
 void lire_base_deces(const std::string& nomfic, std::vector<std::string>& vect)
 {
      try {
            deces_file_t f(nomfic); //ouverture du fichier
            //seule la colonne "prénom" nous intéresse
            f.read_header(io::ignore_extra_column,"prenom");
            //une chaine de prénoms séparés par des espaces
            std::string prenomstr;
            while (f.read_row(prenomstr)) {
            vect.push_back(prenomstr);
            }
      } catch (const io::error::can_not_open_file& e) {
            std::cerr << e.what() << "\n";
            exit(1);
      }
}
//------------------------------------------------------------------------------
//fonction créant le graphe des prénoms à partir du vecteur de prénoms associés
//entrée : vecteur et base_deces_t
void separate(std::vector<std::string>& vect, base_deces_t& base_deces){
      //intialisation d'un tableau de 10 cases (7 prenoms max observés dans la base "large")
      std::string nom[10];
      //les chaines de caractères nécessaires pour la suite
      std::string prenom_noeud, prenom_branche, p_sep;
      std::cout << "Création du graphe..." << '\n';
      for (std::string prenoms_lies : vect) {
            //les prénoms séparés par des espaces
            std::stringstream s(prenoms_lies);
            //On place les prénoms dans un tableau et on garde combien ont été enregitsrés
            int nb_prenoms = 0; //le nombre de prénoms liés
            //on sépare les prénoms et les place dans un tableau
            while (s>>p_sep){
                  nom[nb_prenoms] = p_sep;
                  nb_prenoms ++; //le nombre de prénoms placés dans le tableau
            }
            //s'il y a un seul prénom dans le tableau, on l'ajoute directement
            if( nb_prenoms == 1){
                  //s'il est déjà dans la base, on ne fait rien
                  //s'il n'y est pas :
                  if (base_deces.find(nom[0]) == base_deces.end()){
                        //initialisaton d'une branche vide :
                        branche_t branche;
                        //création de l'entrée et de sa branche
                        base_deces.insert({nom[0], branche});
                  }
            }
            //s'il y a plusieurs prénoms liés,
            //il faut incrémenter le poids des branches
            for (int i = nb_prenoms-1; 0 <= i; i--) {
                  prenom_noeud = nom[i];
                  // std::cout << prenom_noeud << '\n';
                  for (int i = nb_prenoms-1; 0 <= i; i--) {
                        prenom_branche = nom[i];
                        if (prenom_noeud != prenom_branche){
                              base_deces[prenom_noeud][prenom_branche] ++;
                        }
                  }
            }
      }
}

//##############################################################################
//composantes connexes approche récursive :
//un algorithme de parcours en profondeur est mis en place

//première fonction englobant la fonction récursive
//et mettant en place les variables nécessaires
void creaction_connexe( base_deces_t & base_deces,
                        connexe_t & tableau_composantes){
      //un tableau gardant en mémoire tous les prénoms parcourus
      tableau_t all_prenoms;
      //boucle sur l'ensemble des prénoms
      //chaque prénom sert de noeud dont les branches seront parcourues
      for (auto & branche : base_deces){
            //un tableau correspondant à une composante connexe
            tableau_t set_prenoms;
            //le prénom qui servira de noeud
            std::string branche_t = branche.first;
            //la fonction remplissant le tableau set_prenoms
            connexe(base_deces, branche_t, set_prenoms, all_prenoms);
            //si le tableau n'a pas été rempli car le prénom donné à la base
            //appartient déjà à une composante connexe
            //alors il ne faut pas le placer dans le tableau général
            if (!set_prenoms.empty()){
                  tableau_composantes.push_back(set_prenoms);
            }
      }
}
//------------------------------------------------------------------------------
//fonction remplissant un tableau de prénom
//destiné à être ajouté au tableau des composantes connexes
void connexe(base_deces_t & base_deces, std::string & noeud,
            tableau_t & set_prenoms, tableau_t & all_prenoms){
      //si le prénom donné en tant que noeud existe dans la liste globale des prénoms
      if (all_prenoms.find(noeud) == all_prenoms.end()){
            //on l'ajoute dans la liste locale --> composante connexe
            set_prenoms.insert(noeud);
            //on l'ajoute dans la liste globale --> mémoire nécessaire à la fonction
            all_prenoms.insert(noeud);
      }
      for (auto & branche : base_deces[noeud]){
            //on parcours les branches non marquées et on ajoute les prénoms
            if (all_prenoms.find(branche.first) == all_prenoms.end()){
                  std::string branche_t = branche.first;
                  connexe(base_deces, branche_t, set_prenoms, all_prenoms);
            }
      }
}

//##############################################################################

//fonctions supplémentaires
//compte le nombre maximum de prénom observés chez une seule personne
void comptage(std::vector<std::string>& vect, int& n_max){
      for (std::string prenoms_lies : vect) {
            std::stringstream s1(prenoms_lies);
            int n = 0;
            while (s1>>prenoms_lies){
                  n++;
            }
            if (n > n_max) {
                  n_max = n;
            }
      }
}
