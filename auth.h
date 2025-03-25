#ifndef AUTH_H_INCLUDED
#define AUTH_H_INCLUDED

int authentification(const char *nom_utilisateur,const char *password);

#endif
/*
void afficher_grille(int grille[N][N]) {
    printf("    1     2    3    4    5    6    7    8    9");
    printf("\n  +----+----+----+----+----+----+----+----+----+\n");
    for (int i = 0; i < N; i++) {
        printf("%d |", i+1);
        for (int j = 0; j < N; j++) {
        
            if (grille[i][j] == 0) {
                printf("    |"); // Afficher un espace pour les cases vides
            } else {
                printf(" %2d |", grille[i][j]);
            }
            
        }
        printf("\n  +----+----+----+----+----+----+----+----+----+\n");
    }
}
*/