#ifndef JOUEUR_H
#define JOUEUR_H

/**
 * \file joueur.h
 * \brief Structure Joueur et liste chainée circulaire ListeJoueurs.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include "grille.h"

/**
 * \struct Joueur
 * \brief Représente un joueur par l'agrégation d'un identifiant et d'une stratégie.
 *
 * Un joueur est caractérisé par son identifiant et sa stratégie.
 * S'il a une stratégie non interactive, c'est que c'est un joueur automatisé
 * (un ia du jeu).
 */
typedef struct Joueur {
	int id;        /*!< Identifiant du joueur. */
	void (*place)(struct Joueur*, Grille*, int* x, int* y); /*!< Pointeur vers la fonction de stratégie. */
} Joueur;

/**
 * \struct ListeJoueurs
 * \brief Élément d'une liste chainée circulaire de joueurs.
 *
 * Chaque élément contient un poiveur vers le joueur associé et un pointeur vers
 * l'élément suivant de la liste.
 * La liste est circulaire et il n'y a pas de fin. L'avantage de cette structure
 * est de représenter la liste circulaire des joueurs qui doivent joueurs leurs
 * tours.
 * Cette structure est particulièrement adaptée pour l'ajour dynamique de
 * joueurs en pleine partie.
 *
 * La fonction ::joueurs_creer_liste permet d'ajouter un jouer après un élément
 * et permet donc de faire jouer le nouveau joueur au tour suivant.
 */
typedef struct ListeJoueurs {
	Joueur* joueur;                 /*!< Pointeur vers le joueur. */
	struct ListeJoueurs* suivant;   /*!< Pointeur vers l'élément suivant. */
} ListeJoueurs;

ListeJoueurs* joueurs_creer_liste(Joueur* joueur);
void joueurs_place_suivant(ListeJoueurs* liste, Joueur* joueur);
void joueurs_liberer_liste(ListeJoueurs* liste);

Joueur* creerJoueurHumain(int id);
Joueur* creerJoueurRandom(int id);
Joueur* creerJoueurDefense(int id);

#endif

