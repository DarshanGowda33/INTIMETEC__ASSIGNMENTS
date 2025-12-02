#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "operations.h"

void printPlayer(const Player *player)
{
    if (player == NULL)
    {
        return;
    }
    printf("%-5d %-20s %-12s %-12s %6d %6.2f %6.2f %6d %6.2f %8.2f\n",
           player->id,
           player->name,
           player->team,
           player->role,
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

Node* insertNode(Node *head, const Player player)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode)
    {
        return head;
    }
    newNode->data = player;
    newNode->next = head;
    return newNode;
}

void destroyList(Node *head)
{
    Node *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void printList(const Node *head)
{
    while (head != NULL)
    {
        printPlayer(&head->data);
        head = head->next;
    }
}

void computeTeamStrikeRate(Team *team)
{
    if (!team)
    {
        return;
    }
    int count = 0;
    float totalStrikeRate = 0.0f;
    for (int index = 0; index < team->totalPlayers; ++index)
    {
        const char *role = team->players[index].role;
        if (strcasecmp(role, "Batsman") == 0 || strcasecmp(role, "All-rounder") == 0)
        {
            totalStrikeRate += team->players[index].strikeRate;
            count++;
        }
    }
    team->avgBattingStrikeRate = (count > 0) ? (totalStrikeRate / count) : 0.0f;
}

void printTeam(const Team *team)
{
    if (!team)
    {
        return;
    }
    printf("\nPlayers of Team %s:\n", team->name);
    printf("===================================================================================\n");
    printf("%-6s %-20s %-12s %-6s %-6s %-6s %-6s %-5s %-8s\n",
           "ID","Name","Role","Runs","Avg","SR","Wkts","ER","Perf.Index");
    printf("-----------------------------------------------------------------------------------\n");
    for (int index = 0; index < team->totalPlayers; ++index)
    {
        printPlayer(&team->players[index]);
    }
    printf("===================================================================================\n");
}

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

static int ignoreCase(const char *ch1, const char *ch2)
{
    if (!ch1 || !ch2)
    {
        return 0;
    }
    while (*ch1 && *ch2)
    {
        char charA = *ch1 >= 'A' && *ch1 <= 'Z' ? *ch1 + ('a' - 'A') : *ch1;
        char charB = *ch2 >= 'A' && *ch2 <= 'Z' ? *ch2 + ('a' - 'A') : *ch2;
        if (charA != charB)
        {
            return 0;
        }
        ch1++;
        ch2++;
    }
    return (*ch1 == '\0' && *ch2 == '\0');
}

float computePerformanceIndex(const Player *player)
{
    if (!player)
    {
        return 0.0f;
    }

    if (ignoreCase(player->role, "Batsman"))
    {
        return (player->battingAverage * player->strikeRate) / 100.0f;
    }
    else if (ignoreCase(player->role, "Bowler"))
    {
        return (player->wickets * 2.0f) + (100.0f - player->economyRate);
    }
    else
    {
        float batPart = (player->battingAverage * player->strikeRate) / 100.0f;
        float bowlPart = (player->wickets * 2.0f);
        return batPart + bowlPart;
    }
}

void computePerformanceForAll(Player players[], int playersCount)
{
    if (!players)
    {
        return;
    }
    for (int index = 0; index < playersCount; ++index)
    {
        players[index].performanceIndex = computePerformanceIndex(&players[index]);
    }
}

int searchTeamById(const Team teams[], int teamsCount, int teamId)
{
    int low = 0, high = teamsCount - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (teams[mid].teamId == teamId)
        {
            return mid;
        }
        if (teams[mid].teamId < teamId)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

static void mergeTeams(Team teams[], Team temp[], int left, int mid, int right)
{
    int i = left, j = mid, k = left;
    while (i < mid && j <= right)
    {
        if (teams[i].avgBattingStrikeRate >= teams[j].avgBattingStrikeRate)
        {
            temp[k++] = teams[i++];
        }
        else
        {
            temp[k++] = teams[j++];
        }
    }
    while (i < mid)
    {
        temp[k++] = teams[i++];
    }
    while (j <= right)
    {
        temp[k++] = teams[j++];
    }
    for (i = left; i <= right; ++i)
    {
        teams[i] = temp[i];
    }
}

static void mergeSortTeams(Team teams[], Team temp[], int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSortTeams(teams, temp, left, mid);
    mergeSortTeams(teams, temp, mid+1, right);
    mergeTeams(teams, temp, left, mid+1, right);
}

void sortTeamsByStrikeRate(Team teams[], int teamsCount)
{
    if (!teams || teamsCount <= 1)
    {
        return;
    }
    Team *temp = (Team*)malloc(sizeof(Team) * teamsCount);
    if (!temp)
    {
        return;
    }
    mergeSortTeams(teams, temp, 0, teamsCount - 1);
    free(temp);
}

static void mergePlayers(Player players[], Player temp[], int left, int mid, int right)
{
    int i = left, j = mid, k = left;
    while (i < mid && j <= right)
    {
        if (players[i].performanceIndex >= players[j].performanceIndex)
        {
            temp[k++] = players[i++];
        }
        else
        {
            temp[k++] = players[j++];
        }
    }
    while (i < mid)
    {
        temp[k++] = players[i++];
    }
    while (j <= right)
    {
        temp[k++] = players[j++];
    }
    for (i = left; i <= right; ++i)
    {
        players[i] = temp[i];
    }
}

static void mergeSortPlayers(Player players[], Player temp[], int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSortPlayers(players, temp, left, mid);
    mergeSortPlayers(players, temp, mid+1, right);
    mergePlayers(players, temp, left, mid+1, right);
}

void sortPlayersByPerformance(Player players[], int playersCount)
{
    if (!players || playersCount <= 1)
    {
        return;
    }
    Player *temp = (Player*)malloc(sizeof(Player) * playersCount);
    if (!temp)
    {
        return;
    }
    mergeSortPlayers(players, temp, 0, playersCount - 1);
    free(temp);
    temp = NULL;
}
