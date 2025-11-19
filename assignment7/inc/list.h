#ifndef LIST_H
#define LIST_H

#include "player.h"

typedef struct Node
{
    Player data;
    struct Node *next;
} Node;

Node* insertNode(Node *head, const Player player);
void destroyList(Node *head);
void printList(const Node *head);

#endif
