#ifndef OPERATIONS_H
#define OPERATIONS_H

typedef struct Player
{
    int id;
    char name[50];
    char team[50];
    char role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} Player;

typedef struct Node
{
    Player data;
    struct Node *next;
} Node;

typedef struct Team
{
    int teamId;
    char name[50];
    Player players[50];
    int totalPlayers;
    float avgBattingStrikeRate;
} Team;

typedef enum
{
    ADD_PLAYER = 1,
    DISPLAY_ALL_PLAYERS,
    DISPLAY_ALL_TEAMS,
    DISPLAY_TOP_K_PLAYERS,
    DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX,
    EXIT
} MenuOptions;

void printPlayer(const Player *player);

Node* insertNode(Node *head, const Player player);
void destroyList(Node *head);
void printList(const Node *head);

void computeTeamStrikeRate(Team *team);
void printTeam(const Team *team);

void displayMenu();

float computePerformanceIndex(const Player *player);
void computePerformanceForAll(Player players[], int playersCount);

int searchTeamById(const Team teams[], int teamsCount, int teamId);

void sortTeamsByStrikeRate(Team teams[], int teamsCount);
void sortPlayersByPerformance(Player players[], int playersCount);

#endif
