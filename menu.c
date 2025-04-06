#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include "menu.h"
#include "main.h"
#include "auth.h"
#include "generer.h" // Assurez-vous d'inclure ce fichier d'en-tête
#include "affichageCentrer.h"

char username[20];

void afficherLigneSeparation() {
    afficherCentre("-----------------------------------------------------------------------------\n");
}

void afficherRetourMenuPrincipal() {
    int choix;
    do {
        afficherCentre("Entrer 1 pour revenir au menu principal");
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); // Nettoyer le buffer
        }
    } while (choix != 1);
}

void masquerCurseur() {
    printf("\033[?25l"); // Masque le curseur
}

void afficherCurseur() {
    printf("\033[?25h"); // Affiche le curseur
}

void effacerEcran() {
    printf("\033[H\033[J"); // Efface l'écran et repositionne le curseur en haut
}

void menu_acceuil() {
    effacerEcran();

    afficherLigneSeparation();
    afficherCentre("*** Menu principal ***");
    afficherLigneSeparation();
    afficherCentre("Faites un choix");
    afficherCentre("| 1. Connectez vous a votre espace personnel  |");
    afficherCentre("| 2. Creer un compte                          |");
    afficherCentre("| 3. Sudoku resolver                          |");
    afficherCentre("| 4. Contacter un administrateur              |");
    afficherCentre("| 5. Quitter                                  |");
    afficherLigneSeparation();
}

void afficherMenuPrincipal(int choix) {
    effacerEcran();
    afficherLigneSeparation();
    afficherCentre("*** Menu principal ***");
    afficherLigneSeparation();
    afficherCentre("Utilisez les flèches pour naviguer et Entrée pour sélectionner:");
    afficherCentre(choix == 1 ? "-> 1. Connectez vous a votre espace personnel  " : "   1. Connectez vous a votre espace personnel  ");
    afficherCentre(choix == 2 ? "-> 2. Creer un compte                          " : "   2. Creer un compte                          ");
    afficherCentre(choix == 3 ? "-> 3. Sudoku resolver                          " : "   3. Sudoku resolver                          ");
    afficherCentre(choix == 4 ? "-> 4. Contacter un administrateur              " : "   4. Contacter un administrateur              ");
    afficherCentre(choix == 5 ? "-> 5. Quitter                                  " : "   5. Quitter                                  ");
    afficherLigneSeparation();
}

void gerer_menu_acceuil() {
    int choix = 1; // Option par défaut
    char touche;

    masquerCurseur();
    afficherMenuPrincipal(choix);
    while (1) {
        touche = _getch();
        if (touche == -32) {
            touche = _getch();
            if (touche == 72 && choix > 1) {
                choix--;
                afficherMenuPrincipal(choix);
            } else if (touche == 80 && choix < 5) {
                choix++;
                afficherMenuPrincipal(choix);
            }
        } else if (touche == '\r') {
            break;
        }
    }
    afficherCurseur();

    switch (choix) {
        case 1:
            connecter_menu();
            break;
        case 2:
            addNewPlayers();
            break;
        case 3:
            afficherCentre("Fonctionnalité Sudoku resolver non implémentée.");
            system("pause");
            break;
        case 4:
            contact_admin_menu();
            break;
        case 5:
            exit(0);
    }
}

void contact_admin_menu() {
    effacerEcran();

    afficherLigneSeparation();
    afficherCentre("Contacter un administrateur");
    afficherLigneSeparation();
    afficherCentre("| Nom : Harry MBENGMO                     |");
    afficherCentre("| Tel : +237 6 96 56 46 38                |");
    afficherCentre("| Mail: hmbengmo@gmail.com                |");
    afficherLigneSeparation();

    afficherRetourMenuPrincipal();
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
    effacerEcran();

    FILE *player_file = fopen("utilisateurs.csv", "a");
    if (player_file == NULL) {
        afficherCentre("Erreur lors de l'ouverture du fichier");
        return;
    }

    Utilisateur user;
    char ch;
    int i = 0;

    afficherLigneSeparation();
    afficherCentre("Nouvel utilisateur");
    afficherLigneSeparation();

    user.id = getLastId();
    afficherCentre("Entrer votre nom:");
    getchar();
    fgets(user.nom, sizeof(user.nom), stdin);
    user.nom[strcspn(user.nom, "\n")] = 0;

    afficherCentre("Entrer votre prenom:");
    fgets(user.prenom, sizeof(user.prenom), stdin);
    user.prenom[strcspn(user.prenom, "\n")] = 0;

    afficherCentre("Entrer votre pseudo:");
    fgets(user.nom_utilisateur, sizeof(user.nom_utilisateur), stdin);
    user.nom_utilisateur[strcspn(user.nom_utilisateur, "\n")] = 0;

    afficherCentre("Entrer votre mot de passe:");
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

    afficherRetourMenuPrincipal();
    gerer_menu_acceuil(); // Retour au menu principal
}

void connecter_menu() {
    while (1) {
        effacerEcran();
        int result = 0;
        char nom_utilisateur[20];
        char password[20];
        char ch;
        int i = 0;

        afficherLigneSeparation();
        afficherCentre("Connectez vous a votre espace personel");
        afficherLigneSeparation();
        afficherCentre("Entrer votre nom d'utilisateurs:");
        getchar();
        scanf("%19[^\n]", nom_utilisateur);
        afficherCentre("Entrer votre mot de passe:");

        while ((ch = _getch()) != '\r') {
            if (ch == '\b' && i > 0) {
                printf("\b \b");
                i--;
            } else if (ch != '\b' && i < (sizeof(password) - 1)) {
                password[i] = ch;
                printf("*");
                i++;
            }
        }
        password[i] = '\0';

        if (authentification(nom_utilisateur, password)) {
            effacerEcran();
            strncpy(username, nom_utilisateur, sizeof(username) - 1);
            username[sizeof(username) - 1] = '\0';
            break;
        } else {
            afficherCentre("Echec de l'authentification");
            system("pause");
        }
    }
}

void menu_niveau() {
    effacerEcran();

    afficherCentre("Bienvenu:");
    afficherCentre(username);

    afficherLigneSeparation();
    afficherCentre("*** CHOIX DE DIFFICULTE ***");
    afficherLigneSeparation();
    afficherCentre("Faites un choix");
    afficherCentre("| 1. Facile                                |");
    afficherCentre("| 2. Moyen                                 |");
    afficherCentre("| 3. Difficile                             |");
    afficherCentre("| 4. charger une partie sauvegarder        |");
    afficherLigneSeparation();
}

void afficherMenuNiveau(int choix) {
    effacerEcran();
    afficherCentre("Bienvenu:");
    afficherCentre(username);
    afficherLigneSeparation();
    afficherCentre("*** CHOIX DE DIFFICULTE ***");
    afficherLigneSeparation();
    afficherCentre("Utilisez les flèches pour naviguer et Entrée pour sélectionner:");
    afficherCentre(choix == 1 ? "-> 1. Facile                                " : "   1. Facile                                ");
    afficherCentre(choix == 2 ? "-> 2. Moyen                                 " : "   2. Moyen                                 ");
    afficherCentre(choix == 3 ? "-> 3. Difficile                             " : "   3. Difficile                             ");
    afficherCentre(choix == 4 ? "-> 4. Charger une partie sauvegardée       " : "   4. Charger une partie sauvegardée       ");
    afficherLigneSeparation();
}

void gerer_menu_niveau() {
    int choix = 1; // Option par défaut
    char touche;

    masquerCurseur(); // Masquer le curseur pour réduire les distractions
    afficherMenuNiveau(choix); // Afficher le menu initial
    while (1) {
        touche = _getch();
        if (touche == -32) { // Touche spéciale (flèches)
            touche = _getch();
            if (touche == 72 && choix > 1) { // Flèche haut
                choix--;
                afficherMenuNiveau(choix); // Mettre à jour uniquement les options
            } else if (touche == 80 && choix < 4) { // Flèche bas
                choix++;
                afficherMenuNiveau(choix); // Mettre à jour uniquement les options
            }
        } else if (touche == '\r') { // Touche Entrée
            break;
        }
    }
    afficherCurseur(); // Réafficher le curseur avant de quitter la boucle

    switch (choix) {
        case 1:
            if (niveau_facile() == -1) {
                afficherCentre("Erreur: Impossible de jouer au niveau facile.");
                system("pause");
            }
            break;
        case 2:
            if (niveau_moyen() == -1) {
                afficherCentre("Erreur: Impossible de jouer au niveau moyen.");
                system("pause");
            }
            break;
        case 3:
            if (niveau_difficile() == -1) {
                afficherCentre("Erreur: Impossible de jouer au niveau difficile.");
                system("pause");
            }
            break;
        case 4:
            afficherCentre("Chargement de la partie sauvegardée non implémenté.");
            system("pause");
            break;
        default:
            afficherCentre("Choix incorrect");
            break;
    }
}