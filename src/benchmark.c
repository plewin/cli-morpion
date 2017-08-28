/**
 * \file benchmark.c
 * \brief Commande pour vérifier les performances des algorithmes de stratégie.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */
#include <stdlib.h>
#include <time.h>

#include "morpion.h"

int main(int argc, char* argv[]) {
	Morpion morpion;

	srand ( time(NULL) );

	morpion.grille = NULL;
	morpion.config = morpion_config_parse_options(argc, argv);
	morpion.ui     = null_interface_create();

	morpion_reset_grille(&morpion);

	morpion.liste_joueurs = joueurs_creer_liste(creerJoueurRandom(1));
	joueurs_place_suivant(morpion.liste_joueurs, creerJoueurDefense(2));

	morpion_play(&morpion);

	morpion_free_resources(&morpion);

	return EXIT_SUCCESS;
}
