#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <set>

using namespace std;

// Générateur aléatoire global (initialisé une seule fois)
random_device rd;
mt19937_64 g(rd());

template <typename T>
void melanger_aleatoirement(vector<T>& vec) {
    shuffle(vec.begin(), vec.end(), g); // Utilisation du générateur global
}

// Fonction pour normaliser un binôme (ordre indifférent)
pair<int, int> normaliser_binome(int a, int b) {
    return (a < b) ? make_pair(a, b) : make_pair(b, a);
}

vector<vector<pair<char, pair<int, int>>>> generer_planning_tp(int nombre_etudiants) {
    if (nombre_etudiants % 2 != 0) {
        nombre_etudiants++;
        cout << "Nombre d'étudiants ajusté à " << nombre_etudiants << " (ajout d'un étudiant fictif)." << endl;
    }

    int nombre_tp = nombre_etudiants / 2;
    vector<char> lettres_tp(nombre_tp);
    iota(lettres_tp.begin(), lettres_tp.end(), 'A');

    vector<vector<pair<char, pair<int, int>>>> planning(nombre_tp);
    set<pair<int, int>> binomes_formes; // Ensemble pour stocker les binômes déjà formés

    vector<int> etudiants(nombre_etudiants);
    iota(etudiants.begin(), etudiants.end(), 1);

    // Mélange des étudiants UNE SEULE FOIS au début ! (CORRECTION CRUCIALE)
    melanger_aleatoirement(etudiants);

    for (int semaine = 0; semaine < nombre_tp; ++semaine) {
        vector<pair<int, int>> binomes_semaine;

        // Algorithme de la table de mariage (APPLIQUÉ APRÈS LE MÉLANGE DES ÉTUDIANTS)
        for (int i = 0; i < nombre_etudiants / 2; ++i) {
            binomes_semaine.push_back({etudiants[i], etudiants[nombre_etudiants - 1 - i]});
        }

        // Vérification et mélange des binômes
        bool binome_ok = false;
        int tentatives = 0;
        const int MAX_TENTATIVES = 1000;

        while (!binome_ok && tentatives < MAX_TENTATIVES) {
            melanger_aleatoirement(binomes_semaine);
            binome_ok = true;
            for (const auto& binome : binomes_semaine) {
                if (binomes_formes.count(normaliser_binome(binome.first, binome.second)) > 0) {
                    binome_ok = false;
                    break;
                }
            }
            tentatives++;
        }

        if (!binome_ok && tentatives >= MAX_TENTATIVES) {
            cerr << "Erreur: Impossible de générer un planning sans binômes répétés après " << MAX_TENTATIVES << " tentatives. Augmentez MAX_TENTATIVES ou réduisez le nombre d'étudiants." << endl;
            return {}; // Retourne un planning vide en cas d'erreur
        }

        for (const auto& binome : binomes_semaine) {
            binomes_formes.insert(normaliser_binome(binome.first, binome.second));
        }

        for (int i = 0; i < nombre_tp; ++i) {
            planning[semaine].push_back({lettres_tp[i], binomes_semaine[i]});
        }

        if (nombre_etudiants > 2) {
            int temp = etudiants[1];
            for (int i = 1; i < nombre_etudiants - 1; ++i) {
                etudiants[i] = etudiants[i + 1];
            }
            etudiants[nombre_etudiants - 1] = temp;
        }
    }
    return planning;
}

void afficher_planning(const vector<vector<pair<char, pair<int, int>>>>& planning) {
    if (planning.empty()) {
        cout << "Aucun planning à afficher (nombre d'étudiants insuffisant ou erreur lors de la génération).\n";
        return;
    }
     // Vider le buffer avant le PREMIER cin.get() (et avant la boucle)
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    for (size_t semaine = 0; semaine < planning.size(); ++semaine) {
        cout << "\nSemaine " << semaine + 1 << ":" << endl;
        for (const auto& affectation : planning[semaine]) {
            cout << "TP " << affectation.first << ": Binôme (" << affectation.second.first << ", " << affectation.second.second << ")" << endl;
        }
        // Condition MODIFIÉE pour ne PAS afficher la pause après la dernière semaine
        if (semaine < planning.size() - 1) { // Vérifie si ce n'est PAS la dernière semaine
            cout << "\nAppuyez sur Entrée pour afficher la semaine suivante...";
            cin.get(); // Pause simple
        }
    }
}

int main() {
    int nombre_etudiants;
    char reponse;

    cout << "Nombre d'étudiants : "; // Demande le nombre d'étudiants UNE SEULE FOIS
    cin >> nombre_etudiants;

    if (nombre_etudiants <= 1) {
        cout << "Nombre d'étudiants insuffisant pour former des binômes.\n";
        return 1;
    }

    do { // Boucle do...while pour recommencer le tirage
        auto planning = generer_planning_tp(nombre_etudiants); // Tirage DANS la boucle
        afficher_planning(planning);

        cout << "\nLe tirage convient-il ? (O/N) : ";
        cin >> reponse;

    } while (reponse == 'N' || reponse == 'n'); // Recommence si la réponse est N ou n

    cout << "Tirage validé. Fin du programme.\n";

    return 0;
}
