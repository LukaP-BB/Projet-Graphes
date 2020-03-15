/*
	Gestion de la base de personnes décédées.
 */
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include "base_deces.hpp"
 // #include "base_prenoms.hpp"

 void lire_base_deces(const std::string& nomfic, base_deces_t& bp)
 {
       try {
             deces_file_t f(nomfic);
             f.read_header(io::ignore_extra_column,"prenom");
             std::string prenomstr;
             std::string p_sep, prenom;
             // uint32_t annee, nombre;
             while (f.read_row(prenomstr)) {

                   //séparation des prénoms
                   std::vector<std::string> liste_prenoms;
                   separate(prenomstr, liste_prenoms);

                   prenom_add(liste_prenoms, bp);
                   for (std::string prenom : liste_prenoms) {
                        // std::cout << prenom << '\t';
                  }
            }
      } catch (const io::error::can_not_open_file& e) {
            std::cerr << e.what() << "\n";
            exit(1);
      }
}

//fonction séparant les prénoms associés et en faisant une liste
void separate(std::string prenoms_str, auto& liste_p){
      std::stringstream s(prenoms_str);
      std::string p_sep;
      while (s>>p_sep){
            liste_p.push_back(p_sep);
      }
}

//fonction prenant la liste de prénoms associés,
//recherchant chacun de ces prénoms dans la base_deces
//et ajoutant les autres prénoms à la table de hash associée
void prenom_add(std::vector<std::string> liste_prenoms, base_deces_t& base_deces){
      for (std::string prenom_noeud : liste_prenoms) {
            auto iter = base_deces.find(prenom_noeud);
            branche_t branche;
            if (iter != base_deces.end()) { // Le prénom est déjà dans la base ?
                  //on ajoute les prénoms associés à la table de hash
                  for (std::string prenom_branche : liste_prenoms) {
                        if (prenom_noeud != prenom_branche){
                              branche = base_deces[prenom_noeud];
                              branche[prenom_branche] ++;
                              base_deces[prenom_noeud] = branche;
                        }
                  }
            } else { // Le prénom n'est pas dans la base ?
            // // => On l'y ajoute
                  base_deces[prenom_noeud];
                  for (std::string prenom_branche : liste_prenoms) {
                        if (prenom_branche != prenom_noeud){
                              branche = base_deces[prenom_noeud];
                              branche[prenom_branche] ++;
                              base_deces[prenom_noeud] = branche;
                        }
                  }
            }
      }
}