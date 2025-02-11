#include <iostream>
//#include <vector>
#include <algorithm>
#include <fstream>
#include <set>
//#include <unordered_map>
#include <unordered_set>
//#include <string>
#include <random>
//#include <chrono>

using namespace std;

// Fonction pour générer tous les binômes possibles
vector<pair<int, int> > genererBinomes(int nombreEtudiants) {
    vector<pair<int, int> > binomes;
    for (int i = 1; i <= nombreEtudiants; ++i) {
        for (int j = i + 1; j <= nombreEtudiants; ++j) {
            binomes.push_back({i, j});
        }
    }
    return binomes;
}

// Fonction pour mélanger aléatoirement un vecteur
void melanger(vector<pair<int, int> >& binomes) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(binomes.begin(), binomes.end(), default_random_engine(seed));
}

// Fonction principale
int main() {
    int nombreEtudiants;
    cout << "Entrez le nombre d'etudiants : ";
    cin >> nombreEtudiants;

    // Ajouter un étudiant fictif si le nombre est impair
    if (nombreEtudiants % 2 != 0) {
        cout << "Ajout d'un etudiant fictif pour equilibrer les binomes." << endl;
        nombreEtudiants++;
    }

    // Calcul du nombre de TP nécessaires
    int nombreTP = nombreEtudiants / 2; // Nombre de TP par semaine
    cout << "Nombre de TP necessaires par semaine : " << nombreTP << endl;

    // Le nombre de semaines est égal au nombre de TP
    int nombreSemaines = nombreTP;

    vector<char> tps;
    for (int i = 0; i < nombreTP; ++i) {
        tps.push_back('A' + i); // TP A, B, C...
    }

    vector<pair<int, int>> binomes = genererBinomes(nombreEtudiants); // Tous les binômes possibles
    vector<vector<pair<pair<int, int>, char>>> planning; // Chaque semaine associe un binôme à un TP
    set<pair<int, int>> binomesUtilisesGlobalement; // Suivi des binômes déjà utilisés dans toutes les semaines

    // Associer chaque étudiant aux TP qu'il a déjà réalisés
    unordered_map<int, unordered_set<char>> etudiantTP;

    cout << "Calcul du planning en cours..." << endl;

    while (true) {
        planning.clear();
        etudiantTP.clear();
        binomesUtilisesGlobalement.clear();
        bool tirageReussi = true;

        for (int semaine = 1; semaine <= nombreSemaines; ++semaine) {
            vector<pair<pair<int, int>, char>> semaineBinomes;
            unordered_set<int> etudiantsUtilises; // Vérifier qu'un étudiant n'est pas déjà dans un binôme cette semaine
            vector<char> tpRestants = tps;       // TP disponibles cette semaine
            melanger(binomes);

            for (const auto& binome : binomes) {
                if (tpRestants.empty()) break; // Plus de TP disponibles cette semaine

                // Vérifier que le binôme est unique globalement
                if (binomesUtilisesGlobalement.find(binome) == binomesUtilisesGlobalement.end() &&
                    etudiantsUtilises.find(binome.first) == etudiantsUtilises.end() &&
                    etudiantsUtilises.find(binome.second) == etudiantsUtilises.end()) {
                    
                    // Chercher un TP valide pour ce binôme
                    char tpAssigne = '\0';
                    for (char tp : tpRestants) {
                        if (etudiantTP[binome.first].find(tp) == etudiantTP[binome.first].end() &&
                            etudiantTP[binome.second].find(tp) == etudiantTP[binome.second].end()) {
                            tpAssigne = tp;
                            break;
                        }
                    }

                    if (tpAssigne != '\0') {
                        // Ajouter le binôme et assigner le TP
                        semaineBinomes.push_back({binome, tpAssigne});
                        etudiantsUtilises.insert(binome.first);
                        etudiantsUtilises.insert(binome.second);
                        etudiantTP[binome.first].insert(tpAssigne);
                        etudiantTP[binome.second].insert(tpAssigne);
                        tpRestants.erase(remove(tpRestants.begin(), tpRestants.end(), tpAssigne), tpRestants.end());
                        binomesUtilisesGlobalement.insert(binome); // Marquer ce binôme comme utilisé
                    }

                    if (semaineBinomes.size() == nombreTP) break;
                }
            }

            // Vérifier si le tirage est valide
            if (semaineBinomes.size() != nombreTP) {
                //cout << "Tirage invalide. Recommencement..." << endl;
                tirageReussi = false;
                break;
            }

            planning.push_back(semaineBinomes);
        }

        if (tirageReussi) break; // Sortir de la boucle si le tirage est réussi
    }

    // Afficher le planning final (avec les TP triés dans l'ordre)
    cout << endl;
    cout << "Planning final sur " << nombreSemaines << " semaines :" << endl;

    for (int itp = 0; itp < tps.size(); ++itp) {
        cout << "\t" << tps[itp];
    }
    cout << endl;

    for (size_t semaine = 0; semaine < planning.size(); ++semaine) {
        cout << "S" << semaine + 1 << ":";

        // Trier les binômes par TP pour afficher dans l'ordre des TP
        sort(planning[semaine].begin(), planning[semaine].end(),
             [](const pair<pair<int, int>, char>& a, const pair<pair<int, int>, char>& b) {
                 return a.second < b.second; // Comparer les TP (A, B, C...)
             });

        for (size_t i = 0; i < planning[semaine].size(); ++i) {
            auto binome = planning[semaine][i].first;
            //char tp = planning[semaine][i].second;
            //cout << "TP " << tp << " : Etudiants " << binome.first << " et " << binome.second << endl;
            cout << "\t" << binome.first << "-" << binome.second;
        }
        cout << endl;
    }
/*
    // Demander si l'utilisateur souhaite exporter en CSV
    char choix;
    cout << "Souhaitez-vous exporter le planning au format CSV ? (o/n) : ";
    cin >> choix;

    if (choix == 'o' || choix == 'O') {
        ofstream fichier("planning_tp.csv");
        fichier << "Semaine,TP,Etudiant 1,Etudiant 2\n";
        for (size_t semaine = 0; semaine < planning.size(); ++semaine) {
            for (size_t i = 0; i < planning[semaine].size(); ++i) {
                auto binome = planning[semaine][i].first;
                char tp = planning[semaine][i].second;
                fichier << "Semaine " << semaine + 1 << ",";
                fichier << tp << ",";
                fichier << binome.first << ",";
                fichier << binome.second << "\n";
            }
        }
        fichier.close();
        cout << "Planning exporte dans le fichier 'planning_tp.csv'." << endl;
    } else {
        cout << "Exportation annulee." << endl;
    }
*/
    return 0;
}
