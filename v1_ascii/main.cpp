#include "math.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>


void init();
void draw();
void process();
void processMutate(int x, int y);
int getNumNeigbors(int x, int y);
void clrscr();
void flip();
char getLeft(int x, int y);
char getRight(int x, int y);
char getUp(int x, int y);
char getDown(int x, int y);
char getUpLeft(int x, int y);
char getUpRight(int x, int y);
char getDownLeft(int x, int y);
char getDownRight(int x, int y);
void sleep(unsigned int mseconds);

char BLOCKS[] = {' ', '*'};
int NUM_BLOCKS = 2;
char LIVE = BLOCKS[1];
char DEAD = BLOCKS[0];

int GRID_WIDTH = 70;
int GRID_HEIGHT = 22;
char** grid;
char** gridTmp;

int main() {
    init();
    bool running = true;
    while(running) {
        draw();
        sleep(500);
        process();
    }
}


void init() {
    srand(time(0));
    grid = new char*[GRID_WIDTH];
    gridTmp = new char*[GRID_WIDTH];
    for(int x = 0; x < GRID_WIDTH; ++x) {
         grid[x] = new char[GRID_HEIGHT];
         gridTmp[x] = new char[GRID_HEIGHT];
         for(int y = 0; y < GRID_HEIGHT; ++y) {
              grid[x][y] = BLOCKS[rand() % NUM_BLOCKS];
              gridTmp[x][y] = ' ';
         }
    }
}

void draw() {
    clrscr();
    for(int y = 0; y < GRID_HEIGHT; ++y) {
        for(int x = 0; x < GRID_WIDTH; ++x) {
            std::cout << grid[x][y];
        }
        std::cout << std::endl;
    }
}

void process() {
    for(int y = 0; y < GRID_HEIGHT; ++y) {
        for(int x = 0; x < GRID_WIDTH; ++x) {
            int neighbors = getNumNeigbors(x, y);
            if(grid[x][y] == LIVE) {
                //1. Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
                //2. Any live cell with more than three live neighbors dies, as if by overcrowding.
                if(neighbors < 2 || neighbors > 3) {
                    gridTmp[x][y] = DEAD;
                } else {
                    gridTmp[x][y] = LIVE;
                }
                //3. Any live cell with two or three live neighbors lives on to the next generation.

            } else {
                //4. Any dead cell with exactly three live neighbors becomes a live cell.
                if(neighbors == 3) {
                    gridTmp[x][y] = LIVE;
                } else {
                    gridTmp[x][y] = DEAD;
                }
            }
          //  processMutate(x, y);
        }
    }
    flip();
}


int getNumNeigbors(int x, int y) {
    int i = 0;
    if(getLeft(x, y) == LIVE) {
        i++;
    }
    if(getRight(x, y) == LIVE) {
        i++;
    }
    if(getUp(x, y) == LIVE) {
        i++;
    }
    if(getDown(x, y) == LIVE) {
        i++;
    }
    if(getUpLeft(x, y) == LIVE) {
        i++;
    }
    if(getUpRight(x, y) == LIVE) {
        i++;
    }
    if(getDownLeft(x, y) == LIVE) {
        i++;
    }
    if(getDownRight(x, y) == LIVE) {
        i++;
    }
    return i;
}


void processMutate(int x, int y) {
    if(rand() % 1000 > 997) {
        if(gridTmp[x][y] == DEAD) {
            gridTmp[x][y] = LIVE;
        }
    }
}

void flip() {
    char** tmp = grid;
    grid = gridTmp;
    gridTmp = tmp;

}


char getLeft(int x, int y) {
    if(x == 0) {
        return DEAD;
    }
    return grid[x - 1][y];
}

char getRight(int x, int y) {
    if(x == GRID_WIDTH - 1) {
        return DEAD;
    }
    return grid[x + 1][y];
}

char getUp(int x, int y) {
    if(y == 0) {
        return DEAD;
    }
    return grid[x][y - 1];
}

char getDown(int x, int y) {
    if(y == GRID_HEIGHT - 1) {
        return DEAD;
    }
    return grid[x][y + 1];
}

char getUpLeft(int x, int y) {
    if(x == 0 || y == 0) {
        return DEAD;
    }
    return grid[x - 1][y - 1];
}

char getUpRight(int x, int y) {
    if(x == GRID_WIDTH - 1 || y == 0) {
        return DEAD;
    }
    return grid[x + 1][y - 1];
}

char getDownLeft(int x, int y) {
    if(y == GRID_HEIGHT - 1 || x == 0) {
        return DEAD;
    }
    return grid[x - 1][y + 1];
}

char getDownRight(int x, int y) {
    if(y == GRID_HEIGHT - 1 || x == GRID_WIDTH - 1 ) {
        return DEAD;
    }
    return grid[x + 1][y + 1];
}

void clrscr() {
   COORD topLeft = {0, 0};
   DWORD cCharsWritten, dwConSize;
   CONSOLE_SCREEN_BUFFER_INFO cInfo;
              HANDLE hConsoleOutput = GetStdHandle((DWORD)-11);

   // Get the number of character cells in the current buffer
   GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);
   dwConSize = cInfo.dwSize.X * cInfo.dwSize.Y;

   // Fill the whole screen with blank chars
  // FillConsoleOutputCharacter(hConsoleOutput, (TCHAR)' ', dwConSize, topLeft, &cCharsWritten);

   // Get the current text attribute
   GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);

   // Set the buffer's attributes accordingly
   FillConsoleOutputAttribute(hConsoleOutput, cInfo.wAttributes, dwConSize, topLeft, &cCharsWritten);

   // Put the cursor at its home coordinates
   SetConsoleCursorPosition(hConsoleOutput, topLeft);
}


void sleep(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
