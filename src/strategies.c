#include <stdio.h>
#include <stdlib.h>

#include "strategies.h"

/**
 * \fn void strategie_manuelle(Joueur* joueur, Grille* grille, int* x, int* y)
 * \brief Stratégie manuelle.
 *
 * \param joueur Joueur qui a la stratégie.
 * \param grille Grille sur laquelle il faut jouer.
 * \param x Pointeur pour sauver la position x de la case choisie.
 * \param y Pointeur pour sauver la position y de la case choisie.
 */
void strategie_manuelle(Joueur* joueur, Grille* grille, int* x, int* y) {
	do {
		printf("Entrez les coordonées d'une case :\n");
		scanf("%d %d", x, y);
	} while(!placerPion(grille, joueur->id, *x, *y));
}

/**
 * \fn void strategie_random(Joueur* joueur, Grille* grille, int* x, int* y)
 * \brief Stratégie aléatoire, place le pion dans une case vide aléatoirement.
 *
 * \param joueur Joueur qui a la stratégie.
 * \param grille Grille sur laquelle il faut jouer.
 * \param x Pointeur pour sauver la position x de la case choisie.
 * \param y Pointeur pour sauver la position y de la case choisie.
 */
void strategie_random(Joueur* joueur, Grille* grille, int* x, int* y) {
	do {
		*x = rand() % grille->longueur;
		*y = rand() % grille->largeur;
	} while(!placerPion(grille, joueur->id, *x, *y));
}

struct element_list_id {
   int joueur_id;
   struct element_list_id* next;
};

typedef struct element_list_id element_list_id;
typedef element_list_id* list_id;

char in_list_id(list_id list, int id_joueur) {
	if(list == NULL) {
		return 0;
	}

	while(list != NULL) {
		if(list->joueur_id == id_joueur) {
			return 1;
		}

		list = list->next;
	}

	return 0;
}

void add_list_id(list_id* list, int id_joueur) {
	element_list_id* nouveau;

	if(list == NULL) {
		return;
	}

	nouveau = (element_list_id*) malloc(sizeof(element_list_id));
	if(nouveau == NULL) {
		perror("Impossible d'allouer un élément de liste chainé.");
		exit(1);
	}
	nouveau->joueur_id = id_joueur;
	nouveau->next = NULL;

	if(*list == NULL) {
		*list = nouveau;
	} else {
		element_list_id* dernier = *list;
		while(dernier->next != NULL) {
			dernier = dernier->next;
		}
		dernier->next = nouveau;
	}
}

void free_list_id(list_id list) {
	while(list != NULL) {
		element_list_id* tmp = list->next;
		free(list);
		list = tmp;
	}
}

void strategie_defense(Joueur* joueur, Grille* grille, int* x, int* y) {
	int i, j;
	list_id liste_joueurs = NULL;
	element_list_id* joueur_courant;
	int nombre_adversaires = 0;

	int joueur_dangereux_coefficient = 0;
	int x_dangereux = 0;
	int y_dangereux = 0;

	for(j = 0; j < grille->largeur; j++) {
		for(i = 0; i < grille->longueur; i++) {
			if(grille->tab[j][i] != 0) {
				if(!in_list_id(liste_joueurs, grille->tab[j][i])) {
					add_list_id(&liste_joueurs, grille->tab[j][i]);
					nombre_adversaires++;
				}
			}
		}
	}

	joueur_courant = liste_joueurs;

	while(joueur_courant != NULL) {
		for(j = 0; j < grille->largeur; j++) {
			for(i = 0; i < grille->longueur; i++) {
				if(grille->tab[j][i] == 0) {
					grille->tab[j][i] = joueur_courant->joueur_id;

					if(alignePion(grille,i,j, joueur_dangereux_coefficient+1)) {
						x_dangereux = i;
						y_dangereux = j;
						joueur_dangereux_coefficient += 1;
					}

					grille->tab[j][i] = 0;
				}
			}
		}

		joueur_courant = joueur_courant->next;
	}

	*x = x_dangereux;
	*y = y_dangereux;

	placerPion(grille, joueur->id, *x, *y);
}
