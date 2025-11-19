#include <stdio.h>
#include "menu.h"

void displayMenu()
{
    printf("===============================================================================");
    printf("\n                ICC ODI Performance Analyzer                 \n");
    printf("===============================================================================\n");
    printf("%d. Add player to Team.\n", ADD_PLAYER);
    printf("%d. Display Players of a Specific Team.\n", DISPLAY_ALL_PLAYERS);
    printf("%d. Display Teams by Average Batting Strike Rate.\n", DISPLAY_ALL_TEAMS);
    printf("%d. Display Top K Players of a Specific Team by Role.\n", DISPLAY_TOP_K_PLAYERS);
    printf("%d. Display all Players of specific role Across All Teams by Performance Index.\n", DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX);
    printf("%d. Exit.\n", EXIT);
    printf("===============================================================================\n");
    printf("Enter your choice: ");
}
