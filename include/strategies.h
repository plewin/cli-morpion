#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "joueur.h"
#include "grille.h"

void strategie_manuelle(Joueur* joueur, Grille* grille, int* x, int* y);
void strategie_random(Joueur* joueur, Grille* grille, int* x, int* y);
void strategie_defense(Joueur* joueur, Grille* grille, int* x, int* y);


#endif
