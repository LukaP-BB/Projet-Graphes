/*
	Gestion de la base de personnes décédées pour en extraire les prénoms.
 */

#ifndef __base_deces_hpp__
#define __base_deces_hpp__

#include <iosfwd>
#include <string>
#include <unordered_map>
#include <array>
#include<unordered_set>
#include <csv/csv.h>

using deces_file_t =  io::CSVReader<1, io::trim_chars<' '>,io::double_quote_escape<',','\"'> >;

enum class sexe_t {
   masculin = 1,
	 feminin = 2
};

std::ostream& operator<<(std::ostream& os, sexe_t s);

using prenom_t = std::pair<std::string, sexe_t>;
std::ostream& operator<<(std::ostream& os, const prenom_t& p);

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		return hash1;
	}
};

struct equality_prenom_t {
	bool operator()(const prenom_t& p1, const prenom_t& p2) const {
		return p1.first == p2.first && p1.second == p2.second;
	}
};

//Une table qui contiendra des prénoms et un poids associé
using branche_t = std::unordered_map<std::string,int>;
//Une table qui contiendra pour chaque prénom, la table des prénoms associés branche_t
using base_deces_t = std::unordered_map<std::string,branche_t>;

//utilisation de set pour avoir l'équivalent d'une unordered_map ne contenant que des keys
using tableau_t = std::unordered_set<std::string>; //3 fois plus rapide qu'un set normal dans le cas présent
//le tableau des composantes connexes
using connexe_t = std::vector<tableau_t>;

//compte le nombre de prénom maximal observé
void comptage(std::vector<std::string>& vect, int& n_max);

//fonction lisant le fichier et englobant les autres fonctions
void lire_base_deces(const std::string& nomfic, std::vector<std::string>& vect);
//fonction séparant les prénoms associés et renvoyant une paire prenom/prénoms associés
void separate(std::vector<std::string>& vect, base_deces_t& base_deces);
//fonction insérant ces prénoms dans la base
// void prenom_add(std::vector<std::string> liste_prenoms, base_deces_t& base_deces);


void connexe(base_deces_t & base_deces, std::string & graphe, tableau_t & set_prenoms);
void creaction_connexe(base_deces_t & base_deces, connexe_t & tableau_composantes);



//fonction calculant les composantes connexes
void connexes(base_deces_t& base_deces, connexe_t & tableau_composantes);

#endif // __base_deces_hpp__
