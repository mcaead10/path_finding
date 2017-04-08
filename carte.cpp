//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#include <cstdio>
#include <limits>
#include <math.h>
#include <queue>
#include <sstream>
#include <stack>
#include "carte.h"


Carte::Noeud::Noeud() {
}

Carte::Arete::Arete() {
}

Carte::Carte() {
}

void Carte::ajouterNoeud(const string &nom, const Coordonnee &coor) {
    int &id = indexnoeuds[nom];
    assert(id == 0);
    id = noeuds.size();
    noeuds.push_back(Noeud());
    Noeud &noeud = noeuds[id];
    noeud.nom = nom;
    noeud.coor = coor;
}

const string &Carte::getNomNoeudPlusProche(const Coordonnee &c) const {
    double distance = INFINITY;
    int noeudPlusProche = -1;
    for (int i = 0; i < noeuds.size(); i++) {
        double d = c.distance(noeuds[i].coor);
        if (d < distance) {
            distance = d;
            noeudPlusProche = i;
        }
    }
    return noeuds[noeudPlusProche].nom;
}

void Carte::ajouterRoute(const string &nom, list<string> &route) {
    int &id = indexnoeuds[nom];
    list<string> copieRoute = route;
    id = aretes.size();
    for (int i = 0; i < copieRoute.size(); ++i) {
        aretes.push_back(Arete());
        Arete &arete = aretes[id];
        arete.depart = route.front();
        route.pop_front();
        if (!route.empty()) {
            arete.arrive = route.front();
            arete.nomRue = nom;
            int &indexnoeud1 = indexnoeuds[arete.arrive];
            int &indexnoeud2 = indexnoeuds[arete.depart];
            Noeud noeud1 = noeuds[indexnoeud1];
            Noeud noeud2 = noeuds[indexnoeud2];
            arete.distanceMetre = noeud1.coor.distance(noeud2.coor);
            vector<Arete> &temp1 = aretesortante[arete.depart];
            temp1.push_back(arete);
            vector<Arete> &temp2 = aretesortante[arete.arrive];
            temp2.push_back(arete);
            id++;
        }
    }
    route = copieRoute;
}

/////////////////////  PARTIE A /////////////////////////////
double Carte::calculerChemin(const string &nomorigine, const string &nomdestination,
                             std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) {
    resetCarte();
    int indexOrigine = indexnoeuds[nomorigine];
    noeuds[indexOrigine].distanceLaPlusCourte = 0;
    noeuds[indexOrigine].visite = true;
    Noeud noeudActuel = noeuds[indexOrigine];
    std::priority_queue<Arete> queue;
    std::priority_queue<Noeud> queueAjoute;
    remplirQueue(noeudActuel.nom, queue);
    while (!queue.empty()) {
        string nomNoeudActuel = noeudActuel.nom;
        Arete areteLaPlusCourte = queue.top();
        queue.pop();
        if (areteLaPlusCourte.distanceMetre == INFINITY) { break; }
        for (int i = 0; i < aretesortante[noeudActuel.nom].size(); ++i) {
            if (i != 0) {
                if (queue.empty()) { break; }
                areteLaPlusCourte = queue.top();
                queue.pop();
            }
            Noeud &noeudArrive = noeuds[indexnoeuds[areteLaPlusCourte.arrive]]; // constitue les deux noeuds du l'arete
            Noeud &noeudDepart = noeuds[indexnoeuds[areteLaPlusCourte.depart]]; // constitue les deux noeuds du l'arete
            string nomNoeudArrive;
            string nomNoeudParent;
            Noeud noeudArriveLocal = setSortant(noeudActuel, noeudArrive, noeudDepart, nomNoeudArrive);
            Noeud noeudParentLocal = setEntrant(noeudActuel, noeudArrive, noeudDepart, nomNoeudParent);
            double distTemp = getDistance(areteLaPlusCourte, noeudParentLocal);
            if (distTemp < noeudArriveLocal.distanceLaPlusCourte) {
                if (nomNoeudActuel == noeudArrive.nom) {
                    noeudDepart.visite = true;
                    noeudDepart.parent = nomNoeudParent;
                    noeudDepart.distanceLaPlusCourte = distTemp;
                    queueAjoute.push(noeudDepart);
                } else {
                    noeudArrive.visite = true;
                    noeudArrive.parent = nomNoeudParent;
                    noeudArrive.distanceLaPlusCourte = distTemp;
                    queueAjoute.push(noeudArrive);
                }
            }
        }
        if (!queueAjoute.empty()) {
            Noeud noeudCheminCourt = queueAjoute.top();
            noeudActuel = noeudCheminCourt;
            queueAjoute.pop();
            remplirQueue(noeudCheminCourt.nom, queue);
        }
    }
    construireChemin(nomdestination, nomorigine, out_cheminnoeuds, out_cheminroutes);
    return noeuds[indexnoeuds[nomdestination]].distanceLaPlusCourte;
}

void Carte::resetCarte() {
    for (int i = 0; i < noeuds.size(); i++) {
        noeuds[i].distanceLaPlusCourte = INFINITY;
        noeuds[i].parent = "";
        noeuds[i].visite = false;
    }
}

double Carte::getDistance(const Carte::Arete &areteLaPlusCourte, const Carte::Noeud &noeudParentLocal) const {
    return areteLaPlusCourte.distanceMetre + noeudParentLocal.distanceLaPlusCourte;
}

Carte::Noeud Carte::setEntrant(const Carte::Noeud &noeudActuel, const Carte::Noeud &noeudArrive,
                               const Carte::Noeud &noeudDepart, string &nomNoeudDepart) {
    Carte::Noeud noeudArriveLocal;
    if (noeudArrive.nom == noeudActuel.nom) {
        nomNoeudDepart = noeudArrive.nom;
        noeudArriveLocal = noeuds[indexnoeuds[nomNoeudDepart]];
    }
    else {
        nomNoeudDepart = noeudDepart.nom;
        noeudArriveLocal = noeuds[indexnoeuds[nomNoeudDepart]];
    }
    return noeudArriveLocal;
}

Carte::Noeud Carte::setSortant(const Carte::Noeud &noeudActuel, const Carte::Noeud &noeudArrive,
                               const Carte::Noeud &noeudDepart, string &nomNoeudArrive) {
    Carte::Noeud noeudArriveLocal;
    if (noeudArrive.nom == noeudActuel.nom) {
        nomNoeudArrive = noeudDepart.nom;
        noeudArriveLocal = noeuds[indexnoeuds[nomNoeudArrive]];
    }
    else {
        nomNoeudArrive = noeudArrive.nom;
        noeudArriveLocal = noeuds[indexnoeuds[nomNoeudArrive]];
    }
    return noeudArriveLocal;
}

void Carte::remplirQueue(string noeudActuel, std::priority_queue<Arete> &queue) {
    for (int j = 0; j < aretesortante[noeudActuel].size(); j++) {
        Arete &arete = aretesortante[noeudActuel][j];
        string prochaineNoeud;
        if (arete.arrive == noeudActuel) {
            prochaineNoeud = arete.depart;
        } else {
            prochaineNoeud = arete.arrive;
        }
        queue.push(arete);
    }
}

void Carte::construireChemin(const string &nomdestination, const string &nomorigine,
                             std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) {
    Noeud noeudDestination = noeuds[indexnoeuds[nomdestination]];
    while (noeudDestination.nom != nomorigine) {
        if (noeudDestination.parent == "") {
            break;
        }
        out_cheminnoeuds.push_front(noeudDestination.nom);
        for (int i = 0; i < aretesortante[noeudDestination.nom].size(); ++i) {
            if (aretesortante[noeudDestination.nom][i].arrive == noeudDestination.parent ||
                aretesortante[noeudDestination.nom][i].depart == noeudDestination.parent) {
                if (out_cheminroutes.empty()) {
                    out_cheminroutes.push_front(aretesortante[noeudDestination.nom][i].nomRue);
                } else if (out_cheminroutes.front() != aretesortante[noeudDestination.nom][i].nomRue) {
                    out_cheminroutes.push_front(aretesortante[noeudDestination.nom][i].nomRue);
                    break;
                }
            }
        }
        noeudDestination = noeuds[indexnoeuds[noeudDestination.parent]];
    }
    out_cheminnoeuds.push_front(noeudDestination.nom);
}

bool Carte::Arete::operator<(const Carte::Arete &autre) const {
    return (distanceMetre > autre.distanceMetre);
}

bool Carte::Noeud::operator<(const Carte::Noeud &autre) const {
    return (distanceLaPlusCourte > autre.distanceLaPlusCourte);
}


double Carte::calculerChemin(const Coordonnee &origine, const Coordonnee &destination,
                             std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) {
    string noeudorigine = getNomNoeudPlusProche(origine);
    string noeuddestination = getNomNoeudPlusProche(destination);
    double d = calculerChemin(noeudorigine, noeuddestination, out_cheminnoeuds, out_cheminroutes);
    d += noeuds.at(indexnoeuds.at(noeudorigine)).coor.distance(origine);
    d += noeuds.at(indexnoeuds.at(noeuddestination)).coor.distance(destination);
    return d;
}

/////////////////////  PARTIE B /////////////////////////////


void Carte::integrerReseau(const Reseau &reseau) {
    // Étape #1 :
    //   a) Pour chaque arrêt du réseau, trouvez le noeud le plus proche dans la carte.
    //   b) N'ajoutez pas l'arrêt dans la carte avant d'avoir trouvé les noeuds les plus proches pour tous les arrêts.
    //   c) Notez le noeud le plus proche dans Arret::noeudproche (vous devrez modifier reseau.h).
    for (map<int, Arret>::const_iterator iter = reseau.arrets.begin(); iter != reseau.arrets.end(); ++iter) {
        // À compléter.
    }

    // Étape #2 :
    //   a) Pour chaque arrêt 'a' du réseau, ajoutez un nouveau noeud dans la carte.
    //   b) Nommez ce nouveau noeud : "a" + numéro de noeud comme nom.
    //      Indice code: char buf[100]; sprintf(buf, "a%d", iter->first); noeud.nom=buf;
    //   c) Noter un index ou pointeur dans la variable Arret::noeudcarte (vous devrez modifier reseau.h).
    //   d) Ajouter deux segments de route reliant a.noeudcarte <--> a.noeudproche.

    for (map<int, Arret>::const_iterator iter = reseau.arrets.begin(); iter != reseau.arrets.end(); ++iter) {
        // À compléter.
    }

    // Étape #3 :
    //   a) Réfléchissez à comment représenter les horaires (avoir carte.h / Carte::Noeud)
    //   b) Pour chaque parcours d'autobus / de train / pour chaque étape
    //     i) Si nécessaire, créez une arête (arc) reliant les noeuds d'origine et de destination
    //     ii) À cette arête, ajouter l'heure de passage...

    for (unsigned int i = 0; i < reseau.parcours.size(); ++i) {
        const Parcours &parcours = reseau.parcours[i];
        for (unsigned int j = 1; j < parcours.etapes.size(); j++) {
            // À compléter
        }
    }
}

Heure Carte::calculerItineraire(Heure depart, const string &nomorigine, const string &nomdestination,
                                std::list<string> &out_cheminnoeuds,
                                std::list<string> &out_itineraireinstructions) const {
    // Enlevez ceci. C'est uniquement pour que le tout fonctionne avec l'interface graphique.
    out_cheminnoeuds.push_back(nomorigine);
    out_cheminnoeuds.push_back(nomdestination);

    Heure arrivee = depart + Heure(3600);

    out_itineraireinstructions.push_back(nomorigine);
    stringstream ssheure;
    ssheure << depart;
    string sheure;
    ssheure >> sheure;
    out_itineraireinstructions.push_back(sheure);
    out_itineraireinstructions.push_back("(vol_d'oiseau)");
    out_itineraireinstructions.push_back(nomdestination);
    out_itineraireinstructions.push_back(nomorigine);
    stringstream ssheure2;
    ssheure2 << arrivee;
    string sheure2;
    ssheure2 >> sheure2;
    out_itineraireinstructions.push_back(sheure2);
    return arrivee;
}

Heure Carte::calculerItineraire(Heure depart, const Coordonnee &origine, const Coordonnee &destination,
                                std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    string noeudorigine = getNomNoeudPlusProche(origine);
    string noeuddestination = getNomNoeudPlusProche(destination);
    depart += (int) ceil(noeuds.at(indexnoeuds.at(noeudorigine)).coor.distance(origine));
    Heure arrivee = calculerItineraire(depart, noeudorigine, noeuddestination, out_cheminnoeuds, out_cheminroutes);
    arrivee += (int) ceil(noeuds.at(indexnoeuds.at(noeuddestination)).coor.distance(destination));
    return arrivee;
}

// Vous ne devriez pas avoir à modifier l'opérteur >>

// Lire une carte.
istream &operator>>(istream &is, Carte &carte) {
    // Lire les noeuds
    while (is) {
        string nomnoeud;
        is >> nomnoeud;
        if (nomnoeud == "---") break;
        Coordonnee coor;
        char pv;
        is >> coor >> pv;
        assert(pv == ';');
        carte.ajouterNoeud(nomnoeud, coor);
    }

    // Lire les routes
    while (is) {
        string nomroute;
        is >> nomroute;
        if (nomroute == "---" || nomroute == "" || !is) break;

        char deuxpoints;
        is >> deuxpoints;
        assert(deuxpoints == ':');

        std::list<std::string> listenomsnoeuds;

        string nomnoeud;
        while (is) {
            is >> nomnoeud;
            if (nomnoeud == ";") break;
            assert(nomnoeud != ":");
            assert(nomnoeud.find(";") == string::npos);
            listenomsnoeuds.push_back(nomnoeud);
        }

        assert(nomnoeud == ";");

        carte.ajouterRoute(nomroute, listenomsnoeuds);
    }

    return is;
}
