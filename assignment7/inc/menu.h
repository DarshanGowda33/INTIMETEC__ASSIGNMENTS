#ifndef MENU_H
#define MENU_H

typedef enum
{
    ADD_PLAYER = 1,
    DISPLAY_ALL_PLAYERS,
    DISPLAY_ALL_TEAMS,
    DISPLAY_TOP_K_PLAYERS,
    DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX,
    EXIT
} MenuOptions;

void displayMenu();

#endif
