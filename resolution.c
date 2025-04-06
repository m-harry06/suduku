#include <stdio.h>
#include <stdbool.h>

#define GRID_SIZE 9

bool est_sauve(int grille[GRID_SIZE][GRID_SIZE], int ligne, int colonne, int num) {
    // Vérification ligne et colonne optimisée
    for (int i = 0; i < GRID_SIZE; i++) {
        if (grille[ligne][i] == num) return false;
        if (grille[i][colonne] == num) return false;
    }

    // Vérification bloc 3x3 plus précise
    int bloc_ligne = (ligne / 3) * 3;
    int bloc_colonne = (colonne / 3) * 3;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[bloc_ligne + i][bloc_colonne + j] == num) {
                return false;
            }
        }
    }
    return true;
}

bool resoudre_sudoku(int grille[GRID_SIZE][GRID_SIZE], int position) {
    // Nouvelle approche : parcours linéaire de 0 à 80
    if (position >= GRID_SIZE * GRID_SIZE) return true;
    
    int ligne = position / GRID_SIZE;
    int colonne = position % GRID_SIZE;

    // Case déjà remplie -> passer à la suivante
    if (grille[ligne][colonne] != 0) {
        return resoudre_sudoku(grille, position + 1);
    }

    // Essai systématique des valeurs 1-9
    for (int num = 1; num <= GRID_SIZE; num++) {
        if (est_sauve(grille, ligne, colonne, num)) {
            grille[ligne][colonne] = num;
            
            if (resoudre_sudoku(grille, position + 1)) {
                return true;
            }
            
            // Backtrack
            grille[ligne][colonne] = 0;
        }
    }
    return false;
}

/* Version d'appel simplifiée */
bool resoudre_sudoku_entree(int grille[GRID_SIZE][GRID_SIZE]) {
    return resoudre_sudoku(grille, 0);
}