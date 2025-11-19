#include <stdlib.h>
#include <string.h>
#include "sort.h"
#include "team.h"
#include "player.h"

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
