// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h".

// Make sure to include all relevant libraries
#include <stdio.h>
#include "colours.h"
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
// You don't need to use these, but they help make your code look more organized
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'
char mapArr[11][11];
int pacX = 5;
int pacY = 5;
int ghost1X = 1;
int ghost1Y = 1;
int ghost2X = 9;
int ghost2Y = 9;
char ogChar1 = DOT;
char ogChar2 = DOT;
int preMove[] = {NULL, NULL};

/*
 *
 * This function prints that map with the colours
 *
 */
void printMap() {
    char temp;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (temp != mapArr[i][j]) {
                switch (mapArr[i][j]) {
                    case DOT:
                        temp = DOT;
                        colourChange(WHITE);
                        break;
                    case WALL:
                        temp = WALL;
                        colourChange(BLUE);
                        break;
                    case GHOST:
                        temp = GHOST;
                        colourChange(PINK);
                        break;
                    case PACMAN:
                        temp = PACMAN;
                        colourChange(YELLOW);
                        break;
                    default:
                        temp = EMPTY;
                        colourChange(WHITE);
                        break;
                }
            }
            printf("%c ", mapArr[i][j]);
        }
        printf("\n");
    }
    colourChange(WHITE);
    return;
}
/*
 *
 * This function checks if the player has fulfilled the win condition
 *
 */
int winCheck(/*parameters*/){
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (mapArr[i][j] == DOT)
                return 0;
        }
    }
    printMap();
    printf("Congratulations! You win! Press any key to exit the game");
    return 1;
}

/*
 *
 * This function checks if the player has fulfilled the lose condition
 *
 */
int loseCheck(){
    if ((pacX == ghost1X && pacY == ghost1Y) || (pacX == ghost2X && pacY == ghost2Y)) {
        mapArr[pacY][pacX] = GHOST;
        printMap();
        printf("Sorry, you lose. Press any key to exit the game \n");
        return 1;
    }
    return 0;
}

/*
 *
 * This function checks if pacman has hit a wall or not
 *
 */
int isWall(char direction, int x, int y) {
    switch (direction) {
        case UP:
            if (mapArr[y-1][x] == WALL) {
                return 1;
            }
            break;
        case RIGHT:
            if (mapArr[y][x+1] == WALL) {
                return 1;
            }
            break;
        case DOWN:
            if (mapArr[y+1][x] == WALL) {
                return 1;
            }
            break;
        case LEFT:
            if (mapArr[y][x-1] == WALL) {
                return 1;
            }
            break;
    }
    return 0;
}
/*
 *
 * This function creates the map and prints it
 *
 */
void createMap() {
    int xPos, yPos;
    FILE *mapFile = fopen("map.txt", "r");
    if (mapFile == NULL) {
        printf("File not found.");
        return;
    }

    while (yPos < 12) {
        while (true) {
            if (yPos == 0) {
                for (int i = 0; i <= 12; i++) {
                    mapArr[yPos][i] = WALL;
                }
                yPos++;
                break;
            }
            if (xPos == 0) {
                mapArr[yPos][xPos] = WALL;
                xPos++;
            }
            char temp;
            fscanf(mapFile, "%c", &temp);

            if (temp == '\n' || xPos >= 10) {
                mapArr[yPos][xPos] = WALL;
                xPos = 0;
                yPos++;
                break;
            } else if (temp != EMPTY) {
                mapArr[yPos][xPos] = temp;
                xPos++;
            }
        }
    }
    for (int i = 1; i < 10; i++) {
        mapArr[10][i] = WALL;
    }
    return;
}

/*
 *
 * This function moves pacman in the specified direction and replaces the dots with empty spaces
 *
 */
void pacMove(char direction) {
    if (direction == UP) {
        mapArr[pacY][pacX] = EMPTY;
        pacY--;
        mapArr[pacY][pacX] = PACMAN;
    }
    else if (direction == RIGHT) {
        mapArr[pacY][pacX] = EMPTY;
        pacX++;
        mapArr[pacY][pacX] = PACMAN;
    }
    else if (direction == DOWN) {
        mapArr[pacY][pacX] = EMPTY;
        pacY++;
        mapArr[pacY][pacX] = PACMAN;
    }
    else {
        mapArr[pacY][pacX] = EMPTY;
        pacX--;
        mapArr[pacY][pacX] = PACMAN;
    }
}

/*
 *
 * This function checks if the ghost is in a direct line with pac man without a wall in between
 *
 */
int lineOfSight (int x, int y){
    int xDiff;
    int yDiff;

    if(x == pacX){
         yDiff = y - pacY;
        if (yDiff > 0) {
            for (int i = 0; i < abs(yDiff); i++) {
                if(mapArr[y - i][x] == WALL){
                    return 0;
                }
            }
            return 1;
        }
        else if (yDiff < 0){
            for (int i = 0; i <abs(yDiff); i++) {
                if(mapArr[y + i][x] == WALL){
                    return 0;
                }
            }
            return 2;
        }
    }
    else if (y == pacY){
         xDiff = x - pacX;
         if (xDiff < 0) {
             for (int i = 0; i <abs(xDiff); i++) {
                 if(mapArr[y][x + i] == WALL){
                    return 0;
                 }
             }
             return 3;
         }
         else if (xDiff > 0){
             for (int i = 0; i <abs(xDiff); i++) {
                 if(mapArr[y][x - i] == WALL){
                     return 0;
                 }
             }
             return 4;
         }
    }
    return 0;
}


/*
 *
 * This function controls the movement of the ghosts
 *
 */
void ghostMove(int ghostNum){

    int ghostX, ghostY, lastMove;
    char ogChar;

    if (ghostNum == 1){
         ghostX = ghost1X;
         ghostY = ghost1Y;
         ogChar = ogChar1;
         lastMove = preMove[0];
    } else if (ghostNum == 2){
         ghostX = ghost2X;
         ghostY = ghost2Y;
         ogChar = ogChar2;
         lastMove = preMove[1];
    }
    int direction;
    while (true) {
        direction = lineOfSight(ghostX, ghostY);
        int temp = direction;
        if (direction == 0) {
            direction = 1 + (rand() % 4);
        }

        if (direction == 1) {
            if (((isWall(UP, ghostX, ghostY)) == 1) || (lastMove == 2 && temp == 0)) {
                continue;
            } else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostY--;
                ogChar = mapArr[ghostY][ghostX];
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                lastMove = 1;
            }
        } else if (direction == 2) {
            if (((isWall(DOWN, ghostX, ghostY)) == 1) || (lastMove == 1 && temp == 0)) {
                continue;
            } else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostY++;
                ogChar = mapArr[ghostY][ghostX];
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                lastMove = 2;
            }
        } else if (direction == 3) {
            if (((isWall(RIGHT, ghostX, ghostY)) == 1) || (lastMove == 4 && temp == 0)) {
                continue;
            } else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostX++;
                ogChar = mapArr[ghostY][ghostX];
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                lastMove = 3;
            }
        } else if (direction == 4) {
            if (((isWall(LEFT, ghostX, ghostY)) == 1) || (lastMove == 3 && temp == 0)) {
                continue;
            } else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostX--;
                ogChar = mapArr[ghostY][ghostX];
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                lastMove = 4;
            }
        }
        break;
    }

    if (ghostNum == 1) {
        ghost1Y = ghostY;
        ghost1X = ghostX;
        ogChar1 = ogChar;
        preMove[0] = lastMove;
    } else if (ghostNum == 2) {
        ghost2Y = ghostY;
        ghost2X = ghostX;
        ogChar2 = ogChar;
        preMove[1] = lastMove;
    }
}


/*
 *
 * This function contains the main functionality of the game
 *
 */
int main() {
    createMap();
    printMap();

    char temp;
    int gameOver;
    srand(time(NULL));
    while (true) {
        temp = getch();

        switch (temp) {
            case UP:
            case 'W':
                if (isWall(UP,pacX, pacY) == 0) {
                    pacMove(UP);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    ghostMove(1);
                    ghostMove(2);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    printMap();
                }
                break;

            case RIGHT:
            case 'D':
                if (isWall(RIGHT, pacX, pacY) == 0) {
                    pacMove(RIGHT);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    ghostMove(1);
                    ghostMove(2);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    printMap();
                }
                break;

            case DOWN:
            case 'S':
                if (isWall(DOWN, pacX, pacY) == 0) {
                    pacMove(DOWN);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    ghostMove(1);
                    ghostMove(2);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    printMap();
                }
                break;

            case LEFT:
            case 'A':
                if (isWall(LEFT, pacX, pacY) == 0) {
                    pacMove(LEFT);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    ghostMove(1);
                    ghostMove(2);
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    printMap();
                }
                break;
        }
        if (gameOver == 1) {
            break;
        }
    }
    getch();
    return 0;

}
