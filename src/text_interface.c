#include "user_interface.h"

int null_printf(const char * format, ...) {
	return 0;
}

void null_affiche_grille(Grille* grille) {
}

UserInterface text_interface_create()
{
	UserInterface ui;
	ui.log           = printf;
	ui.update_grille = afficherGrille;

	return ui;
}

UserInterface null_interface_create()
{
	UserInterface ui;
	ui.log           = null_printf;
	ui.update_grille = null_affiche_grille;

	return ui;
}
