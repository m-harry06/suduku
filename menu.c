#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "menu.h"
#include "main.h"
#include "auth.h"

char username[20];

void menu_acceuil(){

    system("cls");

    printf("-----------------------------------------------------------------------------\n");
    printf("\n \t \t \t \t  *** Menu principal *** \n ");
    printf("-----------------------------------------------------------------------------\n");
    printf("\n\t faites un choix \n");
    printf (" \n \t \t |1.Connectez vous a votre espace personnel \n \t \t |2.Creer un compte \n \t \t |3.Sdoku resolver\n |Contacter un administrateur\n");
    
}

void gerer_menu_acceuil(int choix){
    switch (choix){
        case 1:
            connecter_menu();
            break;
        case 2:
            addNewPlayers();
            break;
        case 3:
            
            break;
        case 4:
            contact_admin_menu();
            break;
        default:
            menu_acceuil();
            break;
    }
}

void contact_admin_menu(){

    system("cls");

    printf("-----------------------------------------------------------------------------\n");
    printf("\t\t Contacter un administrateur \n");
    printf("-----------------------------------------------------------------------------\n");

    printf(" \t Harry MBENGMO \n \t \t Tel: +237 6 96 56 46 38 \n \t \t mail: hmbengmo@gmail.com \n");


    int b, y = 0;
    do{
        printf(" entrer 1 pour revenir au menu principal ");
        scanf("%d", &b);
        y++;
    }while( b != 1);

}

int getLastId() {
    FILE *player_file = fopen("utilisateurs.csv", "r");
    int lastId = 0;

    if (player_file == NULL) {
        return 1;
    }

    char line[200];
    while (fgets(line, sizeof(line), player_file)) {
        if (strlen(line) > 1) {
            int currentId;
            sscanf(line, "%d,", &currentId);
            lastId = currentId;
        }
    }

    fclose(player_file);
    return lastId + 1;
}


void addNewPlayers() {

    system("cls");

    FILE *player_file = fopen("utilisateurs.csv", "a");
    if (player_file == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    Utilisateur user;
    char ch;
    int i = 0;

    printf("-----------------------------------------------------------------------------\n");
    printf("\t\t Nouvel utilisateur \n");
    printf("-----------------------------------------------------------------------------\n");

    user.id = getLastId();
    printf("Entrer votre nom: ");
    getchar();
    fgets(user.nom, 20, stdin);
    user.nom[strcspn(user.nom, "\n")] = 0;  

    printf("Entrer votre prenom: ");
    fgets(user.prenom, 20, stdin);
    user.prenom[strcspn(user.prenom, "\n")] = 0;

    printf("Entrer votre pseudo: ");
    fgets(user.nom_utilisateur, 20, stdin);
    user.nom_utilisateur[strcspn(user.nom_utilisateur, "\n")] = 0;

    printf("Entrer votre mot de passe: ");

    while ((ch = _getch()) != '\r') {  
        if (ch == '\b' && i > 0) {  
            printf("\b \b");
            i--;
        } else if (ch != '\b' && i < (sizeof(user.password) - 1)) {
            user.password[i] = ch;
            printf("*");  
            i++;
        }
    }
    user.password[i] = '\0';
    printf("\n");

    fprintf(player_file, "%d,%s,%s,%s,%s\n", user.id, user.nom, user.prenom, user.nom_utilisateur, user.password);

    fclose(player_file);

    int a;
    do {
        printf("Entrer 1 pour revenir au menu principal et connectez vous: ");
        if (scanf("%d", &a) != 1) {
            while (getchar() != '\n'); 
        }
    } while (a != 1);
}



void connecter_menu(){

    reload:

    system("cls");
    int result = 0;
    char nom_utilisateur[20];
    char password[20];
    char ch;
    int i = 0;

    printf("-----------------------------------------------------------------------------\n");
    printf("\t\t connectez vous a votre espace personel \n");
    printf("-----------------------------------------------------------------------------\n");
    printf(" \t entrer votre nom d'utilisateurs: ");
    getchar();
    scanf("%[^\n]s", nom_utilisateur);
    printf(" \t entrer votre mot de passe: ");

    while ((ch = getch()) != '\r') {

        if(ch != '\b'){
        password[i] = ch;
        printf(".");
        i++;
        }else{
            printf("\b \b");
            i--;
        }
    }
    password[i] = '\0';

    if(authentification(nom_utilisateur, password)){
        system ("cls");
        memcpy(username, nom_utilisateur, sizeof(nom_utilisateur));
    }else{
        printf("echec de l'authentification");
        goto reload;
    }
}

void menu_niveau(){
    
    system("cls");

    printf("\t\t bienvenu : %s\n", username);

    printf("-----------------------------------------------------------------------------\n");
    printf("\n \t \t \t  *** CHOIX DE DIFICULTE *** \n ");
    printf("-----------------------------------------------------------------------------\n");
    printf("\n\t faites un choix \n");
    printf (" \n \t \t |1.Facile \n \t \t |2.Moyen \n \t \t |3.Difficile \n");
    
}

void gerer_menu_niveau(int choix){

}