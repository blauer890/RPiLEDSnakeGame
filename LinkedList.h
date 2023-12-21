#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

struct Node
{
    int x;
    int y;
    struct Node* next;
};

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DONE
};

void addNode(struct Node **head, int x, int y);
void destroyList(struct Node **head);
bool nodeMatch(struct Node* head, int x, int y);
bool moveHead(struct Node** head, enum Direction direction);
void printList(struct Node* head);

#endif
