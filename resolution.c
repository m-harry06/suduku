#include <stdio.h>
#include <stdbool.h>

#define GRID_SIZE 9
#define MAX_RECURSION 1000

bool est_valide(char grille[GRID_SIZE][GRID_SIZE], int ligne, int colonne, char num) {
    // Vérifie ligne et colonne
    for (int i = 0; i < GRID_SIZE; i++) {
        if (grille[ligne][i] == num || grille[i][colonne] == num) {
            return false;
        }
    }

    // Vérifie bloc 3x3
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

bool resoudre_sudoku(char grille[GRID_SIZE][GRID_SIZE], int depth) {
    if (depth > MAX_RECURSION) {
        fprintf(stderr, "Profondeur maximale atteinte\n");
        return false;
    }

    for (int ligne = 0; ligne < GRID_SIZE; ligne++) {
        for (int colonne = 0; colonne < GRID_SIZE; colonne++) {
            if (grille[ligne][colonne] == ' ') {
                for (char num = '1'; num <= '9'; num++) {
                    if (est_valide(grille, ligne, colonne, num)) {
                        grille[ligne][colonne] = num;
                        
                        if (resoudre_sudoku(grille, depth + 1)) {
                            return true;
                        }
                        
                        grille[ligne][colonne] = ' ';
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool resoudre_sudoku_entree(char grille[GRID_SIZE][GRID_SIZE]) {
    char copie[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            copie[i][j] = grille[i][j];
        }
    }

    bool resolu = resoudre_sudoku(copie, 0);
    
    if (resolu) {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grille[i][j] = copie[i][j];
            }
        }
    }
    
    return resolu;
}