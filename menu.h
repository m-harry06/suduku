#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

void menu_acceuil();
void gerer_menu_acceuil();
void connecter_menu();
void addNewPlayers();
void contact_admin_menu();
int getLastId(const char *filename);
void menu_niveau();
void gerer_menu_niveau();
void changerTheme();
void resetThemeBeforeExit();
void Appliquer_Theme_Terminal();
void Enable_Ansi_Support(void);
void effacerEcran();
void afficherLigneSeparation();
#endif