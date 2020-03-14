/*
	Gestion de la base de personnes décédées pour en extraire les prénoms.
 */

#ifndef __base_deces_hpp__
#define __base_deces_hpp__

#include <iosfwd>
#include <string>
#include <unordered_map>
#include <array>
#include <csv/csv.h>

using base_deces_t = std::unordered_map<prenom_t,occurrences_t,
																					hash_pair,equality_prenom_t>;

void lire_base_deces(const std::string& nomfic, base_prenoms_t& bp);
using branche_t = std::pair<int, prenom_t>;

//pour la fonction sépararnt les prénoms et les associants:
using separes_t = std::pair<std::string, std::vector<std::string> >;
//fonction séparant les prénoms associés et renvoyant une paire prenom/prénoms associés
separes_t separate(std::string prenoms_str);

using deces_file_t =  io::CSVReader<2, io::trim_chars<' '>,
										 io::double_quote_escape<',','\"'> >;




#endif // __base_deces_hpp__
