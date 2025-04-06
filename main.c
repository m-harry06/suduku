#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menu.h"
#include "main.h"

int main(){

    SetConsoleOutputCP(CP_UTF8);

    accueil:

    int user_choice_acceuil;
    int user_choice_niveau;
    char grille[9][9];

    
    gerer_menu_acceuil(user_choice_acceuil);
    gerer_menu_niveau(user_choice_niveau);
    

    goto accueil;
    return 0;
}