#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

int main(){

    accueil:

    int user_choice_acceuil;
    int user_choice_niveau;

    menu_acceuil();
    printf("\n");
    printf("votre choix: ");
    scanf("%d", &user_choice_acceuil);
    gerer_menu_acceuil(user_choice_acceuil);
    printf("\n");
    menu_niveau();
    printf("votre choix: ");
    scanf("%d", &user_choice_niveau);
    gerer_menu_niveau(user_choice_niveau);

    
    return 0;
}