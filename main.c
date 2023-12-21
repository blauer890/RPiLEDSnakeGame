#include <wiringPi.h>
#include <wiringShift.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <pthread.h>

#include "LinkedList.h"
#include "LEDMatrix.h"

pthread_mutex_t directionMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t renderThreadHandle;
enum Direction currDirection = UP;

void printBoard(struct Node *head)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(nodeMatch(head, j, i))
            {
                printf("1 ");
            }
            else
            {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

void shutdown(int sig)
{
    uint8_t zeros[8] = {0};
    writeRows(zeros);
    exit(EXIT_SUCCESS);
}

void spawnFood(struct Node* head, int *foodX, int *foodY)
{
    *foodX = rand() % 8;
    *foodY = -1;

    int confYIdx = 0;
    int confY[8] = {0};
    int availableYIdx = 0;
    int availableY[8] = {0};

    struct Node* temp = NULL;
    temp = head;
    while(temp->next != NULL)
    {
        if(temp->x == *foodX)
        {
            confY[confYIdx] = temp->y;
            confYIdx++;
        }
        temp = temp->next;
    }
    confYIdx = 0;
    for(int i = 0; i < 8; i++)
    {
        if(i != confY[i])
        {
            availableY[availableYIdx] = i;
            availableYIdx++;
        }
        confYIdx++;
    }
    if(availableYIdx > 0)
    {
        *foodY = availableY[(rand() % (availableYIdx + 1))];
    }
}

void growTail(struct Node* head, int *tailX, int *tailY)
{
    struct Node* temp = NULL;
    enum Direction choices[4];
    enum Direction finalChoice;
    int choicesIdx = 0;

    temp = head;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    if(!nodeMatch(head, temp->x - 1, temp->y))
    {
        choices[choicesIdx] = LEFT;
        choicesIdx++;
    }

    if(!nodeMatch(head, temp->x + 1, temp->y))
    {
        choices[choicesIdx] = RIGHT;
        choicesIdx++;
    }

    if(!nodeMatch(head, temp->x, temp->y - 1))
    {
        choices[choicesIdx] = UP;
        choicesIdx++;
    }

    if(!nodeMatch(head, temp->x, temp->y + 1))
    {
        choices[choicesIdx] = DOWN;
        choicesIdx++;
    }

    finalChoice = choices[(rand() % (choicesIdx + 1))];
    switch(finalChoice)
    {
    case LEFT:
        *tailX = temp->x - 1;
        *tailY = temp->y;
        break;
    case RIGHT:
        *tailX = temp->x + 1;
        *tailY = temp->y;
        break;
    case UP:
        *tailX = temp->x;
        *tailY = temp->y - 1;
        break;
    case DOWN:
        *tailX = temp->x;
        *tailY = temp->y + 1;
        break;
    default:
        break;
    }
}

void writeSnakeToMatrix(struct Node* head, int matrix[8][8])
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(nodeMatch(head, j, i))
            {
                matrix[i][j] = 1;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }
}

void *renderSnake(void *arg)
{
    struct Node* snakeHead = NULL;
    int matrix[8][8] = {0};
    uint8_t rows[8] = {0};
    const uint8_t zeros[8] = {0};
    bool findingFood = false;
    int score = 0;

    int headX = 4;
    int headY = 4;

    int foodX = -1;
    int foodY = -1;

    int tailX = -1;
    int tailY = -1;
    setupLEDMatrix();

    addNode(&snakeHead, headX, headY);
    addNode(&snakeHead, headX, headY + 1);
    addNode(&snakeHead, headX, headY + 2);

    while(1)
    {
        pthread_mutex_lock(&directionMutex);
        if(currDirection != DONE)
        {
            if(!moveHead(&snakeHead, currDirection))
            {
                currDirection = DONE;
                pthread_mutex_unlock(&directionMutex);
                break;
            }
        }
        else
        {
            pthread_mutex_unlock(&directionMutex);
            break;
        }
        pthread_mutex_unlock(&directionMutex);
        if((snakeHead->x == foodX) && (snakeHead->y == foodY))
        {
            findingFood = false;
            growTail(snakeHead, &tailX, &tailY);
            addNode(&snakeHead, tailX, tailY);
            score++;
        }
        writeSnakeToMatrix(snakeHead, matrix);

        if(!findingFood)
        {
            spawnFood(snakeHead, &foodX, &foodY);
            findingFood = true;
        }
        matrix[foodY][foodX] = 1;
        cartesianToRowValues(matrix, rows);
        displayRows(rows);
    }
    writeRows(zeros);
    destroyList(&snakeHead);
    printf("final score: %d\n", score);
    return NULL;
}

int main()
{
    char decision = '\0';
    uint8_t zeros[8] = {0};

    srand(time(0));

    pthread_create(&renderThreadHandle, NULL, renderSnake, NULL);

    signal(SIGINT, shutdown);

    printf("Hit \'UP_ARROW\' and \'Enter\' to move the snake up\n");
    printf("Hit \'LEFT_ARROW\' and \'Enter\' to move the snake left\n");
    printf("Hit \'DOWN_ARROW\' and \'Enter\' to move the snake down\n");
    printf("Hit \'RIGHT_ARROW\' and \'Enter\' to move the snake right\n");
    printf("Hit \'q\' and \'Enter\' to quit\n");

    while(currDirection != DONE)
    {
        fflush(stdin);

        if(getc(stdin) == '\033')
        {
            getc(stdin);
            switch(getc(stdin))
            {
            case 'A':
                pthread_mutex_lock(&directionMutex);
                currDirection = UP;
                pthread_mutex_unlock(&directionMutex);
                break;
            case 'B':
                pthread_mutex_lock(&directionMutex);
                currDirection = DOWN;
                pthread_mutex_unlock(&directionMutex);
                break;
            case 'C':
                pthread_mutex_lock(&directionMutex);
                currDirection = RIGHT;
                pthread_mutex_unlock(&directionMutex);
                break;
            case 'D':
                pthread_mutex_lock(&directionMutex);
                currDirection = LEFT;
                pthread_mutex_unlock(&directionMutex);
                break;
            default:
                break;
            }
        }

        if(getc(stdin) == 'q')
        {
            pthread_mutex_lock(&directionMutex);
            currDirection = DONE;
            pthread_mutex_unlock(&directionMutex);
        }
    }
    writeRows(zeros);
    pthread_join(renderThreadHandle, NULL);
    return 0;
}
