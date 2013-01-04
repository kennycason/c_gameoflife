#include <iostream>
#include <SDL/SDL.h>

/*
AUTHOR: Kenny Cason
DATE: 2009 DEC 21
SITE: Ken-Soft.com
EMAIL: kenneth.cason@gmail.com
ABOUT: John Conway's Game of Life + Mutation
    I added mutation to keep the game going longer otherwise it would eventually just balance out.
    Also, there are no "Edges" if a creature keeps going right it will wrap around to the left side, and vice versa in every direction.
*/

using namespace std;

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void initLife();
void rules();
void mutate();


Uint32 LIVECELL = 0x00000000;
Uint32 DEADCELL = 0x00FFFFFF;

// I used a structure only because it leaves room for expansion, (i.e. a point that represents more than just being alive or dead.
struct point {
    Uint32 p;
};

// vars
int worldWidth = 640;
int worldHeight = 480;
point** world;
point** world2;

int delay = 30;
long timer = SDL_GetTicks();
int mX = 0;
int mY = 0;
bool pauseSim = false;
bool mutation = false;
int generationChance = 10; // 1/10
int mutationRate = 1000; // 1/1000
// bounding rectangle for life to start (from center)
int startLifeWidth = 250;
int startLifeHeight = 250;

int main ( int argc, char** argv ) {
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(worldWidth, worldHeight, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);
    if ( !screen ) {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }


    // init world
    srand(time(NULL));
    world = new point*[worldWidth];
    world2 = new point*[worldWidth];
    for(int x = 0; x < worldWidth; x++) {
        world[x] = new point[worldHeight];
        world2[x] = new point[worldHeight];
    }
    initLife();
    // create life with in A given centered rectangle

    cout << "JOHN CONWAY's GAME OF LIFE by Kenny Cason" << endl;
    cout << "INITIALIZE SIMULATION CHANCE OF LIVE CELL = 1/"<< generationChance << endl;
    cout << "SIMULATION SPEED (DELAY mS: " << delay << ")" << endl;
    cout << "MUTATION SET TO: " << mutation << "\tMUTATION RATE SET TO: "<< mutationRate << endl;
    cout << "KEYBOARD:\nM:\tTOGGLE MUTATION\nP:\tPAUSE/UNPAUSE\nR:\tRESET SIMULATION\nUP/DOWN:\tSPEED UP/SLOW DOWN SIMULATION" << endl;
    cout << "Q/W:\tINCREASE/DECREASE GENERATION CHANCE UPON RESET\nA/S:\tINCREASE/DECREASE MUTATION RATE WHEN MUTATION ABLED" << endl;
    cout << "Z/X:\tINCREASE/DECREASE SIZE OF REGION THAT CELLS APPEAR UPON INITIALIZION OR RESETTING OF SIMULATION" << endl;
    cout << "MOUSE:\nRIGHT CLICK:\tTOGGLE CELL ALIVE/DEAD\nLEFT CLICK:\tPAUSE" << endl;

    // program main loop
    bool done = false;
    while (!done) {
        if(SDL_GetTicks() - timer > delay) {
        timer = SDL_GetTicks();

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        done = true;
                        cout << "EXITING SIMULATION" << endl;
                    }
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_UP) {
                        delay -= 30;
                        if(delay < 0) {
                            delay = 0;
                        }
                        cout << "SPEED UP SIMULATION (DELAY mS: " << delay << ")" << endl;
                    } else if (event.key.keysym.sym == SDLK_DOWN) {
                        delay += 30;
                        cout << "SLOW DOWN SIMULATION (DELAY mS: " << delay << ")" << endl;
                    } else if (event.key.keysym.sym == SDLK_m) {
                        mutation = mutation ? false : true;
                        cout << "MUTATION SET TO: " << mutation << "\tMUTATION RATE SET TO: "<< mutationRate << endl;
                    } else if (event.key.keysym.sym == SDLK_p) {
                        pauseSim = pauseSim ? false : true;
                        cout << "PAUSE SET TO: " << pauseSim << endl;
                    } else if (event.key.keysym.sym == SDLK_q) {
                        generationChance--;
                        if(generationChance < 1) {
                            generationChance = 1;
                        }
                        cout << "GENERATION CHANCE (UPON RESET) INCREASED TO 1/" << generationChance << endl;
                    } else if (event.key.keysym.sym == SDLK_w) {
                        generationChance++;
                        cout << "GENERATION CHANCE (UPON RESET) DECREASED TO 1/" << generationChance << endl;
                    } else if (event.key.keysym.sym == SDLK_z) {
                        startLifeWidth -= 10;
                        startLifeHeight -= 10;
                        if(startLifeHeight < 10) {
                            startLifeHeight = 10;
                            startLifeWidth = 10;
                        }
                        cout << "DIMENSIONS OF REGION FOR CELLS TO APPEAR DURING INITIALIZATION DECREASED TO " << startLifeWidth << " x " << startLifeHeight << endl;
                    }  else if (event.key.keysym.sym == SDLK_x) {
                          startLifeWidth += 10;
                        startLifeHeight += 10;
                        cout << "DIMENSIONS OF REGION FOR CELLS TO APPEAR DURING INITIALIZATION INCREASED TO " << startLifeWidth << " x " << startLifeHeight << endl;
                    } else if (event.key.keysym.sym == SDLK_a) {
                        mutationRate -= 100;
                        if(mutationRate < 1) {
                            mutationRate = 2;
                        }
                        cout << "MUTATION RATE (IF MUTATION IS ON) INCREASED TO 1/" << mutationRate << endl;
                    }  else if (event.key.keysym.sym == SDLK_s) {
                        mutationRate += 100;
                        cout << "MUTATION RATE (IF MUTATION IS ON) DECREASED TO 1/" << mutationRate << endl;
                    } else if(event.key.keysym.sym == SDLK_r) {
                        initLife();
                        cout << "RESET SIMULATION CHANCE OF LIVE CELL = 1/"<< generationChance << endl;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN: //Get the mouse offsets
                    if( event.button.button == SDL_BUTTON_LEFT ) {
                        mX = event.button.x;
                        mY = event.button.y;
                        world[mX][mY].p = (world[mX][mX].p == LIVECELL) ? DEADCELL : LIVECELL;
                        cout << "CELL (" << mX << "," << mY << ") = ";
                        if(world[mX][mY].p == LIVECELL) {
                            cout << "ALIVE" << endl;
                        } else {
                            cout << "DEAD" << endl;
                        }
                    }
                    if( event.button.button == SDL_BUTTON_RIGHT ) {
                        pauseSim = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if( event.button.button == SDL_BUTTON_RIGHT ) {
                        pauseSim = false;
                    }
                    break;
                }

            }
            // Clearscreen, not really necessary since every pixel on the screen is re-drawn each iteration
            //SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

            if(!pauseSim) {
                if(mutation) {
                    mutate();
                }
                rules();
            }
            for(int x = 0; x < worldWidth; x++) {
                for(int y = 0; y < worldHeight; y++) {
                    setPixel(screen, x, y, world[x][y].p);
                }
            }
            SDL_Flip(screen);

        }
    }

    printf("Exited cleanly\n");
    return 0;
}

void initLife() {
    for(int x = 0; x < worldWidth; x++) {
        for(int y = 0; y < worldHeight; y++) {
            world[x][y].p = DEADCELL;
        }
    }
    for(int x = 0; x < startLifeWidth; x++) {
        for(int y = 0; y < startLifeHeight; y++) {
            int realX = worldWidth/2 - startLifeWidth/2 + x;
            int realY = worldHeight/2 - startLifeHeight/2 + y;
            if(realX >= 0 && realX < worldWidth && realY >= 0 && realY < worldHeight) {
                if(rand()%generationChance == 0) {
                    world[realX][realY].p = LIVECELL;
                } else {
                    world[realX][realY].p = DEADCELL;
                }
            }
        }
    }
}

/*
Add some mutation to keep things changing and interesting.
*/
void mutate() {
    for(int x = 0; x < worldWidth; x++) {
        for(int y = 0; y < worldHeight; y++) {
            if(rand()%mutationRate < 1) {
                world[x][y].p = (world[x][y].p == LIVECELL) ? DEADCELL : LIVECELL;
            }
        }
    }
}


/*
These are the rules (Straight from wikipedia.
1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
2. Any live cell with more than three live neighbours dies, as if by overcrowding.
3. Any live cell with two or three live neighbours lives on to the next generation.
4. Any dead cell with exactly three live neighbours becomes a live cell.
*/
void rules() {
        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                /*
                  o o o
                  o x o
                  o o o
                */
                int neighbors = 0;
                if(x > 0 && y > 0) { // TOP LEFT
                    if(world[x - 1][y - 1].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(x == 0 && y > 0) {
                        if(world[worldWidth - 1][y - 1].p == LIVECELL) {
                            neighbors++;
                        }
                    } else if(x > 0 && y == 0) {
                        if(world[x - 1][worldHeight - 1].p == LIVECELL) {
                            neighbors++;
                        }
                    }/* else { // CORNERS
                        if(world[worldWidth - 1][worldHeight - 1].p == LIVECELL) {
                            neighbors++;
                        }
                    }*/
                }
                if(y > 0) { // TOP
                    if(world[x][y - 1].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(world[x][worldHeight - 1].p == LIVECELL) {
                        neighbors++;
                    }
                }
                if(x < worldWidth - 1 && y > 0) { // TOP RIGHT
                    if(world[x + 1][y - 1].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(x == worldWidth - 1 && y > 0) {
                        if(world[0][y - 1].p == LIVECELL) {
                            neighbors++;
                        }
                    } else if(x < worldWidth - 1 && y == 0) {
                        if(world[x + 1][worldHeight - 1].p == LIVECELL) {
                            neighbors++;
                        }
                    }/* else { // CORNERS
                        if(world[0][worldHeight - 1].p == LIVECELL) {
                            neighbors++;
                        }
                    }*/
                }
                if(x > 0) { //  LEFT
                    if(world[x - 1][y].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(world[worldWidth - 1][y].p == LIVECELL) {
                        neighbors++;
                    }
                }
                if(x < worldWidth - 1) { // RIGHT
                    if(world[x + 1][y].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(world[0][y].p == LIVECELL) {
                        neighbors++;
                    }
                }
                if(x > 0 && y < worldHeight - 1) { // BOTTOM LEFT
                    if(world[x - 1][y + 1].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(x > 0 && y == worldHeight - 1) {
                        if(world[x - 1][0].p == LIVECELL) {
                            neighbors++;
                        }
                    } else if(x == 0 && y < worldHeight - 1) {
                        if(world[worldWidth - 1][y + 1].p == LIVECELL) {
                            neighbors++;
                        }
                    }/* else { // CORNERS
                        if(world[worldWidth - 1][0].p == LIVECELL) {
                            neighbors++;
                        }
                    }*/
                }
                if(y < worldHeight - 1) { // BOTTOM
                    if(world[x][y + 1].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(world[x][0].p == LIVECELL) {
                        neighbors++;
                    }
                }
                if(x < worldWidth - 1 && y < worldHeight - 1) { // BOTTOM RIGHT
                    if(world[x + 1][y + 1].p == LIVECELL) {
                        neighbors++;
                    }
                } else { // wrap around
                    if(x < worldWidth - 1 && y == worldHeight - 1) {
                        if(world[x + 1][0].p == LIVECELL) {
                            neighbors++;
                        }
                    } else if(x == worldWidth - 1 && y < worldHeight - 1) {
                        if(world[0][y + 1].p == LIVECELL) {
                            neighbors++;
                        }
                    }/* else { // CORNERS
                        if(world[0][0].p == LIVECELL) {
                            neighbors++;
                        }
                    }*/
                }

                // Apply rules based on neighbors
                if(world[x][y].p == LIVECELL) {
                    if(neighbors < 2 || neighbors > 3) { // under/over population, kill it
                        world2[x][y].p = DEADCELL;
                    } else { // it live on
                        world2[x][y].p = LIVECELL;
                    }
                } else {
                    if(neighbors == 3) { // bring it back to life (reproduce)
                         world2[x][y].p = LIVECELL;
                    } else {
                         world2[x][y].p = DEADCELL;
                    }
                }

            }
        }
    point** tmp = world;
    world = world2;
    world2 = tmp;
}



/*
draw a pixel using SDL
*/
void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    if(surface == NULL) {
         cout << "Failed to set pixel, surface not initialized!"<< endl;
         return;
     }
    if(x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
       //  cout << "Pixel not within surface's dimensions"<< endl;
         return;
     }
    int bpp = surface->format->BytesPerPixel;
    /* p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}
