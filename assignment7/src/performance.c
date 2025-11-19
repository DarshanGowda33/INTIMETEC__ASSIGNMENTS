#include <string.h>
#include "performance.h"

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
