#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include "generer.h"
#include "resolution.h"
#include "afficher.h"
#include "sauvegarde.h"
#include "main.h" // Include to access the global username variable

#define GRID_SIZE 9
#define MAX_ATTEMPTS 1000

/* Initialise une grille vide */
void initialiser_grille(char grille[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grille[i][j] = ' ';
        }
    }
}

/* Génère une grille valide */
void generer_grille_valide(char grille[GRID_SIZE][GRID_SIZE]) {
    initialiser_grille(grille);
    srand((unsigned int)time(NULL));

    // Remplir les blocs diagonaux
    for (int bloc = 0; bloc < 3; bloc++) {
        char nombres[9] = {'1','2','3','4','5','6','7','8','9'};
        
        // Mélange Fisher-Yates
        for (int i = 8; i > 0; i--) {
            int j = rand() % (i + 1);
            char temp = nombres[i];
            nombres[i] = nombres[j];
            nombres[j] = temp;
        }
        
        // Remplissage du bloc
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grille[bloc*3 + i][bloc*3 + j] = nombres[i*3 + j];
            }
        }
    }

    if (!resoudre_sudoku_entree(grille)) {
        fprintf(stderr, "Erreur: Résolution échouée\n");
        exit(EXIT_FAILURE);
    }
}

/* Fonction pour créer un niveau */
int creer_niveau(int nombres_visibles) {
    char grille[GRID_SIZE][GRID_SIZE];
    char grille_joueur[GRID_SIZE][GRID_SIZE];
    bool is_user_input[GRID_SIZE][GRID_SIZE] = {false};

    generer_grille_valide(grille);
    copier_grille(grille, grille_joueur);

    int cases_a_supprimer = GRID_SIZE * GRID_SIZE - nombres_visibles;
    int attempts = 0;

    while (cases_a_supprimer > 0 && attempts++ < MAX_ATTEMPTS) {
        int ligne = rand() % GRID_SIZE;
        int colonne = rand() % GRID_SIZE;

        if (grille_joueur[ligne][colonne] != ' ') {
            char temp = grille_joueur[ligne][colonne];
            grille_joueur[ligne][colonne] = ' ';
            
            char grille_test[GRID_SIZE][GRID_SIZE];
            copier_grille(grille_joueur, grille_test);
            
            if (resoudre_sudoku_entree(grille_test)) {
                cases_a_supprimer--;
            } else {
                grille_joueur[ligne][colonne] = temp;
            }
        }
    }

    if (attempts >= MAX_ATTEMPTS) {
        fprintf(stderr, "Erreur: Impossible de créer le niveau\n");
        return -1;
    }

    jouer_sudoku(grille_joueur);
    return 0;
}

/* Niveaux de difficulté */
int niveau_facile() { return creer_niveau(45 + rand() % 6); }
int niveau_moyen() { return creer_niveau(30); }
int niveau_difficile() { return creer_niveau(17); }

// Ajoutez ceci à la fin de votre fichier generer.c

void copier_grille(char source[GRID_SIZE][GRID_SIZE], char destination[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

bool est_complete(char grille[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grille[i][j] == ' ') return false;
        }
    }
    return true;
}

bool confirmer_remplacement() {
    printf("Case déjà remplie. Remplacer? (O/N): ");
    char rep = getchar();
    while(getchar() != '\n');
    return (toupper(rep) == 'O');
}

void jouer_sudoku(char grille[GRID_SIZE][GRID_SIZE]) {
    int ligne = 0, colonne = 0;
    char action, num;
    bool is_user_input[GRID_SIZE][GRID_SIZE] = {false};

    while (!est_complete(grille)) {
        system("cls");
        afficher_grille(grille, is_user_input, ligne, colonne);
        printf("Menu:\n");
        printf("Flèches directionnelles - Déplacer le curseur\n");
        printf("Entrer un chiffre (1-9) - Remplir la case\n");
        printf("S - Résoudre le Sudoku\n");
        printf("R - Supprimer un nombre rouge\n");
        printf("G - Sauvegarder la partie\n");
        printf("Q - Quitter\n");
        printf("Votre choix: ");
        action = getch();

        switch (action) {
            case 72: if (ligne > 0) ligne--; break;
            case 80: if (ligne < GRID_SIZE-1) ligne++; break;
            case 75: if (colonne > 0) colonne--; break;
            case 77: if (colonne < GRID_SIZE-1) colonne++; break;
            case '1'...'9':
                num = action;
                if (grille[ligne][colonne] == ' ' || 
                   (grille[ligne][colonne] != ' ' && confirmer_remplacement())) {
                    if (est_valide(grille, ligne, colonne, num)) {
                        grille[ligne][colonne] = num;
                        is_user_input[ligne][colonne] = true;
                    }
                }
                break;
            case 'R': case 'r':
                if (is_user_input[ligne][colonne] && grille[ligne][colonne] != ' ') {
                    grille[ligne][colonne] = ' ';  // Remove the user-entered number
                    is_user_input[ligne][colonne] = false;
                } else {
                    printf("Impossible de supprimer cette case.\n");
                    Sleep(1000);  // Pause briefly to show the message
                }
                break;
            case 'S': case 's': 
                if (resoudre_sudoku_entree(grille)) {
                    system("cls");
                    afficher_grille(grille, is_user_input, -1, -1); // Display solved grid
                    printf("La grille a été résolue avec succès!\n");
                    system("pause");
                    return;
                } else {
                    printf("Impossible de résoudre la grille.\n");
                }
                break;
            case 'G': case 'g':
                sauvegarderSudoku(grille, username); // Use the global username variable
                Sleep(1000);  // Pause briefly to confirm save
                break;
            case 'Q': return;
        }
    }
    printf("Félicitations! Vous avez gagné!\n");
    system("pause");
}