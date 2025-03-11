#include "afficher.h"
#include <stdio.h>


void afficher(char grille[9][9]) {

    printf("     1  2  3   4  5  6   7  8  9\n");
    printf("   +---------+---------+---------+\n");

    for (int i = 0; i < 9; i++) {
        
        printf("%d  |", i + 1);

        for (int j = 0; j < 9; j++) {
            
            if (grille[i][j] == '0') {
                printf(" 0 ");
            } else if (grille[i][j] == '\0' || grille[i][j] == ' ') {
                printf("   ");
            } else {
                printf(" %c ", grille[i][j]);
            }

            if ((j + 1) % 3 == 0) {
                printf("|");
            }
        }
        printf("\n");

        if ((i + 1) % 3 == 0) {
            printf("   +---------+---------+---------+\n");
        }
    }
}
