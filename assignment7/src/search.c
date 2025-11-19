#include <string.h>
#include <strings.h>
#include "search.h"
#include "player.h"
#include "team.h"

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
