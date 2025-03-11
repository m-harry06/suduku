/*
    c'est dans ce fichier que toute les structures utile pour le programme sont creer 
    je parle des structures et des variable globale 
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

extern char username[20];

typedef struct Utilisateur
{
    int id;
    char nom[20];
    char prenom[20];
    char nom_utilisateur[20];
    char password[20];
}Utilisateur;


#endif