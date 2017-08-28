#ifndef GRILLE_H
#define GRILLE_H

/**
 * \file grille.h
 * \brief Structure Grille et prototypes.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include "user_interface.h"

/**
 * \struct Grille
 * \brief Contient un tableau 2D d'entiers avec ses dimensions.
 *
 * La Grille d'un jeu de Morpion contient l'état du jeu dans un tableau 2D
 * d'entiers. Les entiers dans la grille sont les identifiants des joueurs qui
 * ont posé les pions dedans.
 * La structure a aussi pour champs,
 * - les dimentions (longueur et largeur) du tableau d'entiers ;
 * - le nombre de cases libres.
 *
 * Pour être correctement sérialisé par ::grille_serialize, la grille doit être
 * alloué et initialisé par ::initGrille.
 */
typedef struct Grille {
	int** tab;     /*!< Tableau 2D d'entier. */
	int longueur;  /*!< Longueur de la grille. */
	int largeur;   /*!< Largeur de la grille. */
	int libres;    /*!< Nombre de cases libres dans la grille. */
} Grille;

Grille* initGrille(int x, int y);
void libererGrille(Grille * G);
int placerPion(Grille * G, int J, int x, int y);
int estPleineGrille(Grille * G);
int alignePion(Grille * G, int x, int y, int n);
void afficherGrille(Grille * G);

char* grille_serialize(Grille* grille);
void grille_update_deserialize(Grille*, char*);

#endif
