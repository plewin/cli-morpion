/**
 * \file server.c
 * \brief Serveur pour un morpion en réseau.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "server.h"
#include "protocol.h"
#include "morpion.h"

/**
 * \fn int main(int argc, char* argv[])
 * \brief Point d'entrée pour le serveur du morpion en réseau.
 *
 * \param argc Nombre d'arguments de la commande.
 * \param argv Tableau des arguments.
 *
 * \return Code de sortie du programme.
 */
int main(int argc, char* argv[]) {
	void *context = zmq_init(1);
	void *responder = zmq_socket(context, ZMQ_REP);

	Morpion morpion;

	morpion.grille = NULL;
	morpion.config = morpion_config_parse_options(argc, argv);
	morpion.ui     = text_interface_create();

	int nbJoueurs = 0;

	morpion_reset_grille(&morpion);

	morpion.liste_joueurs = NULL;

	zmq_bind(responder, "tcp://*:5555");

	while (1) {
		char* request_data;

		zmq_msg_t request;
		zmq_msg_init(&request);
		zmq_recv(responder, &request, 0);
		request_data = (char*) zmq_msg_data(&request);
		switch(*request_data) {
			case PROTOCOL_JOIN:
				nbJoueurs++;
				printf("DEBUG: Joueur %d rejoint la partie : %s\n", nbJoueurs, request_data+1);
				if(morpion.liste_joueurs == NULL) {
					morpion.liste_joueurs = joueurs_creer_liste(creerJoueurHumain(nbJoueurs));
				} else {
					joueurs_place_suivant(morpion.liste_joueurs, creerJoueurHumain(nbJoueurs));
				}
				zmq_msg_t reply;
				zmq_msg_init_size(&reply, sizeof(nbJoueurs));
				memcpy(zmq_msg_data(&reply), &nbJoueurs, sizeof(nbJoueurs));
				zmq_send(responder, &reply, 0);
				zmq_msg_close(&reply);
				break;
			case PROTOCOL_GET_CONFIG:
				printf("DEBUG: Un joueur veut récupérer la configuration du morpion.\n");
				{
					char* serialization = morpion_config_serialize(morpion.config);
					zmq_msg_t reply;
					zmq_msg_init_size(&reply, sizeof(MorpionConfig));
					memcpy(zmq_msg_data(&reply), serialization, sizeof(MorpionConfig));

					zmq_send(responder, &reply, 0);
					zmq_msg_close(&reply);
					free(serialization);
				}
				break;
			case PROTOCOL_GET_GRILLE:
				printf("DEBUG: Un joueur veut récupérer la grille.\n");
				{
					char* serialization = grille_serialize(morpion.grille);
					size_t taille = morpion.grille->largeur * morpion.grille->longueur * sizeof(int);
					zmq_msg_t reply;
					zmq_msg_init_size(&reply, taille);
					memcpy(zmq_msg_data(&reply), serialization, taille);

					for(int j = 0; j < morpion.grille->largeur; j++) {
						for(int i = 0; i < morpion.grille->longueur; i++) {
							int* ser = (int*) serialization;
							printf("%d ", ser[i + j*morpion.grille->longueur]);
						}
						printf("\n");
					}

					zmq_send(responder, &reply, 0);
					zmq_msg_close(&reply);
				}
				break;
			case PROTOCOL_GET_TURN:
				{
					int joueur_id = 0;
					if(morpion.liste_joueurs != NULL) {
						if(morpion.liste_joueurs->suivant != morpion.liste_joueurs) {
							joueur_id = morpion.liste_joueurs->joueur->id;
						}
					}
					printf("Un joueur veut savoir à qui est le tour.\n");
					zmq_msg_t reply;
					zmq_msg_init_size(&reply, sizeof(joueur_id));
					memcpy(zmq_msg_data(&reply), &joueur_id, sizeof(joueur_id));
					zmq_send(responder, &reply, 0);
					zmq_msg_close(&reply);
				}
				break;
			case PROTOCOL_PLAY_TURN:
				{
					printf("Play turn ...\n");
					int joueur_id = * ( (int*) (request_data+1) );
					int x         = * ( (int*) (request_data+1+1*sizeof(int)) );
					int y         = * ( (int*) (request_data+1+2*sizeof(int)) );
					int code;

					if(morpion.liste_joueurs == NULL) {
						/*Cheater*/
						break;
					}

					if(joueur_id != morpion.liste_joueurs->joueur->id) {
						/*Cheater*/
						break;
					}
					code = placerPion(morpion.grille, joueur_id, x, y);
					morpion.liste_joueurs = morpion.liste_joueurs->suivant;

					zmq_msg_t reply;
					zmq_msg_init_size(&reply, sizeof(code));
					memcpy(zmq_msg_data(&reply), &code, sizeof(code));
					zmq_send(responder, &reply, 0);
					zmq_msg_close(&reply);
				}
				break;
			case PROTOCOL_QUIT:
				printf("Un joueur a quité la partie\n");
				break;
			default:
				printf("ERREUR PROTOCOLE !!\n");
				break;
		}
		zmq_msg_close(&request);
	}

	zmq_close(responder);
	zmq_term(context);

	morpion_free_resources(&morpion);

	return 0;
}
