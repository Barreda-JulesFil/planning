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
	
	// Affichage en-tête :
	cout << endl << "TP\t";
	for (i = 1; i <= nombre_tp; i++)
		cout << char(64 + i) << "\t";
	cout << endl;
	
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
	
	// Tri aléatoire des binômes :
	random_device rd;
	mt19937 g(rd());
	shuffle (binomes.begin(), binomes.end(), g);
	
	// Déclaration du tableau 2D planning
	pair<int,int> planning[nombre_tp][nombre_tp];
	
	// Déclaration + Initialisation d'un tableau de booléens pour savoir si un étudiant est déjà dans la séance :
	bool etudiant_seance[nombre_etudiants];
	for (i = 0; i < nombre_etudiants; i++)
		etudiant_seance[i] = false;
		
	// Constitution de la première seance :
	i = 0;
	tp = 0;
	seance = 0;
	planning[seance][tp] = binomes[i]; // Première binôme 
	bin_deja_tire[i] = true;
	etudiant_seance[binomes[i].first - 1] = true;
	etudiant_seance[binomes[i].second - 1] = true;
	
	do
	{ // Recherche des binômes suivants :		
		do // Recherche le binôme suivant qui ne contient pas un étudiant déja tiré :
			i++;
		while ((etudiant_seance[binomes[i].first - 1] == true) || (etudiant_seance[binomes[i].second - 1] == true));
		tp++;
		planning[seance][tp] = binomes[i];
		bin_deja_tire[i] = true;
		etudiant_seance[binomes[i].first - 1] = true;
		etudiant_seance[binomes[i].second - 1] = true;
	}
	while (tp < (nombre_tp -1));
	
	// Affichage d'une séance :
	cout << "S" << seance + 1 << "\t";
	for (j = 0; j < nombre_tp; j++)
		cout << "(" << planning[seance][j].first << "," << planning[seance][j].second << ")\t";
	cout << endl;
	
	// Déclaration d'un tableau de booléens pour savoir si un étudiant est déjà dans une colonne TP :
	bool etudiant_tp[nombre_etudiants];
	
	do
	{ //Constitution des séances suivantes :
		seance++;
		tp = 0;
		// RAZ des tableaux
		for (i = 0; i < nombre_etudiants; i++)
		{
			etudiant_tp[i] = false;	// RAZ tableau etudiant_tp[]
			etudiant_seance[i] = false; // RAZ tableau etudiant_seance[]
		}
		
		// Recherche des étudiants ayant déjà fait ce TP dans les séances précédentes
		for (i = 0; i < seance; i++)
		{
			etudiant_tp[planning[i][tp].first - 1] = true;
			etudiant_tp[planning[i][tp].second - 1] = true;
		}
		
		i = 0;
		// Recherche du premier binôme pas déjà tiré et n'ayant pas fait ce TP
		while ((bin_deja_tire[i] == true) || (etudiant_tp[binomes[i].first - 1] == true) || (etudiant_tp[binomes[i].second - 1] == true))
		{
			i++;
		}
		// Binôme trouvé :
		planning[seance][tp] = binomes[i];
		bin_deja_tire[i] = true;
		etudiant_seance[binomes[i].first - 1] = true;
		etudiant_seance[binomes[i].second - 1] = true;
		
		do
		{ // TP suivants :
			tp++;
			
			for (i = 0; i < nombre_etudiants; i++)
				etudiant_tp[i] = false;	// RAZ tableau etudiant_tp[]
			
			// Recherche des étudiants ayant déjà fait ce TP dans les séances précédentes
			for (i = 0; i < seance; i++)
			{
				etudiant_tp[planning[i][tp].first - 1] = true;
				etudiant_tp[planning[i][tp].second - 1] = true;
			}
			
			i = 0;
			// Recherche du premier binôme pas déjà tiré et n'ayant pas fait ce TP et dont un des éléments n'est pas déja sur la séance
			while ((bin_deja_tire[i] == true) ||
				   (etudiant_tp[binomes[i].first - 1] == true) || (etudiant_tp[binomes[i].second -1] == true) ||
				   (etudiant_seance[binomes[i].first - 1] == true) || (etudiant_seance[binomes[i].second - 1] == true))
			{
				i++;
				if (i == nombre_binomes) 
				{
					i = 0;
					//break;
				}
			}
			
			// Binôme trouvé :
			planning[seance][tp] = binomes[i];
			bin_deja_tire[i] = true;
			etudiant_seance[binomes[i].first - 1] = true;
			etudiant_seance[binomes[i].second - 1] = true;
		}
		while (tp < (nombre_tp - 1));
		
		// Affichage d'une séance :
		cout << "S" << seance + 1 << "\t";
		for (j = 0; j < nombre_tp; j++)
			cout << "(" << planning[seance][j].first << "," << planning[seance][j].second << ")\t";
		cout << endl;
	}
	while (seance < (nombre_tp - 1));
			    
	return 0;
}
