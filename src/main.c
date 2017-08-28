/**
 * \file main.c
 * \brief Point d'entrée du jeu morpion.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */
#include <stdlib.h>
#include <time.h>

#include "morpion.h"
#include "user_interface.h"
/**
 * \fn int main(int argc, char* argv[])
 * \brief Point d'entrée pour le jeu morpion.
 *
 * \param argc Nombre d'arguments de la commande.
 * \param argv Tableau des arguments.
 *
 * \return Code de sortie du programme.
 */
int main(int argc, char* argv[]) {
	Morpion morpion;

	srand ( time(NULL) );

	morpion.grille = NULL;
	morpion.config = morpion_config_parse_options(argc, argv);
	morpion.ui     = text_interface_create();

	morpion_reset_grille(&morpion);
	morpion_add_players(&morpion);
	morpion_play(&morpion);

	morpion_free_resources(&morpion);

	return EXIT_SUCCESS;
}
