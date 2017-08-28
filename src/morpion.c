/**
 * \file morpion.c
 * \brief Fonctions pour gérer un jeu de morpion.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <unistd.h>

#include "morpion.h"
#include "lib.h"
#include "grille.h"
#include "joueur.h"

/**
 * \fn MorpionConfig morpion_config_parse_options (int argc, char* argv[])
 * \brief Génère une configurationde morpion en fonction des arguments.
 *
 * \param argc Nombre d'arguments de la commande (du main).
 * \param argv Tableau des chaines des arguments de la commande (du main).
 * \return Configuration du morpion en fonction des arguments.
 */
MorpionConfig morpion_config_parse_options(int argc, char* argv[]) {
	int next_option;
	const char* const short_options = "y:x:a:h";

	const struct option long_options[] = {
		{ "hauteur",    0, NULL, 'y' },
		{ "largeur",    1, NULL, 'x' },
		{ "alignement", 0, NULL, 'a' },
		{ NULL,         0, NULL,   0 }
	};

	MorpionConfig config;
	config.longueur   = MORPION_DEFAULT_LONGUEUR;
	config.largeur    = MORPION_DEFAULT_LARGEUR;
	config.alignement = MORPION_DEFAULT_ALIGNEMENT;

	do {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL );

		switch (next_option) {
		case 'h':
			print_usage(stdout, 0);
			break;
		case 'x':
			config.longueur = atoi(optarg);
			break;
		case 'y':
			config.largeur = atoi(optarg);
			break;
		case 'a':
			config.alignement = atoi(optarg);
			break;
		case '?':
			print_usage(stderr, 1);
			break;
		case -1:
			break;
		default:
			abort();
		}
	} while (next_option != -1);

	return config;
}

/**
 * \fn void morpion_reset_grille(Morpion* morpion)
 * \brief Réalloue la grille dans les dimensions précisés par la configuration.
 *
 * \param morpion Morpion dont la grille doit être réallouée.
 */
void morpion_reset_grille(Morpion* morpion) {
	if(morpion->grille != NULL) {
		libererGrille(morpion->grille);
	}

	morpion->grille = initGrille(
		morpion->config.longueur,
		morpion->config.largeur
	);
}

/**
 * \fn void morpion_reset_grille(Morpion* morpion)
 * \brief Libère les resources allouées pour le morpion.
 *
 * \param morpion Morpion avec des resources à libérer.
 */
void morpion_free_resources(Morpion* morpion) {
	libererGrille(morpion->grille);
	joueurs_liberer_liste(morpion->liste_joueurs);
}


/**
 * \fn void morpion_add_players(Morpion* morpion)
 * \brief Construit la liste circulaire des joueurs.
 *
 * \param morpion Morpion dont la liste des joueurs est à construire.
 */
void morpion_add_players(Morpion* morpion) {
	Joueur* joueur1 = creerJoueurHumain(1);

	morpion->liste_joueurs = joueurs_creer_liste(joueur1);
	joueurs_place_suivant(morpion->liste_joueurs, creerJoueurDefense(2));
}

/**
 * \fn void morpion_play(Morpion* morpion)
 * \brief Lance la boucle de jeu.
 *
 * Boucle de jeu :
 * Afficher la grille vide.
 * Tant que la grille n'est pas pleine :
 * - Afficher l'identifiant du joueur actuel ;
 * - Laisser jouer le joueur actuel selon sa stratégie ;
 * - Afficher la grille ;
 * - Vérifier si le nouveau pion est dans un alignement (si oui on arrête le jeu).
 * - On passe au joueur suivant.
 *
 * \param morpion Morpion à jouer.
 */
void morpion_play(Morpion* morpion) {

	morpion->ui.update_grille(morpion->grille);

	do {
		int x, y;
		Joueur* joueur_actuel = morpion->liste_joueurs->joueur;

		morpion->ui.log("Tour de joueur %d :\n", joueur_actuel->id);
		joueur_actuel->place(joueur_actuel, morpion->grille, &x, &y);
		morpion->ui.log("Le joueur %d a placé en (%d, %d).\n", joueur_actuel->id, x, y);
		morpion->ui.update_grille(morpion->grille);
		if(alignePion(morpion->grille, x, y, morpion->config.alignement)) {
			morpion->ui.log("Le joueur %d a gagné !\n", joueur_actuel->id);
			break;
		}

		morpion->liste_joueurs = morpion->liste_joueurs->suivant;
	} while(!estPleineGrille(morpion->grille));

}

/**
 * \fn char* morpion_config_serialize(MorpionConfig config)
 * \brief Sérialise une MorpionConfig.
 *
 * Cette fonction doit être utilisé pour récupérer une chaine d'octets à envoyer
 * sur le réseau.
 *
 * \param config La MorpionConfig à sérialiser.
 *
 * \warning Il FAUT free cette serialisation !!
 * \warning Cette sérialisation est dépendante de l'endianness de l'architecture.
 *
 * \todo Faire une implémentation indépendante de l'endianness.
 *
 * \return Pointeur vers la suite d'octets sérialisés.
 */
char* morpion_config_serialize(MorpionConfig config) {
	char* buffer = (char*) malloc(sizeof(MorpionConfig));
	if(buffer == NULL) {
		perror("Impossible d'allouer une suite d'octets pour la sérialisation.");
		exit(EXIT_FAILURE);
	}
	*( (int*) (buffer + (0 * sizeof(int))) ) = config.longueur;
	*( (int*) (buffer + (1 * sizeof(int))) ) = config.largeur;
	*( (int*) (buffer + (2 * sizeof(int))) ) = config.alignement;

	return buffer;
}

/**
 * \fn MorpionConfig morpion_config_deserialize(char* serialized)
 * \brief Désérialise un MorpionConfig.
 *
 * \param serialized Suite d'octets à désérialiser.
 * \return MorpionConfig issu de la désérialisation.
 */
MorpionConfig morpion_config_deserialize(char* serialized) {
	MorpionConfig config;
	config.longueur   = *( (int*) serialized);
	config.largeur    = *( (int*) ( serialized + sizeof(int) ) );
	config.alignement = *( (int*) ( serialized + 2 * sizeof(int) ) );

	return config;
}
