#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menu.h"
#include "main.h"

// Définition de la variable globale
char username[20] = ""; // Initialisez avec une chaîne vide ou une valeur par défaut

int main(){

    SetConsoleOutputCP(CP_UTF8);

    int user_choice_acceuil;
    int user_choice_niveau;
    char grille[9][9];

    
    gerer_menu_acceuil(user_choice_acceuil);
    accueil:
    gerer_menu_niveau(user_choice_niveau);
    

    goto accueil;
    return 0;
}