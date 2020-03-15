/*
	Gestion de la base de personnes décédées.
 */
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include "base_deces.hpp"
 // #include "base_prenoms.hpp"

 // using namespace std;

void lire_base_deces(const std::string& nomfic, base_deces_t& bp)
{
	try {
		deces_file_t f(nomfic);
		f.read_header(io::ignore_extra_column,"prenom","sexe");
		std::string prenomstr;
		int sexe;
		std::string p_sep, prenom;
		// uint32_t annee, nombre;
		while (f.read_row(prenomstr, sexe)) {
      int sexe_a = sexe;

			//séparation des prénoms
      std::vector<prenom_t> liste_prenoms;
      separate(prenomstr, liste_prenoms, sexe_a);
      std::cout << "Liste de prénoms créée" << '\n';

      prenom_add(liste_prenoms, bp);
      for (prenom_t prenom : liste_prenoms) {
        std::cout << prenom << '\n';
      }
			// stringstream s(prenomstr);
      //
			// while (s>>p_sep){
      //
			// 	auto prenom = prenom_t{p_sep,sexe_t(sexe)};
			// 	const auto& iter = bp.find(prenom);
			// 	if (iter != bp.end()) { // Le prénom est déjà dans la base ?
      //
			// 		// bp[prenom];
			// 		// => on ajoute l'année courante et le nombre d'occurrences
			// 	// 	++(iter->second)[annee-1900];
			// 	} else { // Le prénom n'est pas dans la base ?
			// 	// // => On l'y ajoute
			// 		bp[prenom];
			// 	}
			// }


		}


	} catch (const io::error::can_not_open_file& e) {
		std::cerr << e.what() << "\n";
		exit(1);
	}
}


//fonction séparant les prénoms associés et en faisant une liste
void separate(std::string prenoms_str, auto& liste_p, int sexe){
  int sexe_a = sexe;
	std::stringstream s(prenoms_str);
	std::string p_sep;
	while (s>>p_sep){
    prenom_t p_sepa = prenom_t{p_sep,sexe_t(sexe_a)};
    liste_p.push_back(p_sepa);
	}
}

// TODO
//fonction prenant la liste de prénoms associés,
//recherchant chacun de ces prénoms dans la base_deces
//et ajoutant les autres prénoms à la table de hash associée
void prenom_add(std::vector<prenom_t> liste_prenoms, base_deces_t& base_deces){
      for (prenom_t prenom : liste_prenoms) {
            auto iter = base_deces.find(prenom);

        	if (iter != base_deces.end()) { // Le prénom est déjà dans la base ?
                  //on ajoute les prénoms associés à la table de hash
                  for (prenom_t prenom_A : liste_prenoms) {
                        if (prenom != prenom_A){
                              branche_t branche;
                              branche[prenom_A] = 0;
                              base_deces[prenom] = branche;
                        }
                  }
            } else { // Le prénom n'est pas dans la base ?
                  // // => On l'y ajoute
                  // prenom_t iter = iter;
                  base_deces[prenom];

                  for (prenom_t prenom_A : liste_prenoms) {
                        if (prenom != prenom_A){
                              branche_t branche;
                              branche[prenom_A] = 0;
                              base_deces[prenom] = branche;
                        }
                  }
            // std::cout << iter << '\n';
            }

      }
}