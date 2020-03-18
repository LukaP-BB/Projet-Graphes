/*
	Gestion de la base de personnes décédées.
 */
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include "base_deces.hpp"
 // #include "base_prenoms.hpp"


//nouvelle approche : charger les prénoms dans un seul grand vecteur puis
//traiter ensuite ce vecteur
//entrée : fichier et vecteur
 void lire_base_deces(const std::string& nomfic, std::vector<std::string>& vect)
 {
       try {
             deces_file_t f(nomfic);
             f.read_header(io::ignore_extra_column,"prenom");
             std::string prenomstr;
             std::string p_sep, prenom;
             while (f.read_row(prenomstr)) {
                  vect.push_back(prenomstr);
            }
      } catch (const io::error::can_not_open_file& e) {
            std::cerr << e.what() << "\n";
            exit(1);
      }
}


//Composantes connexes :
//parcourir les prénoms de base_deces, remplir un tableau de tableau de prénoms
//Premier prénom de base_deces : l'ajouter lui et les prénoms associés au premier tableau
//Prénoms suivants :
      //si le prénom est dans l'un des tableaux, on ajoute tous les prénoms associés dans ce tableau
      //si le prénom n'est pas dans l'un des tableaux, on l'ajoute lui et tous ses prénoms associés


void connexes(base_deces_t & base_deces, connexe_t & tableau_composantes){
      std::cout << "Création des composantes connexes..." << '\n';
      for (auto prenom : base_deces) {
            tableau_t set_prenoms;
            if (tableau_composantes.empty()){ //s'il n'y a pas de encore de set de prénoms
                  //on met tous les prénoms de la table de hash dans un set de prénoms que l'on crée
                  set_prenoms.insert(prenom.first);

                  for (auto & prenom2 : prenom.second){
                        set_prenoms.insert(prenom2.first);
                  }
                  tableau_composantes.push_back(set_prenoms);
            }else{
                  // parcourir tous les sets du tableau_composantes
                  //si ce prénom ou un de ses prénoms associés est déja dans un de ces sets : ajouter tous les prénoms associés

                  bool found = false;
                  for (tableau_t& set : tableau_composantes){
                        // found = false;
                        //on commence par chercher le prénom du noeud courant
                        if (set.find(prenom.first) != set.end()){
                              //si on le trouve, on ajoute tous les prénoms à la branche de ce noeud
                              for (auto & prenom2 : prenom.second){
                                    set.insert(prenom2.first);
                              }
                              found = true; //et on retient qu'on l'a fait
                        }
                        //si le prénom courant n'est pas trouvé
                        //on cherche ses prénoms associés dans tous les sets
                        if (!found) {
                              // for (tableau_t& set : tableau_composantes){
                              for (auto & prenom2 : prenom.second){
                                    if (set.find(prenom2.first) != set.end()){
                                          found = true;
                                          break;
                                    }
                              }
                              //si l'un des prénoms associés est trouvé dans le set,
                              //on insère le prénom courant et tous les prénoms associés
                              if (found){
                                    set.insert(prenom.first);
                                    for (auto & prenom2 : prenom.second){
                                          set.insert(prenom2.first);
                                    }
                              }
                        }
                  }
                  // si aucun de ces prénoms n'est dans aucun de ces sets : créer un nouveau set
                  if (!found){
                        set_prenoms.insert(prenom.first);
                        std::cout << "Prénom potentiellement problématique : " << prenom.first << '\n';
                        for (auto & prenom2 : prenom.second){
                              set_prenoms.insert(prenom2.first);
                        }
                        tableau_composantes.push_back(set_prenoms);
                  }
            }
      }
}

//fonction séparant les prénoms associés et en faisant une liste
//entrée : vecteur et base_deces_t
void separate(std::vector<std::string>& vect, base_deces_t& base_deces){
      std::string nom[10]; //intialisation d'un tableau de 10 cases (7 prenoms max observés dans la base "large")
      std::string prenom_noeud, prenom_branche, p_sep;
      std::cout << "Création du graphe..." << '\n';
      for (std::string prenoms_lies : vect) {
            // std::cout << prenoms_lies << '\n';
            std::stringstream s(prenoms_lies);
            //On place les prénoms dans un tableau et on garde combien ont été enregitsrés
            int nb_prenoms = 0;
            while (s>>p_sep){
                  nom[nb_prenoms] = p_sep;
                  nb_prenoms ++;
            }
            if( nb_prenoms == 1){
                  branche_t branche;
                  base_deces.insert({nom[0], branche});
            }

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
