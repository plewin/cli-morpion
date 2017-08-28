#ifndef PROTOCOL_H
#define PROTOCOL_H

/**
 * \file protocol.h
 * \brief Protocol niveau application pour gérer une partie de morpion en réseau.
 * \author Philippe Lewin <lewinp@esiee.fr>
 * \version 0.1
 * \date Janvier 2013
 */

/**
 * Le client veut rejoindre une partie.
 *
 * Requête du client
 * -----------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | ::PROTOCOL_JOIN
 * char*         | Nom d'utilisateur terminé par \0
 *
 *
 * Réponse du serveur
 * ------------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * int           | Identifiant du Joueur
 */
#define PROTOCOL_JOIN       0x01

/**
 * Le client veut quitter la partie.
 *
 * Requête du client
 * -----------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | ::PROTOCOL_QUIT
 * int           | Identifiant du joueur
 *
 *
 * Réponse du serveur
 * ------------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | 1 pour OK
 */
#define PROTOCOL_QUIT       0x02

/**
 * Le client veut la configuration du Morpion sur le serveur.
 *
 * Requête du client
 * -----------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | ::PROTOCOL_GET_CONFIG
 *
 *
 * Réponse du serveur
 * ------------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * MorpionConfig | MorpionConfig sérialisé
 */
#define PROTOCOL_GET_CONFIG 0x10

/**
 * Le client veut la configuration du Morpion sur le serveur.
 *
 * Requête du client
 * -----------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | ::PROTOCOL_GET_GRILLE
 *
 *
 * Réponse du serveur
 * ------------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char*         | Tableau 2D d'entiers sérialisé
 */
#define PROTOCOL_GET_GRILLE 0x11

/**
 * Le client veut savoir quel est le joueur qui doit jouer le prochain coup.
 *
 *
 * Requête du client
 * -----------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | ::PROTOCOL_GET_TURN
 *
 *
 * Réponse du serveur
 * ------------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * int           | Identifiant du joueur qui doit jouer le prochain coup.
 */
#define PROTOCOL_GET_TURN   0x12

/**
 * Le client veut transmettre le placement d'un pion sur la grille.
 *
 *
 * Requête du client
 * -----------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * char          | ::PROTOCOL_PLAY_TURN
 * int           | Identifiant du Joueur
 * int           | Position x sur la Grille
 * int           | Position y sur la Grille
 *
 *
 * Réponse du serveur
 * ------------------
 *
 * Type de champ | Valeur
 * ------------- | -------------
 * int           | Acceptation ou non du placement.
 */
#define PROTOCOL_PLAY_TURN  0x13

#endif
