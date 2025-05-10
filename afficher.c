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
    const char *input = "\033[31m"; // rouge pour utilisateur (toujours)
    const char *border;
    const char *cursor_fg = "\033[35m"; // violet pour curseur

    if (theme_blanc) {
        bg = "\033[47m";      // fond blanc
        fixed = "\033[30m";   // texte noir
        border = "\033[30m";  // bordure noire
    } else {
        bg = "";              // fond par défaut terminal
        fixed = "\033[37m";   // texte blanc
        border = "\033[37m";  // bordure blanche
    }

    const int largeur_texte = 27;
    int marge = marge_gauche(largeur_texte);

    // Bordure supérieure
    for (int i = 0; i < marge; i++) printf("%s %s", bg, bg);
    printf("%s%s╔═══════════════════════╗%s\n", bg, border, reset);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < marge; j++) printf("%s %s", bg, bg);
        printf("%s%s║ %s", bg, border, bg);

        for (int j = 0; j < GRID_SIZE; j++) {
            char c = grille[i][j];

            if (i == cursor_row && j == cursor_col) {
                // Curseur : juste texte rouge, pas de fond spécial
                if (c == ' ') {
                    printf("%s%s-%s ", bg, cursor_fg, bg);
                } else if (is_user_input[i][j]) {
                    printf("%s%s%c%s ", bg, input, c, bg);
                } else {
                    printf("%s%s%c%s ", bg, cursor_fg, c, bg);
                }
            } else if (c == ' ') {
                printf("%s  %s", bg, bg);
            } else if (is_user_input[i][j]) {
                printf("%s%s%c%s ", bg, input, c, bg);
            } else {
                printf("%s%s%c%s ", bg, fixed, c, bg);
            }

            if ((j + 1) % 3 == 0)
                printf("%s%s║ %s", bg, border, bg);
        }

        printf("%s\n", reset);

        if ((i + 1) % 3 == 0 && i != GRID_SIZE - 1) {
            for (int j = 0; j < marge; j++) printf("%s %s", bg, bg);
            printf("%s%s╠═══════════════════════╣%s\n", bg, border, reset);
        }
    }

    // Bordure inférieure
    for (int i = 0; i < marge; i++) printf("%s %s", bg, bg);
    printf("%s%s╚═══════════════════════╝%s\n", bg, border, reset);

    // Affichage position curseur
    printf("\n");
    for (int i = 0; i < marge; i++) printf("%s %s", bg, bg);
    printf("%s%sPosition du curseur: Ligne %d, Colonne %d%s\n",
           bg, border, cursor_row + 1, cursor_col + 1, reset);
           
    // Effacer complètement le thème à la fin
    printf("%s", reset);
}