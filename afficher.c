#include "afficher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define GRID_SIZE 9

int theme_blanc = 0; // 0 = sombre, 1 = clair

int marge_gauche(int largeur_texte) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return (columns - largeur_texte) / 2;
}

void enable_ansi_support() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void appliquer_theme_terminal() {
    enable_ansi_support();
    // Effacer l'écran d'abord
    printf("\033[2J\033[H");
    
    // Puis appliquer le thème si nécessaire
    if (theme_blanc) {
        // Version corrigée pour VS Code: appliquer fond blanc partout
        printf("\033[47m"); // fond blanc
    }
}

void afficher_grille(char grille[GRID_SIZE][GRID_SIZE], bool is_user_input[GRID_SIZE][GRID_SIZE], int cursor_row, int cursor_col) {
    appliquer_theme_terminal();

    const char *reset = "\033[0m";
    const char *bg;
    const char *fixed;
    const char *input = "\033[31m"; // rouge
    const char *border;
    const char *cursor_fg = "\033[35m"; // violet

    if (theme_blanc) {
        bg = "\033[47m";
        fixed = "\033[30m";
        border = "\033[30m";
    } else {
        bg = "";
        fixed = "\033[37m";
        border = "\033[37m";
    }

    const int largeur_texte = 37; // Ajusté pour 9 chiffres + séparateurs
    int marge = marge_gauche(largeur_texte);

    // Bordure supérieure
    for (int i = 0; i < marge; i++) printf("%s %s", bg, bg);
    printf("%s%s╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗%s\n", bg, border, reset);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < marge; j++) printf("%s %s", bg, bg);
        printf("%s%s║", bg, border);

        for (int j = 0; j < GRID_SIZE; j++) {
            char c = grille[i][j];

            printf(" ");
            if (i == cursor_row && j == cursor_col) {
                if (c == ' ') printf("%s%s-%s", bg, cursor_fg, bg);
                else if (is_user_input[i][j]) printf("%s%s%c%s", bg, input, c, bg);
                else printf("%s%s%c%s", bg, cursor_fg, c, bg);
            } else if (c == ' ') {
                printf(" ");
            } else if (is_user_input[i][j]) {
                printf("%s%s%c%s", bg, input, c, bg);
            } else {
                printf("%s%s%c%s", bg, fixed, c, bg);
            }
            printf(" ");

            // Barres verticales
            if (j == 2 || j == 5) printf("%s│", border);
            else if (j != 8) printf("%s│", border);
        }

        printf("%s║%s\n", border, reset);

        // Barres horizontales
        if (i == 2 || i == 5) {
            for (int j = 0; j < marge; j++) printf("%s %s", bg, bg);
            printf("%s%s╟───┼───┼───╫───┼───┼───╫───┼───┼───╢%s\n", bg, border, reset);
        } else if (i != 8) {
            for (int j = 0; j < marge; j++) printf("%s %s", bg, bg);
            printf("%s%s╟───┼───┼───┼───┼───┼───┼───┼───┼───╢%s\n", bg, border, reset);
        }
    }

    // Bordure inférieure
    for (int i = 0; i < marge; i++) printf("%s %s", bg, bg);
    printf("%s%s╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝%s\n", bg, border, reset);

    // Info curseur
    printf("\n");
    for (int i = 0; i < marge; i++) printf("%s %s", bg, bg);
    printf("%s%sPosition du curseur: Ligne %d, Colonne %d%s\n",
           bg, border, cursor_row + 1, cursor_col + 1, reset);

    printf("%s", reset);
}
