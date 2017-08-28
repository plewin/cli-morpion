#ifndef MORPION_H
#define MORPION_H

/**
 * \file morpion.h
 * \brief Structures Morpion et MorpionConfig et prototypes.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#define MORPION_DEFAULT_LONGUEUR   8
#define MORPION_DEFAULT_LARGEUR    8
#define MORPION_DEFAULT_ALIGNEMENT 4

#include "grille.h"
#include "joueur.h"
#include "user_interface.h"

/**
 * \struct MorpionConfig
 * \brief Contient la configuration d'un jeu de morpion.
 *
 * Un MorpionConfig permet de passer à fonctions la configuration entière d'un
 * jeu de Morpion comme une configuration initiale pour construire correctement
 * la Grille.
 */
typedef struct MorpionConfig {
	int longueur;   /**< Longueur de la grille. */
	int largeur;    /**< Largeur de la grille. */
	int alignement; /**< Nombre de pions à aligner pour gagner. */
} MorpionConfig;

/**
 * \struct Morpion
 * \brief Contient une MorpionConfig, une Grille, une ListeJoueurs et une UserInterface.
 *
 * La structure Morpion contient un pointeur vers la grille, un pointeur vers
 * la liste des joueurs et un MorpionConfig.
 * Pour gérer de multiples interfaces, une UserInterface est aussi stockée.
 */
typedef struct Morpion {
	Grille* grille;              /**< Pointeur vers une Grille. */
	MorpionConfig config;        /**< Configuration du jeu. */
	ListeJoueurs* liste_joueurs; /**< Liste des joueurs. */
	UserInterface ui;            /**< UserInterface à utiliser. */
} Morpion;

MorpionConfig morpion_config_parse_options(int argc, char* argv[]);
void morpion_reset_grille(Morpion* morpion);
void morpion_free_resources(Morpion* morpion);
void morpion_add_players(Morpion* morpion);
void morpion_play(Morpion* morpion);

char* morpion_config_serialize(MorpionConfig config);
MorpionConfig morpion_config_deserialize(char*);

#endif
