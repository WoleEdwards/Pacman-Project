
#include <stdio.h>
#include "colours.h"
#include <conio.h>
#include <cstdlib>
#include <ctime>

//Defines the characters associated with the game
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

//creates the array which stores the map
char mapArr[11][11];

//creates global variables for the integers used in the code
int pacX = 5;
int pacY = 5;
int ghost1X = 1;
int ghost1Y = 1;
int ghost2X = 9;
int ghost2Y = 9;
int preMove[] = {NULL, NULL};

//global variable for the character in front of the ghost
char ogChar1 = DOT;
char ogChar2 = DOT;

/*
 *
 * This function prints that map with the colours
 *
 */
void printMap() {
    //defines a temporary character variable
    char temp;

    //runs for each column
    for (int i = 0; i < 11; i++) {
        //runs for each row
        for (int j = 0; j < 11; j++) {
            //checks if the temp char is not equal to the char at the given spot in the array
            if (temp != mapArr[i][j]) {
                //changes the colour based on what game element is at that part of the array
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
            //prints the character at that spot of the array
            printf("%c ", mapArr[i][j]);
        }
        printf("\n");
    }
    //turns the colour of the printed characters back to white
    colourChange(WHITE);
    return;
}
/*
 *
 * This function checks if the player has fulfilled the win condition
 *
 */
int winCheck(){
    //runs for each column of the array
    for (int i = 0; i < 11; i++) {

        //runs for each row of the array
        for (int j = 0; j < 11; j++) {

            //checks if there is a dot at the given spot
            if (mapArr[i][j] == DOT)
                return 0;
        }
    }
    //if no dots were found the win condition has been met
    printMap();
    printf("Congratulations! You win! Press any key to exit the game");
    return 1;
}

/*
 *
 * This function checks if the player has fulfilled the losing condition
 *
 */
int loseCheck(){
    //checks if the position of the ghost is equal to the position of pacman
    if ((pacX == ghost1X && pacY == ghost1Y) || (pacX == ghost2X && pacY == ghost2Y)) {
        mapArr[pacY][pacX] = GHOST;
        printMap();
        printf("Sorry, you lose. Press any key to exit the game \n");
        return 1;
    }
    //if they are not equal the game continues
    return 0;
}

/*
 *
 * This function checks if the ghost or pacman has hit a wall or not
 * It takes the x and y position of the game element as parameters
 */
int isWall(char direction, int x, int y) {
    //changes for each direction
    switch (direction) {
        //checks if there is a wall above when trying to move up
        case UP:
            if (mapArr[y-1][x] == WALL) {
                return 1;
            }
            break;
            //checks if there is a wall to the side when trying to move right
        case RIGHT:
            if (mapArr[y][x+1] == WALL) {
                return 1;
            }
            break;
            //checks if there is a wall below when trying to move down
        case DOWN:
            if (mapArr[y+1][x] == WALL) {
                return 1;
            }
            break;
            //checks if there is a wall to the side when trying to move left
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
 * This function creates the map and saves it to the array
 *
 */
void createMap() {
    //declares integer variables for x and y positions
    int xPos, yPos;
    //opens the file and checks if it is null
    FILE *mapFile = fopen("map.txt", "r");
    if (mapFile == NULL) {
        printf("File not found.");
        return;
    }

    //runs until the ypos = 11
    while (yPos < 12) {
        //runs until break
        while (true) {
            //creates the outermost borders
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
            //reads the map from the file
            char temp;
            fscanf(mapFile, "%c", &temp);

            //
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
    //adds the wall at the far right to the array
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
    //moves pacman up and removes the dot
    if (direction == UP) {
        mapArr[pacY][pacX] = EMPTY;
        pacY--;
        mapArr[pacY][pacX] = PACMAN;
    }
    //moves pacman right and removes the dot
    else if (direction == RIGHT) {
        mapArr[pacY][pacX] = EMPTY;
        pacX++;
        mapArr[pacY][pacX] = PACMAN;
    }
    //moves pacman down and removes the dot
    else if (direction == DOWN) {
        mapArr[pacY][pacX] = EMPTY;
        pacY++;
        mapArr[pacY][pacX] = PACMAN;
    }
    //moves pacman left and removes the dot
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
    //declares variables
    int xDiff;
    int yDiff;

    //check if the ghosts x is equal to pacmans x
    if(x == pacX){
        //finds the difference in their y values
         yDiff = y - pacY;
         //runs if pacman is above the ghost
        if (yDiff > 0) {
            //runs for each space between the ghost and pacman
            for (int i = 0; i <abs(yDiff); i++) {
                //checks if any of the spaces are a wall
                if(mapArr[y - i][x] == WALL){
                    return 0;
                }
            }
            //if no wall is between and pacman is above returns 1
            return 1;
        }
        //runs if pacman is below the ghost
        else if (yDiff < 0){
            //runs for each space between the ghost and pacman
            for (int i = 0; i <abs(yDiff); i++) {
                //checks if any of the spaces are a wall
                if(mapArr[y + i][x] == WALL){
                    return 0;
                }
            }
            //if no wall is between and pacman is below returns 2
            return 2;
        }
    }
    //check if ghosts y is equal to pacmans y
    else if(y == pacY){
        //finds the difference in their x values
         xDiff = x - pacX;
         //runs if pacman is to the right of the ghost
         if (xDiff < 0) {
             //runs for each space between the ghost and pacman
             for (int i = 0; i <abs(xDiff); i++) {
                 //checks if any of those spaces are walls
                 if(mapArr[y][x + i] == WALL){
                     //ends the function
                    return 0;
                 }
             }
             //if no wall is between and pacman is to the right, returns 3
             return 3;
         }
         //checks if pacman is to the left of the ghost
         else if (xDiff > 0){
             //runs for each space between the ghost and pacman
             for (int i = 0; i <abs(xDiff); i++) {
                 //checks if any of those spaces are walls
                 if(mapArr[y][x - i] == WALL){
                     //ends the function
                     return 0;
                 }
             }
             //if no wall between and pacman is to the left returns 4
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
    //declares integer variables for ghost movement
    int ghostX, ghostY, lastMove;
    //declare char variable for saving the character in a position
    char ogChar;

    //checks if ghost 1 is being moved
    if (ghostNum == 1){
        ghostX = ghost1X;
        ghostY = ghost1Y;
        ogChar = ogChar1;
        lastMove = preMove[0];
    }

    //checks if ghost 2 is being moved
    else if (ghostNum == 2){
        ghostX = ghost2X;
        ghostY = ghost2Y;
        ogChar = ogChar2;
        lastMove = preMove[1];
    }
    //delcares variable for the direction which the ghosts move
    int direction;

    //runs until break is called
    while (true) {
        //direction is set to the return of line of sight (0 if pacman not in sight)
        direction = lineOfSight(ghostX, ghostY);
        //set a temp variable to the direction
        int temp = direction;
        //if direction is 0 create a random direction for the ghost to move in
        if (direction == 0) {
            direction = 1 + (rand() % 4);
        }

        //checks if direction was set to 1
        if (direction == 1) {
            //if there is a wall above or the last move was downwards and temp variable was 0 rerun the loop
            if (((isWall(UP, ghostX, ghostY)) == 1) || (lastMove == 2 && temp == 0)) {
                continue;
            }

            //if the above was false then the ghost will move upwards and be replaced with what was in that position
            else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostY--;
                ogChar = mapArr[ghostY][ghostX];
                //checks if the second ghost was being moved and if the character infront was a ghost to prevent duplication
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                //save the move that was just made
                lastMove = 1;
            }
        }

        //check if the direction was set to 2
        else if (direction == 2) {
            //if there is a wall below or the last move was upwards and temp equals 0 rerun the loop
            if (((isWall(DOWN, ghostX, ghostY)) == 1) || (lastMove == 1 && temp == 0)) {
                continue;
            }

            //if the above if statement was false move the ghost downwards and replace what was in its position before
            else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostY++;
                ogChar = mapArr[ghostY][ghostX];
                //checks if the second ghost was being moved and if the character infront was a ghost to prevent duplication
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                //saves the move that was just made
                lastMove = 2;
            }
        }

        //checks if the direction was set to 3
        else if (direction == 3) {
            //if there is a wall to the right or the last move was to the left and temp equals 0 rerun the loop
            if (((isWall(RIGHT, ghostX, ghostY)) == 1) || (lastMove == 4 && temp == 0)) {
                continue;
            }
            //if the above was false then move the ghost right and replace it with what was in that position before
            else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostX++;
                ogChar = mapArr[ghostY][ghostX];
                //checks if the second ghost was being moved and if the character infront was a ghost to prevent duplication
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                //saves the move that was made
                lastMove = 3;
            }
        }

        //checks if the direction was set to 4
        else if (direction == 4) {
            //if there is a wall to the left or the last move was to the right and temp equals 0 rerun the loop
            if (((isWall(LEFT, ghostX, ghostY)) == 1) || (lastMove == 3 && temp == 0)) {
                continue;
            }
            //if the above is false then move pacman to the left and replace the ghost with what was in its position before
            else {
                mapArr[ghostY][ghostX] = ogChar;
                ghostX--;
                ogChar = mapArr[ghostY][ghostX];
                //checks if the second ghost was being moved and if the character infront was a ghost to prevent duplication
                if (ghostNum == 2 && ogChar == GHOST) {
                    ogChar = ogChar1;
                }
                mapArr[ghostY][ghostX] = GHOST;
                //save the move that was made
                lastMove = 4;
            }
        }
        //break the while loop
        break;
    }

    //save the local variables to the global variables for ghost 1
    if (ghostNum == 1) {
        ghost1Y = ghostY;
        ghost1X = ghostX;
        ogChar1 = ogChar;
        preMove[0] = lastMove;
    }

    //save the local variables to the global variables for ghost 2
    else if (ghostNum == 2) {
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
    //create and print the initial map
    createMap();
    printMap();

    //defines a temporary character variable
    char temp;
    //defines an int which changes when the game is over
    int gameOver;
    //creates a seed for the randomized movement of ghosts
    srand(time(NULL));
    //runs until break is called
    while (true) {
        //temp is set to the character input by the user
        temp = getch();

        //different function depending on if WASD were input
        switch (temp) {
            //runs if w was input
            case UP:
            case 'W':
                //if there is no wall above move pacman up
                if (isWall(UP,pacX, pacY) == 0) {
                    pacMove(UP);
                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //move the ghosts using the ghost move function
                    ghostMove(1);
                    ghostMove(2);
                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //print the map with the new positions of the ghosts and pacman
                    printMap();
                }
                //end the switch case
                break;

            //runs if d was input
            case RIGHT:
            case 'D':
                //if there is no wall to the right move pacman
                if (isWall(RIGHT, pacX, pacY) == 0) {
                    pacMove(RIGHT);
                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //move the ghosts using the ghost move function
                    ghostMove(1);
                    ghostMove(2);

                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //print the map with the new positions
                    printMap();
                }
                //end the switch case
                break;

            //runs if s was input
            case DOWN:
            case 'S':
                //if there is no wall below move pacman down
                if (isWall(DOWN, pacX, pacY) == 0) {
                    pacMove(DOWN);
                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //move the ghosts using the ghostMove function
                    ghostMove(1);
                    ghostMove(2);

                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //print the map with the new positions
                    printMap();
                }
                //end the switch case
                break;

            //runs if A was input
            case LEFT:
            case 'A':
                //if there is no wall to the left move pacman left
                if (isWall(LEFT, pacX, pacY) == 0) {
                    pacMove(LEFT);

                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //move the ghosts using the ghostMove function
                    ghostMove(1);
                    ghostMove(2);
                    //check if the game has ended
                    if (winCheck() == 1 || loseCheck() == 1) {
                        gameOver = 1;
                        break;
                    }
                    //print the map with the new positions
                    printMap();
                }
                //end the switch case
                break;
        }
        //if game has ended, end the while loop
        if (gameOver == 1) {
            break;
        }
    }
    //get a final character to end the game
    getch();

}
