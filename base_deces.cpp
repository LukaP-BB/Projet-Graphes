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


//composantes connexes approche récursive :
//parcours de la base et marquage des prénoms déjà parcourus
void connexe(base_deces_t & base_deces, std::string & noeud, tableau_t & set_prenoms){
      //on marque le sommet du graphe donné
      if (set_prenoms.find(noeud) == set_prenoms.end()){
            std::cout << "Ajoute : " << noeud << '\n';
            set_prenoms.insert(noeud);
      }
      // std::cout << "noeud : " << noeud << '\n';
      for (auto & branche : base_deces[noeud]){
            // std::cout << branche.first << '\n';
            //on parcours les branches non marquées et on ajoute les prénoms
            if (set_prenoms.find(branche.first) == set_prenoms.end()){
                  std::string branche_t = branche.first;
                  std::cout << "explore : " << branche_t << '\n';
                  connexe(base_deces, branche_t, set_prenoms);
            }
      }
      std::cout << "efface : " << noeud << '\n';
      base_deces.erase(noeud);
}

void creaction_connexe(base_deces_t & base_deces, connexe_t & tableau_composantes){
      //base2 sera détruite au fur et à mesure de la création du tableau des composantes connexes
      base_deces_t base2 = base_deces;
      for (auto & branche : base2){
            tableau_t set_prenoms;
            std::string branche_t = branche.first;
            // std::cout << "/* message */" << branche_t << '\n';
            connexe(base2, branche_t, set_prenoms);
            tableau_composantes.push_back(set_prenoms);
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
            if (tableau_composantes.empty()){ //s'il n'y a pas de encore de set de prénoms
                  //on met tous les prénoms de la table de hash dans un set de prénoms que l'on crée
                  tableau_t set_prenoms;
                  set_prenoms.insert(prenom.first);

                  for (auto & prenom2 : prenom.second){
                        set_prenoms.insert(prenom2.first);
                  }
                  tableau_composantes.push_back(set_prenoms);
            }else{
                  bool found = false;
                  // parcourir tous les sets du tableau_composantes
                  //si ce prénom ou un de ses prénoms associés est déja dans un de ces sets : ajouter tous les prénoms associés

                  for (tableau_t& set : tableau_composantes){
                        //on commence par chercher le prénom du noeud courant
                        if (set.find(prenom.first) != set.end()){
                              //si on le trouve, on ajoute tous les prénoms à la branche de ce noeud
                              for (auto & prenom2 : prenom.second){
                                    set.insert(prenom2.first);
                              }
                              found = true; //et on retient qu'on l'a fait
                        }

                        //si le prénom courant n'est pas trouvé, on cherche dans ses prénoms associés
                        if (!found) {
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
                                          found = true;
                                    }
                              }
                        }
                  }
                  // si aucun de ces prénoms n'est dans aucun de ces sets : créer un nouveau set
                  if (!found){
                        tableau_t set_prenoms;
                        set_prenoms.insert(prenom.first);
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
