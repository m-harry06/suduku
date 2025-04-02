#include <stdio.h>
#include <locale.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

void afficherCentre(const char *texte) {
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int largeurConsole;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        largeurConsole = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int position = (largeurConsole - strlen(texte)) / 2;
        for (int i = 0; i < position; i++) {
            printf(" ");
        }
    }
    #endif
    printf("%s\n", texte);
}

void exempleFonction() {
    afficherCentre("Message centré dans la console");
}