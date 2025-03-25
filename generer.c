#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include "resolution.h"
#include "generer.h"

#define N 9

// Initialise une grille vide
void initialiser_grille(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grille[i][j] = 0;
        }
    }
}

// Vérifie si un nombre peut être placé
bool est_valide(int grille[N][N], int ligne, int colonne, int num) {
    if (grille[ligne][colonne] != 0) {
        return false; // Empêche l'écrasement d'une valeur existante
    }

    for (int i = 0; i < N; i++) {
        if (grille[ligne][i] == num || grille[i][colonne] == num) {
            return false;
        }
    }
    
    int blocLigne = ligne - ligne % 3;
    int blocColonne = colonne - colonne % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[blocLigne + i][blocColonne + j] == num) {
                return false;
            }
        }
    }
    return true;
}

// Génère une grille valide
bool generer_grille_valide(int grille[N][N]) {
    initialiser_grille(grille);

    // Remplit les blocs diagonaux
    for (int bloc = 0; bloc < 3; bloc++) {
        int nombres[9] = {1,2,3,4,5,6,7,8,9};
        
        for (int i = 0; i < 9; i++) {
            int j = rand() % 9;
            int temp = nombres[i];
            nombres[i] = nombres[j];
            nombres[j] = temp;
        }
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grille[bloc*3 + i][bloc*3 + j] = nombres[i*3 + j];
            }
        }
    }

    // Résout la grille complète
    if (!resoudre_sudoku_entree(grille)) {
        return false;
    }

    int cases_vides = N*N - 40;
    while (cases_vides > 0) {
        int ligne = rand() % N;
        int colonne = rand() % N;
        if (grille[ligne][colonne] != 0) {
            grille[ligne][colonne] = 0;
            cases_vides--;
        }
    }
    return true;
}

// Affiche la grille
void afficher_grille(int grille[N][N]) {
    printf("\n    1 2 3   4 5 6   7 8 9\n");
    printf("  +-------+-------+-------+\n");

    for (int i = 0; i < N; i++) {
        printf("%d |", i + 1);
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) {
                printf(" . ");
            } else {
                printf(" %d ", grille[i][j]);
            }
            if ((j + 1) % 3 == 0) printf("|");
        }
        printf("\n");
        if ((i + 1) % 3 == 0) {
            printf("  +-------+-------+-------+\n");
        }
    }
}

// Vérifie si la grille est complète
bool est_complete(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) return false;
        }
    }
    return true;
}

// Vide le buffer d'entrée
void clear_input_buffer() {
    while (getchar() != '\n');
}

// Fonction principale du jeu
void jouer_sudoku(int grille[N][N]) {
    int ligne, colonne, num;
    char action;
    
    while (!est_complete(grille)) {
        afficher_grille(grille);
        
        printf("Menu:\n");
        printf("R - Remplir une case\n");
        printf("S - Resoudre le Sudoku\n");
        printf("Q - Quitter\n");
        printf("Votre choix : ");
        scanf(" %c", &action);
        action = toupper(action);
        clear_input_buffer();

        switch(action) {
            case 'R':
                printf("\nEntrez la ligne, colonne et nombre (1-9) : ");
                if (scanf("%d %d %d", &ligne, &colonne, &num) != 3) {
                    printf("Saisie invalide.\n");
                    clear_input_buffer();
                    continue;
                }
                
                if (ligne < 1 || ligne > 9 || colonne < 1 || colonne > 9 || num < 1 || num > 9) {
                    printf("Valeurs invalides. Doit être entre 1 et 9.\n");
                    continue;
                }
                
                if (grille[ligne-1][colonne-1] != 0) {
                    printf("Case déjà remplie.\n");
                    continue;
                }
                
                if (est_valide(grille, ligne-1, colonne-1, num)) {
                    grille[ligne-1][colonne-1] = num;
                    printf("Nombre placé avec succès!\n");
                } else {
                    printf("Ce nombre n'est pas valide ici!\n");
                }
                break;
                
            case 'S':
                if (resoudre_sudoku_entree(grille)) {
                    printf("Sudoku résolu !\n");
                    afficher_grille(grille);
                    return;
                } else {
                    printf("Impossible de résoudre ce Sudoku.\n");
                }
                break;
                
            case 'Q':
                printf("\nPartie terminée.\n");
                return;
                
            default:
                printf("Option invalide.\n");
        }
    }
    
    printf("\nFélicitations ! Vous avez complété le Sudoku !\n");
}

// Niveau facile
int niveau_facile() {
    srand(time(0));
    int grille[N][N];
    
    if (!generer_grille_valide(grille)) {
        printf("Erreur lors de la génération du Sudoku.\n");
        return 1;
    }
    
    printf("Bienvenue au niveau facile !\n");
    jouer_sudoku(grille);
    return 0;
}
