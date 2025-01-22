#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

int main()
{ // Programme principal :
	// Déclaration des variables :
	int nombre_etudiants;
	int nombre_binomes;
	int nombre_tp;
	int tp;
	int seance;
	int i, j, k; // Indices pour boucles
	vector<pair<int, int>> binomes {};// Tableau dynamique de tous les binomes possibles (paire de 2 entiers)
	vector<bool> bin_deja_tire {}; // Tableau dynamique pour savoir si un binôme a déjà été tiré

	do
	{ // Demande du Nombre d'étudiants :
		cout << "Entrez le nombre d'etudiants : ";
		while (! (cin >> nombre_etudiants) )
		{ // Vérification si on n'entre pas un nombre
			cout << "Ce n'est pas un nombre !" << endl;
			cout << "Entrez le nombre d'etudiants : ";
			cin.clear();
			cin.ignore(40,'\n');
		};
		if (nombre_etudiants < 5)
			cout << "Le nombre minimal d'etudiants est 5 !" << endl;
	} 
	while (nombre_etudiants < 5);
	if ((nombre_etudiants % 2) == 1)
	{ // Correction si nombre_etudiants impair
		nombre_etudiants = nombre_etudiants + 1;
		cout << "Nombre d'etudiants impair => ajout d'un etudiant fictif." << endl;
		cout << "Nombre d'etudiants : " << nombre_etudiants << "." << endl;
	}
	
	// Calcul du nombre de TP :
	nombre_tp = nombre_etudiants / 2;
	cout << "Votre planning devra comporter " << nombre_tp << " TPs : de A a " << char(64 + nombre_tp) << "." << endl;
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
			k = k +1;
		}
	}
	
	// Rangement par ordre aléatoire :
	random_device rd;
	mt19937 g(rd());
	shuffle (binomes.begin(), binomes.end(), g);
	
	// Affichage en colonne :
	for (i = 0; i < nombre_binomes; i++)
	{
		cout << i + 1 << ":\t(" << binomes[i].first << "," << binomes[i].second << ")" << endl;
	}
	
	// Déclaration du tableau 2D planning
	pair<int,int> planning[nombre_tp][nombre_tp];
	
	// Déclaration + Initialisation d'un tableau de booléens pour savoir si un étudiant est déjà dans la séance :
	bool etudiant_seance[nombre_etudiants];
	for (i = 0; i < nombre_etudiants; i++)
	{
		etudiant_seance[i] = false;
	}
		
	// Constitution de la première seance :
	i = 0;
	tp = 0;
	seance = 0;
	planning[seance][tp] = binomes[i]; // Première paire 
	bin_deja_tire[i] = true;
	etudiant_seance[binomes[i].first - 1] = true;
	etudiant_seance[binomes[i].second - 1] = true;
	
	do
	{ // Recherche des paires suivantes :		
		do // Recherche la paire suivante qui ne contient pas un étudiant déja tiré :
			i++;
		while ((etudiant_seance[binomes[i].first - 1] == true) || (etudiant_seance[binomes[i].second - 1] == true));
		tp++;
		planning[seance][tp] = binomes[i];
		bin_deja_tire[i] = true;
		etudiant_seance[binomes[i].first - 1] = true;
		etudiant_seance[binomes[i].second - 1] = true;
	}
	while (tp < (nombre_tp -1));
		
	// Affichage final :
	cout << endl << "TP :\t";
	for (i = 1; i <= nombre_tp; i++)
	{
		cout << char(64 + i) << "\t";
	}
	cout << "CS" << endl;
	for (i = 0; i < nombre_tp; i++)
	{
		cout << "S" << i+1 << " :\t";
		int CS = 0; // Somme de contrôle
		for (j = 0; j < nombre_tp; j++) 
		{
			cout << "(" << planning[i][j].first << "," << planning[i][j].second << ")\t";
			CS = CS + planning[i][j].first + planning[i][j].second;
		}
		cout << CS << endl;
	}
			    
	return 0;
}
