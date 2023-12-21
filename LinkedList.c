#include <stddef.h>
#include <stdlib.h>
#include <malloc.h>

#include "LinkedList.h"

void addNode(struct Node **head, int x, int y)
{
    struct Node *temp = NULL;
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;

    if(*head == NULL)
    {
        *head = newNode;
        return;
    }

    if((*head)->next == NULL)
    {
        (*head)->next = newNode;
        return;
    }

    temp = *head;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

void destroyList(struct Node **head)
{
    struct Node* currNode = NULL;
    struct Node* nextNode = NULL;

    if(*head == NULL)
    {
        return;
    }

    if((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }

    currNode = *head;
    nextNode = (*head)->next;
    while(nextNode != NULL)
    {
        free(currNode);
        currNode = NULL;
        currNode = nextNode;
        nextNode = currNode->next;
    }
}

bool nodeMatch(struct Node* head, int x, int y)
{
    struct Node* temp = NULL;

    // Empty linked list
    if(head == NULL)
    {
        return false;
    }

    // Only one element in the linked list
    if(head->next == NULL)
    {
        return ((head->x == x) &&
                (head->y == y));
    }

    // More than one node in the linked list
    temp = head;
    while(temp->next != NULL)
    {
        if((temp->x == x) &&
           (temp->y == y))
        {
            return true;
        }
        else // carry on
        {
            temp = temp->next;
        }
    }
    return ((temp->x == x) &&
            (temp->y == y));
}

bool moveHead(struct Node** head, enum Direction direction)
{
    struct Node* temp = NULL;

    struct Node currNode;
    struct Node prevNode;

    if(*head == NULL)
    {
        return false;
    }

    switch(direction)
    {
    case UP:
        if(((*head)->y - 1) < 0)
        {
            return false;
        }

        if(nodeMatch(*head, (*head)->x, (*head)->y - 1))
        {
            return false;
        }

        if((*head)->next == NULL)
        {
            (*head)->y--;
            return true;
        }

        // Save off the head data before modifying
        prevNode.x = (*head)->x;
        prevNode.y = (*head)->y;
        (*head)->y--;
        currNode.x = (*head)->next->x;
        currNode.y = (*head)->next->y;
        temp = (*head)->next;
        temp->x = prevNode.x;
        temp->y = prevNode.y;
        prevNode.x = currNode.x;
        prevNode.y = currNode.y;

        while(temp->next != NULL)
        {
            temp = temp->next;
            currNode.x = temp->x;
            currNode.y = temp->y;
            temp->x = prevNode.x;
            temp->y = prevNode.y;
            prevNode.x = currNode.x;
            prevNode.y = currNode.y;
        }
        return true;
    case DOWN:
        if(((*head)->y + 1) > 7)
        {
            return false;
        }

        if(nodeMatch(*head, (*head)->x, (*head)->y + 1))
        {
            return false;
        }

        if((*head)->next == NULL)
        {
            (*head)->y++;
            return true;
        }

        prevNode.x = (*head)->x;
        prevNode.y = (*head)->y;
        (*head)->y++;
        currNode.x = (*head)->next->x;
        currNode.y = (*head)->next->y;
        temp = (*head)->next;
        temp->x = prevNode.x;
        temp->y = prevNode.y;
        prevNode.x = currNode.x;
        prevNode.y = currNode.y;

        while(temp->next != NULL)
        {
            temp = temp->next;
            currNode.x = temp->x;
            currNode.y = temp->y;
            temp->x = prevNode.x;
            temp->y = prevNode.y;
            prevNode.x = currNode.x;
            prevNode.y = currNode.y;
        }
        return true;
    case LEFT:
        if(((*head)->x - 1) < 0)
        {
            return false;
        }

        if(nodeMatch(*head, (*head)->x - 1, (*head)->y))
        {
            return false;
        }

        if((*head)->next == NULL)
        {
            (*head)->x--;
            return true;
        }

        prevNode.x = (*head)->x;
        prevNode.y = (*head)->y;
        (*head)->x--;
        currNode.x = (*head)->next->x;
        currNode.y = (*head)->next->y;
        temp = (*head)->next;
        temp->x = prevNode.x;
        temp->y = prevNode.y;
        prevNode.x = currNode.x;
        prevNode.y = currNode.y;

        while(temp->next != NULL)
        {
            temp = temp->next;
            currNode.x = temp->x;
            currNode.y = temp->y;
            temp->x = prevNode.x;
            temp->y = prevNode.y;
            prevNode.x = currNode.x;
            prevNode.y = currNode.y;
        }
        return true;
    case RIGHT:
        if(((*head)->x + 1) > 7)
        {
            return false;
        }

        if(nodeMatch(*head, (*head)->x + 1, (*head)->y))
        {
            return false;
        }

        if((*head)->next == NULL)
        {
            (*head)->x++;
            return true;
        }

        prevNode.x = (*head)->x;
        prevNode.y = (*head)->y;
        (*head)->x++;
        currNode.x = (*head)->next->x;
        currNode.y = (*head)->next->y;
        temp = (*head)->next;
        temp->x = prevNode.x;
        temp->y = prevNode.y;
        prevNode.x = currNode.x;
        prevNode.y = currNode.y;

        while(temp->next != NULL)
        {
            temp = temp->next;
            currNode.x = temp->x;
            currNode.y = temp->y;
            temp->x = prevNode.x;
            temp->y = prevNode.y;
            prevNode.x = currNode.x;
            prevNode.y = currNode.y;
        }
        return true;
    default:
        return false;
    }
}

void printList(struct Node* head)
{
    struct Node* temp = NULL;

    if(head == NULL)
    {
        return;
    }

    if(head->next == NULL)
    {
        printf("{x: %d, y: %d}\n", head->x, head->y);
        return;
    }

    temp = head;
    while(temp->next != NULL)
    {
        printf("{x: %d, y: %d} -->\n", temp->x, temp->y);
        temp = temp->next;
    }
    printf("{x: %d, y: %d} -->\n", temp->x, temp->y);
}
