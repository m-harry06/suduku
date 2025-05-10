#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#include "menu.h"
#include "main.h"
#include "auth.h"
#include "generer.h"
#include "affichageCentrer.h"
#include "sauvegarde.h"
#include "afficher.h"



//char username[20];

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
    appliquer_theme_terminal(); // Ensure the theme is applied globally
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
    appliquer_theme_terminal(); // Ensure the theme is applied globally
    effacerEcran();
    afficherLigneSeparation();
    afficherCentre("*** Menu principal ***");
    afficherLigneSeparation();
    afficherCentre("Utilisez les flèches pour naviguer et Entrée pour sélectionner:");
    afficherCentre(choix == 1 ? "-> 1. Connectez vous a votre espace personnel  " : "   1. Connectez vous a votre espace personnel  ");
    afficherCentre(choix == 2 ? "-> 2. Creer un compte                          " : "   2. Creer un compte                          ");
    afficherCentre(choix == 3 ? "-> 3. Sudoku resolver                          " : "   3. Sudoku resolver                          ");
    afficherCentre(choix == 4 ? "-> 4. Contacter un administrateur              " : "   4. Contacter un administrateur              ");
    afficherCentre(choix == 5 ? " -> 5. Thème                                    " : "    5. Thème                                    ");
    afficherCentre(choix == 6 ? "-> 6. Quitter                                  " : "   6. Quitter                                  ");
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
            } else if (touche == 80 && choix < 6) {
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
            changerTheme(); // Change the theme globally
            gerer_menu_acceuil(); // Return to the main menu
            break;
        case 6:
            resetThemeBeforeExit(); // Reset the theme to default before exiting
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

int getLastId(const char *filename) {
    // Vérification du paramètre filename
    if (filename == NULL || strlen(filename) == 0) {
        fprintf(stderr, "Erreur: Nom de fichier invalide\n");
        return 1; // Retourne 1 comme ID par défaut
    }

    FILE *player_file = fopen(filename, "r");
    if (player_file == NULL) {
        // Fichier inexistant, on retourne le premier ID
        return 1;
    }

    int lastId = 0;
    char line[256]; // Taille augmentée pour plus de sécurité

    while (fgets(line, sizeof(line), player_file)) {
        // Ignorer les lignes vides ou trop courtes
        if (strlen(line) < 2) continue;
        
        // Extraire l'ID (suppose que l'ID est le premier champ avant une virgule)
        int currentId;
        if (sscanf(line, "%d,", &currentId) == 1) {
            if (currentId > lastId) {
                lastId = currentId;
            }
        } else {
            fprintf(stderr, "Avertissement: Ligne mal formatee dans %s: %s", filename, line);
        }
    }

    // Gestion des erreurs de lecture
    if (ferror(player_file)) {
        fprintf(stderr, "Erreur de lecture du fichier %s\n", filename);
        fclose(player_file);
        return lastId + 1; // Retourne quand même une valeur plausible
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

    user.id = getLastId("utilisateurs.csv");
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
        afficherCentre("Entrer votre nom d'utilisateurs (ou tapez '0' pour revenir au menu principal):");
        getchar();
        scanf("%19[^\n]", nom_utilisateur);
        nom_utilisateur[strcspn(nom_utilisateur, "\r\n ")] = 0; // Nettoyer l'entrée utilisateur

        if (strcmp(nom_utilisateur, "0") == 0) {
            gerer_menu_acceuil(); // Retour au menu principal
            return;
        }

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
        password[strcspn(password, "\r\n ")] = 0; // Nettoyer l'entrée utilisateur

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
    afficherCentre(choix == 5 ? "-> 5. Retour au menu principal              " : "   5. Retour au menu principal              ");
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
            } else if (touche == 80 && choix < 5) { // Flèche bas
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
        case 4: {
            char grille[GRID_SIZE][GRID_SIZE];

            chargerSudoku(grille, username) ;
            afficherCentre("Chargement de la partie terminer.");
            system("pause");
            jouer_sudoku(grille);
            break;
        }
        case 5:
            gerer_menu_acceuil(); // Retour au menu principal
            break;
        default:
            afficherCentre("Choix incorrect");
            break;
    }
}

extern int theme_blanc; // Declare the global variable from afficher.c
extern void appliquer_theme_terminal(); // Declare the function to apply the theme

void changerTheme() {
    theme_blanc = !theme_blanc; // Toggle the theme
    appliquer_theme_terminal(); // Apply the theme globally and clear the terminal
    afficherCentre(theme_blanc ? "Thème changé: Fond blanc, texte noir." : "Thème changé: Thème par défaut.");
    system("pause");
}

void resetThemeBeforeExit() {
    theme_blanc = 0; // Reset to default theme (dark mode)
    appliquer_theme_terminal(); // Apply the default theme
}