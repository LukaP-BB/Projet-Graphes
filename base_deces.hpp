/*
	Projet de TP X2BI040
	2019/2020
	Les prénoms
	DIBOT Nicolas & PAVAGEAU Luka

	Gestion de la base de personnes décédées pour en extraire les prénoms.
 */

#ifndef __base_deces_hpp__
#define __base_deces_hpp__

#include <fstream>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <array>
#include <queue>
#include<unordered_set>
#include <csv/csv.h>
#include "base_prenoms.hpp"

using deces_file_t =  io::CSVReader<1, io::trim_chars<' '>,io::double_quote_escape<',','\"'> >;

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
base_deces_t lire_base_deces(const std::string& nomfic);
//fonction séparant les prénoms associés et renvoyant une paire prenom/prénoms associés
void separate(std::vector<std::string>& vect, base_deces_t& base_deces);
//fonction insérant ces prénoms dans la base
// void prenom_add(std::vector<std::string> liste_prenoms, base_deces_t& base_deces);

//fonction remplissant le tableau de sets de prénoms
void creaction_connexe(base_deces_t & base_deces, connexe_t & tableau_composantes);

//fonction remplissant un set de prénoms
void connexe(base_deces_t & base_deces, std::string & graphe, tableau_t & set_prenoms, tableau_t & all_prenoms);

//fonction affichant les prénoms oubliés
void prenoms_oublies ( tableau_t& prenoms_oublies, base_deces_t& graphe);

//fonction utilisée lors des tests préliminaires permettant d'afficher le graphe
void afficher_graphe(base_deces_t & bp);

//fonction cherchant un prénom dans la base
//et renvoyant la liste triée des prénoms associés par fréquence décroissante
void afficher_prenoms_assoc(base_deces_t & graphe);

//fonction transformant une chaine de caractères en majuscules
void UPPER(std::string& prenom);

//fonction permettant d'afficher tous les prénoms
//faisant partie de la même composante connexe qu'un prénom demandé
//et proposant d'enregistrer le résultat dans un fichier
void afficher_composantes_connexes(connexe_t tableau_connexes);

//parcours en largeur du graphe
void least_amount(base_deces_t & graphe, connexe_t tableau_composantes, std::string& prenom1, std::string& prenom2);


#endif // __base_deces_hpp__
