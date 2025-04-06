#include "afficher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GRID_SIZE 9

#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define WHITE   "\033[37m"
#define RED     "\033[31m"

void afficher_grille(char grille[GRID_SIZE][GRID_SIZE], bool is_user_input[GRID_SIZE][GRID_SIZE], int cursor_row, int cursor_col) {
    printf(BLUE "╔═══════════════════════╗\n" RESET);
    for (int i = 0; i < GRID_SIZE; i++) {
        printf(BLUE "║ " RESET);
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == cursor_row && j == cursor_col) {
                if (grille[i][j] == ' ') {
                    printf(RED "- " RESET);  // Cursor represented by '-' on an empty cell
                } else if (is_user_input[i][j]) {
                    printf(RED "%c " RESET, grille[i][j]);  // User-entered number in red
                } else {
                    printf(GREEN "%c " RESET, grille[i][j]);  // Generated number in green
                }
            } else if (grille[i][j] == ' ') {
                printf(WHITE "  " RESET);  // Empty cell in white
            } else if (is_user_input[i][j]) {
                printf(RED "%c " RESET, grille[i][j]);  // User-entered numbers in red
            } else {
                printf(GREEN "%c " RESET, grille[i][j]);  // Generated numbers in green
            }

            if ((j + 1) % 3 == 0)
                printf(BLUE "║ " RESET);
        }
        printf("\n");
        if ((i + 1) % 3 == 0 && i != GRID_SIZE - 1)
            printf(BLUE "╠═══════════════════════╣\n" RESET);
    }
    printf(BLUE "╚═══════════════════════╝\n" RESET);

    // Display cursor position at the bottom
    printf("Position du curseur: Ligne %d, Colonne %d\n", cursor_row + 1, cursor_col + 1);
}

/* Affiche la grille dans la console */
/*void afficher_grille(char grille[GRID_SIZE][GRID_SIZE]) {
    printf("\n\n      1   2   3    4   5   6    7   8   9\n");
    printf("    +---+---+---+ +---+---+---+ +---+---+---+\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%d   |", i+1);
        for (int j = 0; j < GRID_SIZE; j++) {
            printf(" %c |", grille[i][j] == ' ' ? '.' : grille[i][j]);
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
*/