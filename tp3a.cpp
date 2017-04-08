//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

/*
    Instructions :
     1. En principe, vous n'avez pas à modifier ce fichier.
     2. Donc, pas besoin de l'imprimer.
     3. Les lignes débutants par # sont là pour vous aider au développement.
     4. Il est important de ne pas enlever les '#', car les lignes débutant par #
        seront ignorées par le valideur automatique.
     5. Par exemple, il est possible de valider le noeud le plus proche des coordonnées.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <math.h>
#include "carte.h"

using namespace std;

void tp3a( Carte &carte, istream &isrequetes) {
    while (isrequetes && !isrequetes.eof()) {
        isrequetes >> std::ws; // provoquera un échec si fin du fichier
        if (!isrequetes) break; // échec lecture

        // Lecture de l'heure.
        string heure; // L'heure est uniquement lue car elle fait partie du format de fichier.
        // Pour la partie A, l'heure est tout simplement ignorée.
        //if (&isrequetes == &cin) cout << "#Heure : ";
        isrequetes >> heure;
        if (!isrequetes) break;

        // Lecture de la coordonnée d'origine.
        Coordonnee origine;
        //if (&isrequetes == &cin) cout << "#Origine : ";
        isrequetes >> origine;
        if (!isrequetes) break;
        string noeudorigine = carte.getNomNoeudPlusProche(origine);
        // Cette ligne vous permet de valider le noeud le plus proche trouvé par la fonction Carte::getNomNoeudPlusProche.
        //cout << "#Noeud le plus près de l'origine dans la carte : " << noeudorigine << endl;

        // Lecture de la coordonnée de destination.
        Coordonnee destination;
        //if (&isrequetes == &cin) cout << "#Destination : ";
        isrequetes >> destination;
        if (!isrequetes) break;
        //string noeuddestination = carte.getNomNoeudPlusProche(destination);
        //cout << "#Noeud le plus près de la destination dans la carte : " << noeuddestination << endl;


        // Premier calcul de noeud à noeud dans la carte uniquement.
        // Ce résultat sera ignoré par le valideur. Il est uniquement à vous aider à valider
        // le chemin calculé par votre algorithme.
        //std::list<string> cheminnoeuds,    // liste des noms de noeuds sur le chemin trouvé
        //        cheminnomroutes; // liste des nom de route sur le chemin trouvé
        //cout << "#Chemin de " << noeudorigine << " à " << noeuddestination << " :" << endl;
        //double longueur1 = carte.calculerChemin(noeudorigine, noeuddestination, cheminnoeuds, cheminnomroutes);
        //cout << "#";
        //for (std::list<string>::iterator iter = cheminnoeuds.begin(); iter != cheminnoeuds.end(); ++iter)
        //    cout << *iter << " ";
        //cout << endl;
        //cout << "#";
        //for (std::list<string>::iterator iter = cheminnomroutes.begin(); iter != cheminnomroutes.end(); ++iter)
        //    cout << *iter << " ";
        //cout << endl;
        //cout << "#" << round(longueur1) << " m" << endl;

        // Deuxième calcul de la coordonnée d'origine à la coordonnée de destination.
        // Il s'agit du résultat qui sera validé par le valideur. Il n'y a donc pas de # devant le chemin et la distance
        // Ce calcul doit intégrer la distance pour connecter les noeuds d'origine et de destination
        // aux deux noeuds les plus près dans la carte.
        std::list<string> cheminnoeuds2, cheminnomroutes2;
        //cout << "#Chemin de " << origine << " à " << destination << " :" << endl;
        double longueur2 = carte.calculerChemin(origine, destination, cheminnoeuds2, cheminnomroutes2);
        for (std::list<string>::iterator iter = cheminnoeuds2.begin(); iter != cheminnoeuds2.end(); ++iter)
            cout << *iter << " ";
        cout << endl;
        for (std::list<string>::iterator iter = cheminnomroutes2.begin(); iter != cheminnomroutes2.end(); ++iter)
            cout << *iter << " ";
        cout << endl;
        cout << round(longueur2) << " m" << endl;
    }
}

int main(int argc, const char **argv) {
    if (argc <= 1) {
        cout << "Syntaxe: ./tp3a carte.txt [requetes]" << endl;
        return 1;
    }

    // Lecture de la carte.
    Carte carte;
    {
        ifstream fichiercarte(argv[1]);
        if (fichiercarte.fail()) {
            cout << "Erreur ouverture du fichier : " << argv[1] << endl;
            return 1;
        }
        fichiercarte >> carte;
    }

    if (argc > 2) {
        // Lecture des requêtes depuis un fichier.
        ifstream isrequetes(argv[2]);
        if (isrequetes.fail()) {
            cout << "Erreur ouverture du fichier : " << argv[2] << endl;
            return 1;
        }
        tp3a(carte, isrequetes);
    } else {
        // Lecture des requêtes depuis l'entrée standard.
        tp3a(carte, cin);
    }
}
