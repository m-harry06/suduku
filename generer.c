#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include "generer.h"
#include "resolution.h"

/* Initialise une grille vide (remplie de 0) */
void initialiser_grille(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grille[i][j] = 0;
        }
    }
}

/* Vérifie si un nombre peut être placé à une position donnée */
bool est_valide(int grille[N][N], int ligne, int colonne, int num) {
    // Vérification de la ligne et colonne
    for (int i = 0; i < N; i++) {
        if (grille[ligne][i] == num || grille[i][colonne] == num) {
            return false;
        }
    }

    // Vérification du bloc 3x3
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
void generer_grille_valide(int grille[N][N]) {
    initialiser_grille(grille);
    srand(time(0));

    // Étape 1: Remplir les blocs diagonaux (indépendants)
    for (int bloc = 0; bloc < 3; bloc++) {
        int nombres[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        
        // Mélanger aléatoirement les nombres
        for (int i = 0; i < 9; i++) {
            int j = rand() % 9;
            int temp = nombres[i];
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
    resoudre_sudoku_entree(grille);

    // Étape 3: Enlever des nombres pour créer le puzzle
    int cases_vides = N*N - 40; // Niveau facile: 40 cases remplies
    while (cases_vides > 0) {
        int ligne = rand() % N;
        int colonne = rand() % N;
        
        if (grille[ligne][colonne] != 0) {
            grille[ligne][colonne] = 0;
            cases_vides--;
        }
    }
}

/* Affiche la grille dans la console */
void afficher_grille(int grille[N][N]) {
    printf("\n\n      1   2   3    4   5   6    7   8   9\n");
    printf("    +---+---+---+ +---+---+---+ +---+---+---+\n");
    
    for (int i = 0; i < N; i++) {
        printf("%d   |", i+1);
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) {
                printf("   |");
            } else {
                printf(" %d |", grille[i][j]);
            }
            if (j == 2 || j == 5) printf(" |");
        }
        printf("\n");
        if (i == 2 || i == 5) {
            printf("    ++===+===+===++===+===+===++===+===+===++\n");
        } else {
            printf("    +---+---+---+ +---+---+---+ +---+---+---+\n");
        }
    }
    printf("\n");
}

/* Vérifie si la grille est complètement remplie */
bool est_complete(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

/* Fonction principale de jeu */
void jouer_sudoku(int grille[N][N]) {
    int ligne, colonne, num;
    char action;
    
    while (!est_complete(grille)) {
        afficher_grille(grille);
        
        printf("Menu:\n");
        printf("R - Remplir une case\n");
        printf("S - Resoudre le Sudoku\n");
        printf("Q - Quitter\n");
        printf("Votre choix: ");
        scanf(" %c", &action);
        action = toupper(action);
        
        switch(action) {
            case 'R':
                printf("\nEntrez la ligne (1-9), colonne (1-9) et nombre (1-9): ");
                if (scanf("%d %d %d", &ligne, &colonne, &num) != 3) {
                    printf("Saisie invalide.\n");
                    while (getchar() != '\n'); // Vide le buffer
                    continue;
                }
                
                // Validation des entrées
                if (ligne < 1 || ligne > 9 || colonne < 1 || colonne > 9 || num < 1 || num > 9) {
                    printf("Valeurs invalides. Doit etre entre 1 et 9.\n");
                    continue;
                }
                
                // Gestion des cases déjà remplies
                if (grille[ligne-1][colonne-1] != 0) {
                    printf("Case deja remplie. Voulez-vous la modifier? (O/N): ");
                    char choix;
                    scanf(" %c", &choix);
                    if (toupper(choix) != 'O') {
                        continue;
                    }
                }
                
                // Placement du nombre si valide
                if (est_valide(grille, ligne-1, colonne-1, num)) {
                    grille[ligne-1][colonne-1] = num;
                    printf("Nombre place avec succes!\n");
                } else {
                    printf("Ce nombre n'est pas valide ici!\n");
                }
                break;
                
            case 'S':
                printf("\nResolution du Sudoku en cours...\n");
                if (resoudre_sudoku_entree(grille)) {
                    printf("Sudoku resolu avec succes!\n");
                    afficher_grille(grille);
                    system("pause");
                    return;
                } else {
                    printf("Aucune solution trouvee pour cette grille.\n");
                }
                break;
                
            case 'Q':
                printf("\nPartie terminee. Voici votre grille finale:\n");
                afficher_grille(grille);
                return;
                
            default:
                printf("Option invalide.\n");
        }
    }
    
    printf("\nFelicitations! Vous avez complete le Sudoku!\n");
    afficher_grille(grille);
}

/* Point d'entrée pour le niveau facile */
int niveau_facile() {
    int grille[N][N];
    
    printf("\n=== Niveau Facile ===\n");
    printf("Grille generee avec 40 cases remplies.\n\n");
    
    generer_grille_valide(grille);
    jouer_sudoku(grille);
    
    return 0;
}