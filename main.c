#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menu.h"
#include "main.h"

// Définition de la variable globale
char username[20] = ""; // Chaîne initialisée vide

// Active les séquences ANSI pour le thème
void activer_support_ANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}

int main() {
    // Affiche les caractères UTF-8 correctement
    SetConsoleOutputCP(CP_UTF8);

    // Active les couleurs ANSI
    activer_support_ANSI();
    
    niveau :
    // Lancement du menu principal (accueil)
    gerer_menu_acceuil();  // plus besoin de passer de pointeur si c’est géré en interne
    gerer_menu_niveau();

    goto niveau ;
    // Fin du programme : on peut réinitialiser le thème ici aussi au cas où
    resetThemeBeforeExit();

    return 0;
}
