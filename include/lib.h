#ifndef LIB_H
#define LIB_H

/**
 * \file lib.h
 * \brief Fonctions utilitaires pour le projet.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

/**
 * Nom du programme.
 *
 * \todo En faire un paramètre de ::print_usage.
 */
const char* program_name = "morpion";

/**
 * \fn void print_usage (FILE* stream, int exit_code)
 * \brief Affiche comment utiliser le programme.
 *
 * \param stream Le flux où écrire l'aide.
 * \param exit_code Le code d'erreur à utiliser.
 */
void print_usage (FILE* stream, int exit_code)
{
	fprintf (stream, "Utilisation : %s options [fichierentrée ...]\n", program_name);
	fprintf (stream,
			" -y --hauteur n          Fixe la hauteur du plateau à n cases.\n"
			" -x --largeur n          Fixe la largeur du plateau à n cases.\n"
			" -a --alignement n       Fixe à n le nombre de pions à aligner pour gagner.\n"
			" -h --help               Affiche une aide et quitte le programme.\n"
	);
	exit (exit_code);
}

#endif
