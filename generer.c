#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <conio.h>
#include "generer.h"
#include "resolution.h"
#include <windows.h>
#include <locale.h>
#include "afficher.h"

#define GRID_SIZE 9

/* Initialise une grille vide (remplie d'espaces) */
void initialiser_grille(char grille[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grille[i][j] = ' ';
        }
    }
}

/* Vérifie si un nombre peut être placé à une position donnée */
bool est_valide(char grille[GRID_SIZE][GRID_SIZE], int ligne, int colonne, char num) {
    for (int i = 0; i < GRID_SIZE; i++) {
        if (grille[ligne][i] == num || grille[i][colonne] == num) {
            return false;
        }
    }
    int bloc_ligne = ligne - ligne % 3;
    int bloc_colonne = colonne - colonne % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[bloc_ligne + i][bloc_colonne + j] == num) {
                return false;
            }
        }
    }
    return true;
}

/* Génère une grille valide et résoluble */
void generer_grille_valide(char grille[GRID_SIZE][GRID_SIZE]) {
    initialiser_grille(grille);
    srand((unsigned int)time(0)); // Initialiser le générateur de nombres aléatoires

    // Étape 1: Remplir les blocs diagonaux (indépendants)
    for (int bloc = 0; bloc < 3; bloc++) {
        char nombres[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
        
        // Mélanger aléatoirement les nombres
        for (int i = 0; i < 9; i++) {
            int j = rand() % 9;
            char temp = nombres[i];
            nombres[i] = nombres[j];
            nombres[j] = temp;
        }
        
        // Remplir le bloc 3x3
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grille[bloc*3 + i][bloc*3 + j] = nombres[i*3 + j];
            }
        }
    }

    // Étape 2: Résoudre complètement la grille
    if (!resoudre_sudoku_entree(grille)) {
        fprintf(stderr, "Erreur: Impossible de résoudre la grille générée.\n");
        return; // Ne pas arrêter brutalement le programme
    }
}

/* Fonction pour nettoyer le terminal */
void nettoyer_terminal() {
    #ifdef _WIN32
        system("cls"); // Commande pour Windows
    #else
        system("clear"); // Commande pour Unix/Linux
    #endif
}

/* Vérifie si la grille est complète */
bool est_complete(char grille[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grille[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

/* Fonction principale de jeu */
void jouer_sudoku(char grille[GRID_SIZE][GRID_SIZE]) {
    int ligne = 0, colonne = 0; // Initial cursor position
    char action, num;
    bool is_user_input[GRID_SIZE][GRID_SIZE] = {false}; // Track user-entered numbers

    while (!est_complete(grille)) {
        nettoyer_terminal(); // Clear the terminal before displaying the grid
        afficher_grille(grille, is_user_input, ligne, colonne);
        printf("Menu:\n");
        printf("Flèches directionnelles - Déplacer le curseur\n");
        printf("Entrer un chiffre (1-9) - Remplir la case\n");
        printf("S - Résoudre le Sudoku\n");
        printf("Q - Quitter\n");
        printf("Votre choix: ");
        action = getch(); // Lire une touche sans appuyer sur Entrée

        switch (action) {
            case 72: // Flèche haut
                if (ligne > 0) ligne--;
                break;
            case 80: // Flèche bas
                if (ligne < GRID_SIZE - 1) ligne++;
                break;
            case 75: // Flèche gauche
                if (colonne > 0) colonne--;
                break;
            case 77: // Flèche droite
                if (colonne < GRID_SIZE - 1) colonne++;
                break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
                num = action;
                if (grille[ligne][colonne] != ' ') {
                    printf("Case déjà remplie. Voulez-vous la modifier? (O/N): ");
                    char choix;
                    scanf(" %c", &choix);
                    if (toupper(choix) != 'O') {
                        continue;
                    }
                }
                if (est_valide(grille, ligne, colonne, num)) {
                    grille[ligne][colonne] = num;
                    is_user_input[ligne][colonne] = true; // Mark as user input
                } else {
                    printf("Ce nombre n'est pas valide ici!\n");
                }
                break;
            case 'S': // Résoudre le Sudoku
                nettoyer_terminal(); // Clear the terminal before solving
                if (resoudre_sudoku_entree(grille)) {
                    printf("Sudoku résolu avec succès!\n");
                    afficher_grille(grille, is_user_input, -1, -1); // No cursor after solving
                    system("pause");
                } else {
                    printf("Impossible de résoudre cette grille.\n");
                }
                return;
            case 'Q': // Quitter
                printf("Vous avez quitté le jeu.\n");
                return;
            default:
                printf("Option invalide.\n");
        }
    }
    nettoyer_terminal(); // Clear the terminal after completion
    printf("\nFélicitations! Vous avez complété le Sudoku!\n");
    afficher_grille(grille, is_user_input, -1, -1); // No cursor after completion
}

/* Point d'entrée pour le niveau facile */
int niveau_facile() {
    char grille[GRID_SIZE][GRID_SIZE];
    char grille_joueur[GRID_SIZE][GRID_SIZE];
    //printf("Génération de la grille pour le niveau facile...\n");
    generer_grille_valide(grille);

    // Créer une copie avec des cases vides
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grille_joueur[i][j] = grille[i][j];
        }
    }

    // Supprimer des cases pour ne garder que 45-50 nombres (niveau facile)
    int cases_a_supprimer = GRID_SIZE * GRID_SIZE - (45 + rand() % 6);
    while (cases_a_supprimer > 0) {
        int ligne = rand() % GRID_SIZE;
        int colonne = rand() % GRID_SIZE;

        if (grille_joueur[ligne][colonne] != ' ') {
            grille_joueur[ligne][colonne] = ' ';
            cases_a_supprimer--;
        }
    }

    //printf("Grille générée pour le niveau facile :\n");
    //afficher_grille(grille_joueur);

    // Vérifier que la grille est résoluble
    char grille_test[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grille_test[i][j] = grille_joueur[i][j];
        }
    }

    if (!resoudre_sudoku_entree(grille_test)) {
        printf("Erreur: Impossible de résoudre la grille générée pour le niveau facile.\n");
        system("pause");
        return -1;
    }

    jouer_sudoku(grille_joueur);
    return 0;
}

/* Point d'entrée pour le niveau moyen */
int niveau_moyen() {
    char grille[GRID_SIZE][GRID_SIZE];
    char grille_joueur[GRID_SIZE][GRID_SIZE]; // Copie pour le joueur
    int tentative = 0;

    do {
        tentative++;
        //printf("Tentative de génération pour le niveau moyen: %d\n", tentative);

        generer_grille_valide(grille);
        
        // Créer une copie pour le joueur
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grille_joueur[i][j] = grille[i][j];
            }
        }

        // Supprimer des cases pour ne garder que 30 nombres
        int cases_a_supprimer = GRID_SIZE * GRID_SIZE - 30;
        while (cases_a_supprimer > 0) {
            int ligne = rand() % GRID_SIZE;
            int colonne = rand() % GRID_SIZE;

            if (grille_joueur[ligne][colonne] != ' ') {
                grille_joueur[ligne][colonne] = ' ';
                cases_a_supprimer--;
            }
        }

        //printf("Grille pour le joueur :\n");
        //afficher_grille(grille_joueur);

        // Vérifier si la grille est résoluble
        char grille_test[GRID_SIZE][GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grille_test[i][j] = grille_joueur[i][j];
            }
        }

        if (resoudre_sudoku_entree(grille_test)) {
            //printf("Grille valide trouvée :\n");
            //afficher_grille(grille_joueur);
            break;
        }

    } while (tentative < 100);

    if (tentative >= 100) {
        printf("Erreur: Impossible de générer une grille valide.\n");
        system("pause");
        return -1;
    }

    jouer_sudoku(grille_joueur);
    return 0;
}

/* Point d'entrée pour le niveau difficile */
int niveau_difficile() {
    char grille[GRID_SIZE][GRID_SIZE];
    char grille_joueur[GRID_SIZE][GRID_SIZE]; // Copie pour le joueur
    int tentative = 0;

    do {
        tentative++;
        //printf("Tentative de génération pour le niveau difficile: %d\n", tentative);

        generer_grille_valide(grille);
        
        // Créer une copie pour le joueur
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grille_joueur[i][j] = grille[i][j];
            }
        }

        // Supprimer des cases pour ne garder que 17 nombres
        int cases_a_supprimer = GRID_SIZE * GRID_SIZE - 17;
        while (cases_a_supprimer > 0) {
            int ligne = rand() % GRID_SIZE;
            int colonne = rand() % GRID_SIZE;

            if (grille_joueur[ligne][colonne] != ' ') {
                grille_joueur[ligne][colonne] = ' ';
                cases_a_supprimer--;
            }
        }

        //printf("Grille pour le joueur :\n");
        //afficher_grille(grille_joueur);

        // Vérifier si la grille est résoluble
        char grille_test[GRID_SIZE][GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grille_test[i][j] = grille_joueur[i][j];
            }
        }

        if (resoudre_sudoku_entree(grille_test)) {
            //printf("Grille valide trouvée :\n");
            //afficher_grille(grille_joueur);
            break;
        }

    } while (tentative < 100);

    if (tentative >= 100) {
        printf("Erreur: Impossible de générer une grille valide.\n");
        system("pause");
        return -1;
    }

    jouer_sudoku(grille_joueur);
    return 0;
}