#ifndef AFFICHER_H_INCLUDED
#define AFFICHER_H_INCLUDED

#include <stdbool.h>

#define GRID_SIZE 9

void appliquer_theme_terminal(); // Declare the function to apply the theme globally
void afficher_grille(char grille[GRID_SIZE][GRID_SIZE], bool is_user_input[GRID_SIZE][GRID_SIZE], int cursor_row, int cursor_col);

#endif