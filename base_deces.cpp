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
             // uint32_t annee, nombre;
             while (f.read_row(prenomstr)) {

                  vect.push_back(prenomstr);
                  // std::cout << prenomstr << '\n';

                  //  //séparation des prénoms
                  //  std::vector<std::string> liste_prenoms;
                  //  // liste_prenoms.reserve(10); //ne fait pas gagner de performance
                  //  separate(prenomstr, liste_prenoms);
                  //
                  //  prenom_add(liste_prenoms, bp);
                  //  for (std::string prenom : liste_prenoms) {
                  //       // std::cout << prenom << '\t';
                  // }
            }
      } catch (const io::error::can_not_open_file& e) {
            std::cerr << e.what() << "\n";
            exit(1);
      }
}



//fonction séparant les prénoms associés et en faisant une liste
//entrée : vecteur et base_deces_t
void separate(std::vector<std::string>& vect, base_deces_t& base_deces){
      std::string nom[10]; //intialisation d'un tableau de 10 cases (7 prenoms max observés dans la base "large")
      std::string prenom_noeud, prenom_branche, p_sep;
      branche_t branche;
      int errors = 0;

      for (std::string prenoms_lies : vect) {

            // std::cout << prenoms_lies << '\n';
            std::stringstream s(prenoms_lies);
            // std::cout << prenoms_lies << '\n';

            try {
                  //On place les prénoms dans un tableau et on garde combien ont été enregitsrés
                  int nb_prenoms = 0;
                  while (s>>p_sep){
                        // std::cout << ". " << p_sep << ". " << '\n';
                        if (p_sep != "") {
                              nom[nb_prenoms] = p_sep;
                              nb_prenoms ++;
                        }
                  }

                  for (int i = nb_prenoms-1; 0 <= i; i--) {
                        prenom_noeud = nom[i];
                        auto iter = base_deces.find(prenom_noeud);

                        // std::cout << prenom_noeud << '\n';

                        if (iter != base_deces.end()) { // Le prénom est déjà dans la base ?
                              //on ajoute les prénoms associés à la table de hash
                              for (int i = nb_prenoms-1; 0 <= i; i--) {
                                    prenom_branche = nom[i];
                                    // std::cout << "\t" << nom[i] <<'\n';
                                    if (prenom_noeud != prenom_branche){
                                          branche = base_deces[prenom_noeud];
                                          branche[prenom_branche] ++;
                                          base_deces[prenom_noeud] = branche;
                                    }
                              }
                        } else { // Le prénom n'est pas dans la base ?
                              // // => On l'y ajoute
                              base_deces[prenom_noeud];
                              for (int i = nb_prenoms-1; 0 <= i; i--) {
                                    prenom_branche = nom[i];
                                    // std::cout << "\t" << prenom_branche <<'\n';
                                    if (prenom_branche != prenom_noeud){
                                          branche = base_deces[prenom_noeud];
                                          branche[prenom_branche] ++;
                                          base_deces[prenom_noeud] = branche;
                                    }
                              }
                        }
                  }
            }catch(...){
                  errors ++;
            }
      }
      std::cout << "Erreurs : " << errors << '\n';
}


//compte le nombre maximum de prénom observés chez une seule personne
void comptage(std::vector<std::string>& vect, int& n_max){
      for (std::string prenoms_lies : vect) {
            std::stringstream s1(prenoms_lies);
            int n = 0;
            while (s1>>prenoms_lies){
                  // std::cout << prenoms_lies << n << '\n';
                  n++;
            }
            if (n > n_max) {
                  n_max = n;
            }

      }
}
