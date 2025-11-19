#include <stdlib.h>
#include <string.h>
#include "list.h"


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
