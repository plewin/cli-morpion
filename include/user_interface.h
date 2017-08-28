#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdio.h>

#include <grille.h>

/*Forward declaration pour un problème de dépendance circulaire*/
struct Grille;

typedef struct UserInterface {
	int (*log)(const char *, ...);
	void (*update_grille)(struct Grille *);
} UserInterface;

UserInterface text_interface_create();
UserInterface null_interface_create();

#endif
