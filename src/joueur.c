/**
 * \file joueur.c
 * \brief Fonctions pour gérer des Joueur et ListeJoueurs.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include <stdlib.h>
#include <stdio.h>

#include "joueur.h"
#include "grille.h"
#include "strategies.h"

/**
 * \fn ListeJoueurs* joueurs_creer_liste(Joueur* joueur)
 * \brief Crée une liste circulaire simplement chainée un premier joueur.
 *
 * La liste circulaire crée est un élément de liste circulaire dont le joueur
 * est le premier joueur et l'élément suivant est le même élément.
 *
 * \param joueur Le premier joueur de la liste.
 * \return Un élément de ListeJoueurs, seul élément de la liste.
 */
ListeJoueurs* joueurs_creer_liste(Joueur* joueur) {
	ListeJoueurs* liste_circulaire = (ListeJoueurs*) malloc(sizeof(ListeJoueurs));
	if(liste_circulaire == NULL) {
		perror("Impossible d'allouer la liste des joueurs.");
		exit(EXIT_FAILURE);
	}
	liste_circulaire->joueur  = joueur;
	liste_circulaire->suivant = liste_circulaire;

	return liste_circulaire;
}

/**
 * \fn void joueurs_place_suivant(ListeJoueurs* liste, Joueur* joueur)
 * \brief Ajoute un élément contenant un joueur à la liste circulaire liste.
 *
 * \param liste Liste circulaire où le nouveau élément doit être ajouté.
 * \param joueur Joueur à mettre dans le nouveau élément de la liste circulaire.
 */
void joueurs_place_suivant(ListeJoueurs* liste, Joueur* joueur) {
	ListeJoueurs* nouveau = joueurs_creer_liste(joueur);
	nouveau->suivant = liste->suivant;
	liste->suivant   = nouveau;
}

/**
 * \fn void joueurs_liberer_liste(ListeJoueurs* liste)
 * \brief Libère les Joueurs et les éléments de la liste.
 */
void joueurs_liberer_liste(ListeJoueurs* liste) {
	ListeJoueurs* premier;
	ListeJoueurs* suivant;

	if(liste == NULL) {
		return;
	}

	/* Cas spécial un seul élément */
	if(liste == liste->suivant) {
		free(liste->joueur);
		free(liste);
		return;
	}

	for(
			premier = liste, suivant = liste->suivant;
			liste != NULL && (suivant != premier);
			liste = suivant
	) {
		suivant = liste->suivant;
		free(liste->joueur);
		free(liste);
	}
}

/**
 * \fn Joueur* creerJoueurHumain(int id)
 * \brief Crée un joueur avec la stratégie manuelle.
 *
 * \param id Identifiant du joueur.
 * \return Joueur avec stratégie manuelle.
 */
Joueur* creerJoueurHumain(int id) {
	Joueur* joueur = (Joueur*) malloc(sizeof(Joueur));
	if(joueur == NULL) {
		perror("Impossible d'allouer le joueur");
		exit(EXIT_FAILURE);
	}
	joueur->id = id;
	joueur->place = strategie_manuelle;

	return joueur;
}

/**
 * \fn Joueur* creerJoueurRandom(int id)
 * \brief Crée un joueur avec la stratégie aléatoire.
 *
 * \param id Identifiant du joueur.
 * \return Joueur avec stratégie aléatoire.
 */
Joueur* creerJoueurRandom(int id) {
	Joueur* joueur = (Joueur*) malloc(sizeof(Joueur));
	if(joueur == NULL) {
		perror("Impossible d'allouer le joueur");
		exit(EXIT_FAILURE);
	}
	joueur->id = id;
	joueur->place = strategie_random;

	return joueur;
}

/**
 * \fn Joueur* creerJoueurDefense(int id)
 * \brief Crée un joueur avec la stratégie défense.
 *
 * \param id Identifiant du joueur.
 * \return Joueur avec stratégie défense.
 */
Joueur* creerJoueurDefense(int id) {
	Joueur* joueur = (Joueur*) malloc(sizeof(Joueur));
	if(joueur == NULL) {
		perror("Impossible d'allouer le joueur");
		exit(EXIT_FAILURE);
	}
	joueur->id = id;
	joueur->place = strategie_defense;

	return joueur;
}
