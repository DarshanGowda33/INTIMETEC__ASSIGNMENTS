#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "menu.h"
#include "player.h"
#include "players_data.h"
#include "list.h"
#include "team.h"
#include "performance.h"
#include "search.h"
#include "sort.h"

#define MAX_TEAMS 10

static Node *head = NULL;
static Team team[MAX_TEAMS];
static int teamsCount = 0;

extern const Players players[];
extern int playerCount;
extern const char* teams[];
extern int teamCount;

static void initializeTeams()
{
    teamsCount = teamCount;
    if (teamsCount > MAX_TEAMS)
    {
        teamsCount = MAX_TEAMS;
    }
    for (int index = 0; index < teamsCount; ++index)
    {
        team[index].teamId = index + 1;
        strncpy(team[index].name, teams[index], sizeof(team[index].name)-1);
        team[index].name[sizeof(team[index].name)-1] = '\0';
        team[index].totalPlayers = 0;
        team[index].avgBattingStrikeRate = 0.0f;
    }
}

static void addingPlayerToTeam(const Player *player)
{
    if (!player)
    {
        return;
    }
    for (int index = 0; index < teamsCount; ++index)
    {
        if (strcmp(team[index].name, player->team) == 0)
        {
            if (team[index].totalPlayers < 50) {
                team[index].players[team[index].totalPlayers++] = *player;
            }
            return;
        }
    }
}

static void loadInitialData()
{
    for (int index = 0; index < playerCount; ++index)
    {
        Player player;
        player.id = players[index].id;

        strncpy(player.name, players[index].name, sizeof(player.name)-1);
        player.name[sizeof(player.name)-1] = '\0';

        strncpy(player.team, players[index].team, sizeof(player.team)-1);
        player.team[sizeof(player.team)-1] = '\0';

        strncpy(player.role, players[index].role, sizeof(player.role)-1);
        player.role[sizeof(player.role)-1] = '\0';

        player.totalRuns = players[index].totalRuns;
        player.battingAverage = players[index].battingAverage;
        player.strikeRate = players[index].strikeRate;
        player.wickets = players[index].wickets;
        player.economyRate = players[index].economyRate;
        player.performanceIndex = computePerformanceIndex(&player);

        head = insertNode(head, player);
        addingPlayerToTeam(&player);
    }

    for (int index = 0; index < teamsCount; ++index)
    {
        computeTeamStrikeRate(&team[index]);
    }
}

static void computeAllTeamsFromList()
{
    for (int index = 0; index < teamsCount; ++index)
    {
        team[index].totalPlayers = 0;
    }
    Node *cur = head;
    while (cur)
    {
        addingPlayerToTeam(&cur->data);
        cur = cur->next;
    }

    for (int index = 0; index < teamsCount; ++index)
    {
        computeTeamStrikeRate(&team[index]);
    }
}

static const char* roleFromChoice(int index)
{
    if (index == 1)
    {
        return "Batsman";
    }
    if (index == 2)
    {
        return "Bowler";
    }
    return "All-rounder";
}


static void addPlayer()
{
    Player player;

    printf("Enter Team ID to add player: ");
    int teamId;
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    int idx = searchTeamById(team, teamsCount, teamId);
    if (idx < 0)
    {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("Enter Player Details:\n");
    printf("Player ID: ");
    if (scanf("%d", &player.id) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    printf("Name: ");
    scanf(" %[^\n]", player.name);

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role;
    if (scanf("%d", &role) != 1)
    {
        printf("Invalid input.\n");
        return;
    }
    strncpy(player.role, roleFromChoice(role), sizeof(player.role)-1);
    player.role[sizeof(player.role)-1] = '\0';

    printf("Total Runs: ");
    if (scanf("%d", &player.totalRuns) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    printf("Batting Average: ");
    if (scanf("%f", &player.battingAverage) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    printf("Strike Rate: ");
    if (scanf("%f", &player.strikeRate) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    printf("Wickets: ");
    if (scanf("%d", &player.wickets) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    printf("Economy Rate: ");
    if (scanf("%f", &player.economyRate) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    strncpy(player.team, team[idx].name, sizeof(player.team) - 1);
    player.team[sizeof(player.team) - 1] = '\0';

    player.performanceIndex = computePerformanceIndex(&player);

    head = insertNode(head, player);
    addingPlayerToTeam(&player);
    computeTeamStrikeRate(&team[idx]);

    printf("Player added successfully to Team %s!\n", team[idx].name);
}

static void displayTeamPlayers()
{
    printf("Enter Team ID: ");
    int teamId;
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    int idx = searchTeamById(team, teamsCount, teamId);
    if (idx < 0)
    {
        printf("Team not found.\n");
        return;
    }

    for (int index = 0; index < teamsCount; ++index)
    {
        team[index].totalPlayers = 0;
    }

    Node *cur = head;
    while (cur)
    {
        addingPlayerToTeam(&cur->data);
        cur = cur->next;
    }

    computeTeamStrikeRate(&team[idx]);

    printTeam(&team[idx]);
    printf("Total Players: %d\n", team[idx].totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team[idx].avgBattingStrikeRate);
}


static void displayTeamsSorted()
{
    for (int index = 0; index < teamsCount; ++index)
    {
        team[index].totalPlayers = 0;
    }

    Node *cur = head;
    while (cur)
    {
        addingPlayerToTeam(&cur->data);
        cur = cur->next;
    }

    for (int index = 0; index < teamsCount; ++index)
    {
        computeTeamStrikeRate(&team[index]);
    }

    Team temp[MAX_TEAMS];
    for (int index = 0; index < teamsCount; ++index)
    {
        temp[index] = team[index];
    }

    sortTeamsByStrikeRate(temp, teamsCount);

    printf("\nTeams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("ID  Team Name             Avg Bat SR  Total Players\n");
    printf("---------------------------------------------------------\n");

    for (int index = 0; index < teamsCount; ++index)
    {
        printf("%-3d %-20s %10.2f %12d\n",
               temp[index].teamId,
               temp[index].name,
               temp[index].avgBattingStrikeRate,
               temp[index].totalPlayers);
    }

    printf("=========================================================\n");
}


static void displayTopKPlayers()
{
    printf("Enter Team ID: ");
    int teamId;
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    int idx = searchTeamById(team, teamsCount, teamId);
    if (idx < 0)
    {
        printf("Team not found.\n");
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int index;
    if (scanf("%d", &index) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    const char *role = roleFromChoice(index);

    Player temp[60];
    int tempCount = 0;

    Node *cur = head;
    while (cur)
    {
        if (strcmp(cur->data.team, team[idx].name) == 0 &&
            strcasecmp(cur->data.role, role) == 0)
        {

            Player player = cur->data;
            player.performanceIndex = computePerformanceIndex(&player);
            temp[tempCount++] = player;
        }
        cur = cur->next;
    }

    if (tempCount == 0)
    {
        printf("No players found.\n");
        return;
    }

    printf("Enter number of players K: ");
    int k;
    if (scanf("%d", &k) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    if (k <= 0)
    {
        printf("K must be positive.\n");
        return;
    }
    if (k > tempCount)
    {
        k = tempCount;
    }

    sortPlayersByPerformance(temp, tempCount);

    printf("\nTop %d %s of Team %s:\n", k, role, team[idx].name);
    printf("================================================================================\n");
    printf("ID    Name                 Role         Runs   Avg    SR    Wkts   ER   PerfIdx\n");
    printf("--------------------------------------------------------------------------------\n");

    for (int index = 0; index < k; ++index)
    {
        printPlayer(&temp[index]);
    }

    printf("================================================================================\n");
}


static void displayPlayersByPerformance(void)
{
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int index;
    if (scanf("%d", &index) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    const char *role = roleFromChoice(index);

    Player tempArr[2000];
    int tempCount = 0;

    Node *cur = head;
    while (cur)
    {
        if (strcasecmp(cur->data.role, role) == 0)
        {

            Player player = cur->data;
            player.performanceIndex = computePerformanceIndex(&player);
            tempArr[tempCount++] = player;
        }
        cur = cur->next;
    }

    if (tempCount == 0)
    {
        printf("No players found for role %s.\n", role); 
        return;
    }

    sortPlayersByPerformance(tempArr, tempCount);

    printf("\nAll %s of all teams sorted by Performance Index:\n", role);
    printf("================================================================================\n");
    printf("ID    Name                 Team         Role         Runs   Avg    SR    Wkts   ER   PerfIdx\n");
    printf("--------------------------------------------------------------------------------\n");

    for (int index = 0; index < tempCount; ++index)
    {
        printPlayer(&tempArr[index]);
    }

    printf("================================================================================\n");
}

int main()
{
    initializeTeams();
    loadInitialData();

    int choice;
    while (1)
    {

        displayMenu();

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 0;
        }

        switch (choice)
        {
            case ADD_PLAYER:
                addPlayer();
                break;
            case DISPLAY_ALL_PLAYERS:
                displayTeamPlayers();
                break;
            case DISPLAY_ALL_TEAMS:
                displayTeamsSorted();
                break;
            case DISPLAY_TOP_K_PLAYERS:
                displayTopKPlayers();
                break;
            case DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX:
                displayPlayersByPerformance();
                break;
            case EXIT:
                printf("Exiting...\n");
                destroyList(head);
                return 0;
            default:
                printf("Invalid choice. Try again later\n");
        }
    }
    return 0;
}
