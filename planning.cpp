#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{ // Programme principal :
	// Déclaration des variables :
	int nombre_etudiants;
	int nombre_binomes;
	int nombre_tp;
	int tirage;
	int i, j, k; // Indices pour boucles
	vector<pair<int, int>> binomes {};// Tableau dynamique de tous les binomes possibles (paire de 2 entiers)
	vector<bool> bin_deja_tire {}; // Tableau dynamique pour savoir si un binôme a déjà été tiré
	    
	srand(time(0));	// Initialisation du générateur de nombres aléatoires
	    
	// Demande du Nombre d'étudiants :
	cout << "Entrez le nombre d'etudiants : ";
	cin >> nombre_etudiants;
	if ((nombre_etudiants % 2) == 1) // Correction si nombre_etudiants impair
	{
		nombre_etudiants = nombre_etudiants + 1;
		cout << "Nombre d'etudiants impair => ajout d'un etudiant fictif." << endl;
		cout << "Nombre d'etudiants : " << nombre_etudiants << "." << endl;
	}
	// Calcul du nombre de TP :
	nombre_tp = nombre_etudiants / 2;
	cout << "Vous devez avoir " << nombre_tp << " TPs : de A a " << char(64 + nombre_tp) << "." << endl;
	// Calcul du nombre de binomes uniques
	nombre_binomes = (nombre_etudiants * (nombre_etudiants - 1)) / 2;
	cout << "Il existe " << nombre_binomes << " binomes uniques." << endl;
	
	// Génération des binômes uniques
	k = 0;
	for (i = 1; i <= nombre_etudiants; i++)
	{
		for (j = i + 1; j <= nombre_etudiants; j++)
		{
			binomes.emplace_back(i, j);
			bin_deja_tire.push_back(false);
			//cout << k + 1 << ":\t(" << binomes[k].first << "," << binomes[k].second << ")" << endl;
			k = k +1;
		}
	}
	
	// Déclaration du tableau 2D planning
	pair<int,int> planning[nombre_tp][nombre_tp];
	
	// Constitution de la première seance
	tirage = rand() % nombre_binomes; // Nombre aleatoire entre 0 et nombre_binomes-1
	planning[0][0] = binomes[tirage];
	bin_deja_tire[tirage] = true;
	
	// Affichage
	cout << endl << "TP :\t";
	for (i = 1; i <= nombre_tp; i++) cout << char(64 + i) << "\t";
	cout << endl;
	for (i = 0; i < nombre_tp; i++)
	{
		cout << "S" << i+1 << " :\t";
		for (j = 0; j < nombre_tp; j++) cout << "(" << planning[i][j].first << "," << planning[i][j].second << ")\t";
		cout << endl;
	}
		    
	return 0;
}
