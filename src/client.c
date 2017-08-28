/**
 * \file client.c
 * \brief Fonctions pour gérer un jeu de morpion en réseau (client).
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <protocol.h>

#include "morpion.h"

/**
 * \fn void* client_initialize_context()
 * \brief Crée un contexte ZeroMQ pour utiliser les fontions réseau.
 *
 * \return Pointeur vers un contexte ZeroMQ.
 */
void* client_initialize_context() {
	return zmq_init(1);
}

/**
 * \fn void* client_connect(void* context)
 * \brief Connection au serveur qui héberge la partie de morpion.
 *
 * \param context Un contexte ZeroMQ déjà initialisé.
 * \return Pointeur vers le socket ZeroMQ.
 * \todo Laisser l'utilisateur choisir sur quel serveur se connecter.
 */
void* client_connect(void* context) {
#ifdef DEBUG
	printf("DEBUG: Connecting to Morpion server…\n");
#endif
	void *requester = zmq_socket(context, ZMQ_REQ);
	zmq_connect(requester, "tcp://localhost:5555");
	return requester;
}

/**
 * \fn MorpionConfig client_get_morpion_config(void* requester)
 * \brief Récupère la configuration du jeu sur le serveur.
 *
 *  Envoit au serveur une requête PROTOCOL_GET_CONFIG pour lui demander
 *  de retourner un MorpionConfig sérialisé.
 *
 * \param requester Socket ZeroMQ adresser une requête au serveur.
 * \return Structure MorpionConfig avec la configuration du serveur.
 */
MorpionConfig client_get_morpion_config(void* requester) {
	MorpionConfig config;
	char buffer[1];

	buffer[0] = (char) PROTOCOL_GET_CONFIG;
	zmq_msg_t request;
	zmq_msg_init_size(&request, 1);
	memcpy(zmq_msg_data(&request), buffer,1);
#ifdef DEBUG
	printf("DEBUG: Sending GET CONFIG command…\n");
#endif
	zmq_send(requester, &request, 0);
	zmq_msg_close(&request);

	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_recv(requester, &reply, 0);
	config = morpion_config_deserialize( (char*) zmq_msg_data(&reply) ) ;
#ifdef DEBUG
	printf("DEBUG: MorpionConfig.longueur   = %d\n", config.longueur );
	printf("DEBUG: MorpionConfig.largeur    = %d\n", config.largeur );
	printf("DEBUG: MorpionConfig.alignement = %d\n", config.alignement );
#endif
	zmq_msg_close(&reply);

	return config;
}

/**
 * \fn int client_join(void* requester)
 * \brief Rejoint une partie sur le serveur.
 *
 *  Envoit au serveur une requête PROTOCOL_JOIN pour lui demander
 *  d'ajouter un nouveau joueur à la partie et nous fournir un identifiant
 *  de joueur.
 *
 * \param requester Socket ZeroMQ adresser une requête au serveur.
 * \return L'identifiant joueur du client.
 */
int client_join(void* requester) {
	char* login = getenv("USER");
	char buffer[255];
	int joueur_id;

	buffer[0] = (char) PROTOCOL_JOIN;
	strcpy(&buffer[1], login);

	zmq_msg_t request;
	zmq_msg_init_size(&request, strlen(login) + 1);
	memcpy(zmq_msg_data(&request), buffer, strlen(login) + 1);
#ifdef DEBUG
	printf("DEBUG: Sending JOIN command…\n");
#endif
	zmq_send(requester, &request, 0);
	zmq_msg_close(&request);

	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_recv(requester, &reply, 0);
	joueur_id = * ( (int*)zmq_msg_data(&reply) );
#ifdef DEBUG
	printf("DEBUG: Joueur ID = %d\n", joueur_id );
#endif
	zmq_msg_close(&reply);

	return joueur_id;
}

/**
 * \fn void client_update_morpion_grille(Grille* grille, void* requester)
 * \brief Récupère une grille à jour du serveur.
 *
 *  Envoit au serveur une requête PROTOCOL_GET_GRILLE pour lui demander
 *  une version à jour de la grille de la partie en cours.
 *  La réponse du serveur est désérialisée dans le tableau du paramètre
 *  grille.
 *
 * \param grille Grille de morpion, ne doit pas être nulle.
 * \param requester Socket ZeroMQ adresser une requête au serveur.
 */
void client_update_morpion_grille(Grille* grille, void* requester) {
	char buffer[1];
	char* serialized;

	buffer[0] = (char) PROTOCOL_GET_GRILLE;
	zmq_msg_t request;
	zmq_msg_init_size(&request, 1);
	memcpy(zmq_msg_data(&request), buffer,1);
#ifdef DEBUG
	printf("DEBUG: Sending GET GRILLE command…\n");
#endif
	zmq_send(requester, &request, 0);
	zmq_msg_close(&request);

	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_recv(requester, &reply, 0);
	serialized = (char*) zmq_msg_data(&reply);
	grille_update_deserialize(grille, serialized);
	zmq_msg_close(&reply);
#ifdef DEBUG
	for(int j = 0; j < grille->largeur; j++) {
		for(int i = 0; i < grille->longueur; i++) {
			/*nt* ser = (int*) serialized;*/
			printf("%d ", grille->tab[j][i]);
		}
		printf("\n");
	}
#endif
}

/**
 * \fn int client_get_player_turn(void* requester)
 * \brief Récupère l'identifiant du joueur courant.
 *
 *  Envoit au serveur une requête PROTOCOL_GET_TURN pour lui demander
 *  l'identifiant du joueur actuel (qui doit jouer le prochain mouvement)
 *  Si l'identifiant retournée est 0, c'est que personne ne doit jouer (car il
 *  manque un joueur).
 *
 * \param requester Socket ZeroMQ adresser une requête au serveur.
 * \return L'identifiant du joueur qui doit jouer le prochain tour. 0 si personne.
 */
int client_get_player_turn(void* requester) {
	char buffer[1];
	int joueur_id;

	buffer[0] = (char) PROTOCOL_GET_TURN;

	zmq_msg_t request;
	zmq_msg_init_size(&request, 1);
	memcpy(zmq_msg_data(&request), buffer, 1);
#ifdef DEBUG
	printf("DEBUG: Sending GET_TURN command…\n");
#endif
	zmq_send(requester, &request, 0);
	zmq_msg_close(&request);

	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_recv(requester, &reply, 0);
	joueur_id = * ( (int*)zmq_msg_data(&reply) );
#ifdef DEBUG
	printf("DEBUG: Current turn Joueur ID = %d\n", joueur_id );
#endif
	zmq_msg_close(&reply);

	return joueur_id;
}

/**
 * \fn int client_play_turn(void* requester, int joueur_id, int x, int y)
 * \brief Envoit au serveur la position choisie pour jouer.
 *
 *  Envoit au serveur une requête PROTOCOL_PLAY_TURN avec la position x et y
 *  de la case dans la grille choisie par le joueur.
 *  La position peut être refusée par le serveur.
 *
 * \param requester Socket ZeroMQ adresser une requête au serveur.
 * \param joueur_id L'identifiant du joueur qui joue son tour.
 * \param x La position x de la case.
 * \param y La position y de la case.
 * \return L'entier 1 si la position est acceptée par le serveur, 0 sinon.
 */
int client_play_turn(void* requester, int joueur_id, int x, int y) {
	int code;
	size_t message_len = 3*sizeof(int) + 1;
	char buffer[message_len];

	buffer[0] = (char) PROTOCOL_PLAY_TURN;
	memcpy(&buffer[1 + 0*sizeof(int)], &joueur_id, sizeof(int));
	memcpy(&buffer[1 + 1*sizeof(int)], &x, sizeof(int));
	memcpy(&buffer[1 + 2*sizeof(int)], &y, sizeof(int));

	zmq_msg_t request;
	zmq_msg_init_size(&request, message_len);
	memcpy(zmq_msg_data(&request), buffer, message_len);
#ifdef DEBUG
	printf("DEBUG: Sending PLAY_TURN command…\n");
#endif
	zmq_send(requester, &request, 0);
	zmq_msg_close(&request);

	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_recv(requester, &reply, 0);
	code = * ( (int*)zmq_msg_data(&reply) );
#ifdef DEBUG
	printf("DEBUG: Code = %d\n", code );
#endif
	zmq_msg_close(&reply);

	return code;
}

/**
 * \fn void client_quit(void* context, void* requester)
 * \brief Libère les resources réseau.
 *
 *  Ferme le socket et termine le contexte.
 * \param context Un contexte ZeroMQ déjà initialisé.
 * \param requester Socket ZeroMQ adresser une requête au serveur.
 */
void client_quit(void* context, void* requester) {
	zmq_close(requester);
	zmq_term(context);
}

/**
 * \fn int main(int argc, char* argv[])
 * \brief Point d'entrée pour le client du morpion en réseau.
 *
 * \param argc Nombre d'arguments de la commande.
 * \param argv Tableau des arguments.
 *
 * \return Code de sortie du programme.
 */
int main(int argc, char* argv[]) {
	void* context   = client_initialize_context();
	void* requester = client_connect(context);

	int joueur_id   = client_join(requester);

	Morpion morpion;
	Joueur* joueur = creerJoueurHumain(joueur_id);

	morpion.grille = NULL;
	morpion.config = client_get_morpion_config(requester);
	morpion.ui     = text_interface_create();
	morpion_reset_grille(&morpion);

	client_update_morpion_grille(morpion.grille, requester);

	afficherGrille(morpion.grille);

	int last_joueur_seen = 0;
	int joueur_actuel_id = 0;
	do {
		int x, y;

		while((joueur_actuel_id = client_get_player_turn(requester)) != joueur_id) {
			if(joueur_actuel_id != last_joueur_seen) {
				printf("Tour de joueur %d :\n", joueur_actuel_id);
				client_update_morpion_grille(morpion.grille, requester);
				afficherGrille(morpion.grille);
				last_joueur_seen = joueur_actuel_id;
			}
			if(joueur_actuel_id == 0) {
				printf("Attente d'un autre joueur...");
			}
			sleep(1);
		}
		printf("Tour de joueur %d (c'est votre tour) :\n", joueur_actuel_id);
		client_update_morpion_grille(morpion.grille, requester);
		afficherGrille(morpion.grille);
		joueur->place(joueur, morpion.grille, &x, &y);

		client_play_turn(requester, joueur_id, x, y);
		afficherGrille(morpion.grille);

		sleep(1);
	} while(!estPleineGrille(morpion.grille));


	client_quit(context, requester);

	return EXIT_SUCCESS;
}
