/*
	Projet de TP X2BI040
	2019/2020
	Les prénoms
	DIBOT Nicolas & PAVAGEAU Luka
 */

#include <cstdlib>
#include "base_deces.hpp"
#include <iostream>
using namespace std;
#include <fstream>


int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Erreur: nom de fichier csv attendu\n";
		exit(2);
	}

	//création du graphe des prénoms
	base_deces_t graphe = lire_base_deces(argv[1]);
	//création des composantes connexes
	connexe_t tableau_connexes;
	creaction_connexe(graphe, tableau_connexes);

	int choix_utilisateur;
	std::string message_utilisateur =
 	"\nBienvenue sur le programme des prénoms, que souhaitez vous faire ? \n\n"
	"1 : Afficher le nombre de prénoms uniques\n"
	"2 : Afficher le nombre de composantes connexes du graphe des prénoms\n"
	"3 : Afficher les composantes connexes du graphe des prénoms\n"
	"4 : Afficher la liste des prénoms dans la même composante connexe qu’un prénom donné au clavier\n"
	"5 : Afficher la liste des prénoms « oubliés »\n"
	"6 : Afficher la liste des prénoms associés à un prénom donné au clavier\n"
	"7 : quitter le programme\n\n"
	"Tout autre nombre réaffichera ce message\n\n";

	std::cout << message_utilisateur;

	while (true) {

		if (cin>>choix_utilisateur) {
			if (choix_utilisateur == 7){break;}
		}else{
			std::cout << "Merci d'entrer un entier compris en 1 et 7, fermeture du programme" << '\n';
			break;
		}

		switch (choix_utilisateur) {
			case 1 :
			std::cout << "il y a " << graphe.size() << " prénoms uniques" << '\n';
			break;

			case 2 :
			std::cout << "il y a " << tableau_connexes.size() << " composantes connexes" << '\n';
			break;

			case 3 :
				std::cout << "Selon la taille du fichier que vous avez entré "
				"l'affichage de ce résultat peut être extrêmement long. \n"
				"Souhaitez vous enregistrer ce résultat dans un fichier (1) ou l'afficher en console (2) ?" << '\n';
				int switch_fichier;
				std::cin >> switch_fichier;
				switch (switch_fichier) {
					case 1 :
					{
						std::cout << "Choisissez un nom pour votre fichier : " << '\n';
						std::string nom_fichier;
						std::cin >> nom_fichier;
						std::cout << "Enregistrement du fichier dans le répertoire courant" << '\n';
						std::ofstream fichier_ecrit(nom_fichier);
						fichier_ecrit << "Liste des composantes connexes" << '\n';
						int i = 1;
						for (tableau_t set : tableau_connexes){
							fichier_ecrit << "Tableau : " << i++ << '\n';
							for (std::string prenom : set){
								fichier_ecrit << "\t" << prenom << '\n';
							}
						}
					}
					break;
					case 2 :
						{
							std::cout << "Affichage dans la console" << '\n';
							int i = 1;
							for (tableau_t set : tableau_connexes){
								std::cout << "Tableau : " << i++ << '\n';
								for (std::string prenom : set){
									std::cout << "\t" << prenom << '\n';
								}
							}
						}
					break;
					default : std::cout << "Vous n'avez pas entré la bonne information, recommencez" << '\n';
				}
			break;

			case 4 : std::cout << "Voici la liste des prénoms dans la même composante connexe que le prénom demandé" << '\n';
			afficher_composantes_connexes(tableau_connexes);

			break;

			case 5 :
			{//encapsulation des variables locales avec les accolades
				tableau_t prenoms_o;
				prenoms_oublies(prenoms_o, graphe);
				if (prenoms_o.empty()){
					std::cout << "Tous les prénoms connus et 'non rares' sont présents dans votre fichier" << '\n';
				}else {
					std::cout << "Souhaitez vous enregistrer la liste des prenoms oubliés (1) ou l'afficher (2) ?" << '\n';
					// int switch_fichier;
					std::cin >> switch_fichier;
					switch (switch_fichier) {
						case 1 :
						{
							std::cout << "Choisissez un nom pour votre fichier : " << '\n';
							std::string nom_fichier;
							std::cin >> nom_fichier;
							std::cout << "Enregistrement du fichier dans le répertoire courant" << '\n';
							std::ofstream fichier_ecrit(nom_fichier);
							fichier_ecrit << "Tous ces prénoms existent mais ne sont pas présents dans le fichier donné" << '\n';
							for (std::string prenom :prenoms_o){
								fichier_ecrit << prenom << '\n';
							}
						}
						break;
						case 2 :
						std::cout << "Tous ces prénoms existent mais ne sont pas présents dans le fichier donné" << '\n';
						for (std::string prenom :prenoms_o){
							std::cout << prenom << '\n';
						}
					}
				}
			}
			break;

			case 6 :
			afficher_prenoms_assoc(graphe);
			break;

			case 7 : std::cout << "Au revoir" << '\n';
			break;

			default : std::cout <<message_utilisateur;
		}

		std::cout << "Entrez un nouveau choix d'action entre 1 et 7 " << '\n';

	}

	return 0;
}

