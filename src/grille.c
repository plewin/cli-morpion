/**
 * \file grille.c
 * \brief Fonctions pour gérer une Grille de Morpion.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "grille.h"

/**
 * \fn Grille* initGrille(int x, int y)
 * \brief Alloue une Grille et l'initialise.
 *
 * Alloue une Grille de taille x*y et initialise le contenu de la grille
 * et les caractériques longueur, largeur et le compteur de cases libres.
 *
 * Le tableau est initialisé en 2 malloc pour être plus efficace en
 * sérialisation.
 * http://www.clubnix.fr/allocation_dynamique_2_dimensions_C
 *
 * \param x La longueur de la grille.
 * \param y La largeur de la grille.
 * \return Pointeur vers la Grille allouée, retourne NULL en cas d'échec.
 */
Grille* initGrille(int x, int y) {
	int *tabdata;
	int i;

	Grille* grille = (Grille*) malloc(sizeof(Grille));
	if(grille == NULL) {
		perror("Impossible d'allouer la grille.");
		return NULL;
	}

	grille->tab = (int **)malloc(sizeof(int*)*y);
	if(grille->tab == NULL) {
		perror("Impossible d'allouer les colonnes de la grille");
		free(grille);
		return NULL;
	}

	tabdata = (int *)malloc(sizeof(int)*x*y);
	if(grille->tab == NULL) {
		perror("Impossible d'allouer les lignes de la grille");
		free(grille->tab);
		free(grille);
		return NULL;
	}

	for(i = 0 ; i < y ; i++){
		grille->tab[i] = &tabdata[i*x];
	}
	memset(tabdata, 0, sizeof(int)*x*y);

	grille->longueur = x;
	grille->largeur  = y;
	grille->libres   = x*y;

	return grille;
}

/**
 * \fn void libererGrille(Grille * grille)
 * \brief Libère la mémoire utilisé par une grille.
 *
 * \param grille Pointeur vers la Grille à libérer.
 */
void libererGrille(Grille * grille) {
	free(grille->tab[0]);
	free(grille->tab);
	free(grille);
}

/**
 * \fn int placerPion(Grille * grille, int J, int x, int y)
 * \brief Place un pion d'un jour à une position donnée de la grille.
 *
 * Essaye de placer un pion du joueur numero J sur la case (x,y) de la grille,
 * qui doit être vide. La fonction renvoie 1 si tout s'est bien passé, 0 si
 * la case est occupée ou n'existe pas.
 *
 * \param grille Grille qui doit recevoir le pion.
 * \param J Identifiant du joueur qui veut placer un pion.
 * \param x Position x de la case.de la grille.
 * \param y Position y de la case de la grille.
 * \return 1 si tout c'est bien passé, 0 si la case est occupée ou n'existe pas.
 */
int placerPion(Grille * grille, int J, int x, int y)
{
	char hors_jeu_longueur = 0 > x || x >= grille->longueur;
	char hors_jeu_largeur  = 0 > y || y >= grille->largeur;
	char hors_jeu = hors_jeu_longueur||hors_jeu_largeur;
	char case_non_vide = hors_jeu ? 0 : grille->tab[y][x] != 0;

	if(hors_jeu_longueur||hors_jeu_largeur||case_non_vide) {
		return 0;
	}

	grille->tab[y][x] = J;
	grille->libres -= 1;

	return 1;
}

/**
 * \fn int estPleineGrille(Grille * grille)
 * \brief Vérifie si la grille n'est pas vide.
 *
 * \param grille Grille à vérifier.
 * \return 1 si la grille n'a plus de cases libres, 0 sinon.
 */
int estPleineGrille(Grille * grille) {
	return grille->libres == 0;
}

/**
 * \fn int compterDirection(Grille * grille, int x, int y, int n, int dx, int dy)
 * \brief Compte le nombre de pions consécutifs dans une direction et un sens
 *
 * Compte le nombre de pions consécutifs du même joueur dans une direction
 * et un sens donné par dx et dy.
 * La position initiale est donnée par x et y et cherche jusqu'à n pions.
 *
 * Le paramètre n n'est utile que pour la performance.
 *
 * \param grille La grille où on compte les pions.
 * \param x La position x de la case de départ.
 * \param y La position y de la case de départ.
 * \param n Le nombre maximal de cases à regarder.
 * \param dx La direction selon x, typiquement -1, 0 ou 1.
 * \param dy La direction selon y, typiquement -1, 0 ou 1.
 * \return Le nombre de cases du même joueur dans la direction (forcément >= 1).
 */
int compterDirection(Grille * grille, int x, int y, int n, int dx, int dy) {
	int id_joueur_case_depart = grille->tab[y][x];
	int count = 0;

	char hors_jeu_longueur, hors_jeu_largeur;

	while(
			hors_jeu_longueur = (x < 0) || (x >= grille->longueur),
			hors_jeu_largeur  = (y < 0) || (y >= grille->largeur),

			!hors_jeu_longueur && !hors_jeu_largeur && (count <= n)
			&&	(grille->tab[y][x] == id_joueur_case_depart)
	) {
		count +=  1;
		x     += dx;
		y     += dy;
#ifdef DEBUG
		printf("DEBUG: Count (%d, %d) %d, \n", i, j, count);
#endif
	}

	return count;
}

/**
 * \fn int compterLigne(Grille * grille, int x, int y, int n, int dx, int dy)
 * \brief Compte le nombre de pions consécutifs sur une même ligne
 *
 * Compte le nombre de pions consécutifs du même joueur dans une direction
 * et dans les deux sens à partir d'une position initiale.
 * La position initiale est donnée par x et y et cherche jusqu'à n pions dans
 * les deux sens.
 *
 * Le paramètre n n'est utile que pour la performance.
 *
 * \param grille La grille où on compte les pions.
 * \param x La position x de la case de départ.
 * \param y La position y de la case de départ.
 * \param n Le nombre maximal de cases à regarder pour chaque sens.
 * \param dx La direction x d'un vecteur directeur, typiquement -1, 0 ou 1.
 * \param dy La direction y d'un vecteur directeur, typiquement -1, 0 ou 1.
 * \return Le nombre de cases du même joueur sur la ligne (forcément >= 1).
 */
int compterLigne(Grille * grille, int x, int y, int n, int dx, int dy) {
	int comptage_sens_direct  = compterDirection(grille, x, y, n, dx, dy);
	int comptage_sens_oppose  = compterDirection(grille, x, y, n, -dx, -dy);

	int comptage_sans_doublon = comptage_sens_direct + comptage_sens_oppose -1;

	return comptage_sans_doublon;
}


/**
 * \fn int alignePion(Grille * grille, int x, int y, int n)
 * \brief Vérifie s'il y a un alignement de n pions à une position donnée
 *
 * Vérifie s'il y a un alignement de n pions d'un même joueur comprenant le pion
 * situé sur la case (x,y) de la grille.
 * S'il n'y a pas de pion sur la case, 0 est retourné.
 *
 * L'alignement va être vérifié selon la ligne horizontale, verticale et sur
 * les deux diagonales.
 *
 * \param grille La grille à regarder.
 * \param x La position x du pion à partir duquel on cherche un alignement.
 * \param y La position y du pion à partir duquel on cherche un alignement.
 * \param n Le nombre de pions à aligner pour considerer un alignement gagnant.
 *
 * \return 1 s'il y a un alignement de n pions, 0 sinon.
 */
int alignePion(Grille * grille, int x, int y, int n) {
	int comptage_ligne_horizontale = compterLigne(grille, x, y, n, 1, 0);
	int comptage_ligne_verticale   = compterLigne(grille, x, y, n, 0, 1);
	int comptage_ligne_diagonale1  = compterLigne(grille, x, y, n, 1, 1);
	int comptage_ligne_diagonale2  = compterLigne(grille, x, y, n, 1, -1);

	char alignement_gagnant_present =
			   comptage_ligne_horizontale >= n
			|| comptage_ligne_verticale   >= n
			|| comptage_ligne_diagonale1  >= n
			|| comptage_ligne_diagonale2  >= n;

	char alignement_vide = grille->tab[y][x] == 0;

#ifdef DEBUG
	printf("H  : %d\n", comptage_ligne_horizontale);
	printf("V  : %d\n", comptage_ligne_verticale);
	printf("D1 : %d\n", comptage_ligne_diagonale1);
	printf("D2 : %d\n", comptage_ligne_diagonale2);
#endif

	return alignement_gagnant_present && !alignement_vide;
}

/**
 * \fn void afficher_barre(int longueur)
 * \brief Affiche simplement une barre horizontale avec 2 extrémités.
 *
 * \param longueur Longueur de la barre sans les extrémités.
 *
 */
void afficher_barre(int longueur) {
	int i;
	printf("+");
	for(i = 0; i < longueur; i++) {
		printf("-");
	}
	printf("+\n");
}

/**
 * \fn void afficherGrille(Grille * grille)
 * \brief Affiche une grille
 *
 * La grille est affichée avec un encadré façon ASCII art.
 *
 * Le joueur 1 a pour caractère 'X', le joueur caractère 'O', les autres joueurs
 * (>= 3) ont les lettres de l'alphabet en minuscules.
 *
 * \param grille Grille à afficher.
 *
 */
void afficherGrille(Grille * grille) {
	int i;
	int j;

	afficher_barre(grille->longueur);
	for(j = 0; j < grille->largeur; j++) {
		printf("|");
		for(i = 0; i < grille->longueur; i++) {
			switch(grille->tab[j][i]) {
			case 0:
				printf(" ");
				break;
			case 1:
				printf("X");
				break;
			case 2:
				printf("O");
				break;
			default:
				printf("%c", 'a' + grille->tab[j][i] - 3);
				break;
			}
		}
		printf("|\n");
	}
	afficher_barre(grille->longueur);
}

/**
 * \fn char* grille_serialize(Grille* grille)
 * \brief Sérialise le contenu du tableau de la grille.
 *
 * Cette fonction doit être utilisé pour récupérer une chaine d'octets à envoyer
 * sur le réseau.
 *
 * \param grille La grille à sérialiser.
 *
 * \warning Il ne faut PAS free cette serialisation !!
 * \warning Cette sérialisation est dépendante de l'endianness de l'architecture.
 *
 * \todo Faire une implémentation indépendante de l'endianness.
 *
 * \return Pointeur vers la suite d'octets sérialisés.
 */
char* grille_serialize(Grille* grille) {
	return (char*) grille->tab[0];
}

/**
 * \fn void grille_update_deserialize(Grille* grille, char* serialized)
 * \brief Met à jour une grille à partir d'une serialisation de données de grille.
 *
 * \param grille Grille à mettre à jour.
 * \param serialized Chaine d'octets serialisée par grille_serialize.
 */
void grille_update_deserialize(Grille* grille, char* serialized) {
	int* element = (int*) serialized;
	int i, j;
	for (j = 0; j < grille->largeur; j++) {
		for (i = 0; i < grille->longueur; i++) {
			grille->tab[j][i] = element[i + j * grille->longueur];
		}
		printf("\n");
	}
}
