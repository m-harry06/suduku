#ifndef SAUVEGARDE_H_INCLUDED
#define SAUVEGARDE_H_INCLUDED

void sauvegarderSudoku(char grille[9][9], const char *nomUtilisateur);
void afficherCentre(const char *texte);
void chargerSudoku(char grille[9][9], const char *nomUtilisateur);

#endif