#include "afficher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>  // Pour GetConsoleScreenBufferInfo

#define GRID_SIZE 9

#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define WHITE   "\033[37m"
#define RED     "\033[31m"

// Calcule la marge de gauche pour centrer la grille
int marge_gauche(int largeur_texte) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns = 80; // Valeur par défaut

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    return (columns - largeur_texte) / 2;
}

void afficher_grille(char grille[GRID_SIZE][GRID_SIZE], bool is_user_input[GRID_SIZE][GRID_SIZE], int cursor_row, int cursor_col) {
    const int largeur_texte = 27; // largeur de la grille en caractères (╔══...══╗)

    int marge = marge_gauche(largeur_texte);
    for (int i = 0; i < marge; i++) printf(" ");
    printf(BLUE "╔═══════════════════════╗\n" RESET);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int k = 0; k < marge; k++) printf(" ");
        printf(BLUE "║ " RESET);
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == cursor_row && j == cursor_col) {
                if (grille[i][j] == ' ') {
                    printf(RED "- " RESET);
                } else if (is_user_input[i][j]) {
                    printf(RED "%c " RESET, grille[i][j]);
                } else {
                    printf(GREEN "%c " RESET, grille[i][j]);
                }
            } else if (grille[i][j] == ' ') {
                printf(WHITE "  " RESET);
            } else if (is_user_input[i][j]) {
                printf(RED "%c " RESET, grille[i][j]);
            } else {
                printf(GREEN "%c " RESET, grille[i][j]);
            }

            if ((j + 1) % 3 == 0)
                printf(BLUE "║ " RESET);
        }
        printf("\n");

        if ((i + 1) % 3 == 0 && i != GRID_SIZE - 1) {
            for (int k = 0; k < marge; k++) printf(" ");
            printf(BLUE "╠═══════════════════════╣\n" RESET);
        }
    }

    for (int i = 0; i < marge; i++) printf(" ");
    printf(BLUE "╚═══════════════════════╝\n" RESET);

    // Info curseur
    printf("\n");
    for (int i = 0; i < marge; i++) printf(" ");
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
