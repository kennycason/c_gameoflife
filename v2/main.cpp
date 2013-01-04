
#include <iostream>
#include <cstdlib>
#include <SDL/SDL.h>
#include <math.h>

/*
AUTHOR: Kenny Cason
DATE: 2009 DEC 20
SITE: Ken-Soft.com
EMAIL: kenneth.cason@gmail.com
ABOUT: This is the basic backbone for a "Life" like program. basically where simple rules result in complex reactions. Try making your own rules :)
*/

using namespace std;

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

void rule1();
void rule2();
void mutate();
void overpop();
void giveEnergy();

struct point {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 o;
};

int worldWidth = 640;
int worldHeight = 480;
point** world;

int main ( int argc, char** argv ) {
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen ) {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    srand(time(NULL));
    world = new point*[worldWidth];
    for(int x = 0; x < worldWidth; ++x) {
        world[x] = new point[worldHeight];
        for(int y = 0; y < worldHeight; y++) {
            world[x][y].r = rand()%256;
            world[x][y].g = rand()%256;
            world[x][y].b = rand()%256;
        }
    }
    cout << "Initialization complete" << endl;


    // program main loop
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                break;
            }
        }



        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
       // mutate();

        rule1();
      //  rule2();
	/* combine rule1 + overpop() + giveEnergy() for a neat effect */
	//overpop();
	//giveEnergy();

        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                setPixel(screen, x, y, SDL_MapRGB(screen->format, world[x][y].r, world[x][y].g, world[x][y].b));
            }
        }

        SDL_Flip(screen);
    }

    printf("Exited cleanly\n");
    return 0;
}



void mutate() {
        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                Uint32 val = world[x][y].r*(256*256) + world[x][y].g*(256) + world[x][y].b;
                for(int i = 0; i < 24; i++) {
                    if(rand()%100000 < 15) {
                        val -= pow(2, i);
                        break;
                    }
                }
                world[x][y].r = (val & 0x00FF0000) / (256*256);
                world[x][y].g = (val & 0x0000FF00) / (256);
                world[x][y].b = val & 0x000000FF;
            //    cout << " R " << (int)world[x][y].r << " G " << (int)world[x][y].g << " B " << (int)world[x][y].b << endl;
            }
        }
}



void rule1() {
        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                int r = world[x][y].r;
                int g = world[x][y].g;
                int b = world[x][y].b;
                /*
                  o o o
                  o x o
                  o o o
                */
                // RULES
                if(x > 0 && y > 0) { // top left
                    if(world[x - 1][y - 1].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x - 1][y - 1].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x - 1][y - 1].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(y > 0) { // top
                    if(world[x][y - 1].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x][y - 1].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x][y - 1].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(x < worldWidth - 1 && y > 0) { // top right
                    if(world[x + 1][y - 1].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x + 1][y - 1].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x + 1][y - 1].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(x > 0) { //  left
                    if(world[x - 1][y].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x - 1][y].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x - 1][y].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(x < worldWidth - 1) { // right
                    if(world[x + 1][y].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x + 1][y].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x + 1][y].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(x > 0 && y < worldHeight - 1) { // bottom left
                    if(world[x - 1][y + 1].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x - 1][y + 1].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x - 1][y + 1].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(y < worldHeight - 1) { // bottom
                    if(world[x][y + 1].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x][y + 1].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x][y + 1].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(x < worldWidth - 1 && y < worldHeight - 1) { // bottom right
                    if(world[x + 1][y + 1].r > 200) {
                        r+=2; g--; b--;
                    }
                    if(world[x + 1][y + 1].g > 200) {
                        r--; g+=2; b--;
                    }
                    if(world[x + 1][y + 1].b > 200) {
                        r--; g--; b+=2;
                    }
                }
                if(r < 0) {
                    r = 0;
                }
                if(g < 0) {
                    g = 0;
                }
                if(b < 0) {
                    b = 0;
                }
                if(r > 255) {
                    r = 255;
                }
                if(g > 255) {
                    g = 255;
                }
                if(b > 255) {
                    b = 255;
                }
                world[x][y].r = r;
                world[x][y].g = g;
                world[x][y].b = b;
            }
        }
}


void rule2() {
        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                Uint32 val = world[x][y].r*(256*256) + world[x][y].g*(256) + world[x][y].b;
                /*
                  o o o
                  o x o
                  o o o
                */
                // RULES
                if(x > 0 && y > 0) { // top left
                    Uint32 val2 = world[x - 1][y - 1].r*(256*256) + world[x - 1][y - 1].g*(256) + world[x - 1][y - 1].b;
                    val += (val2 - val)/330;
                }
                if(y > 0) { // top
                    Uint32 val2 = world[x][y - 1].r*(256*256) + world[x][y - 1].g*(256) + world[x][y - 1].b;
                    val += (val2 - val)/330;
                }
                if(x < worldWidth - 1 && y > 0) { // top right
                    Uint32 val2 = world[x + 1][y - 1].r*(256*256) + world[x + 1][y - 1].g*(256) + world[x + 1][y - 1].b;
                    val += (val2 - val)/330;
                }
                if(x > 0) { //  left
                    Uint32 val2 = world[x - 1][y].r*(256*256) + world[x - 1][y].g*(256) + world[x - 1][y].b;
                    val += (val2 - val)/330;
                }
                if(x < worldWidth - 1) { // right
                    Uint32 val2 = world[x + 1][y].r*(256*256) + world[x + 1][y].g*(256) + world[x + 1][y].b;
                    val += (val2 - val)/330;
                }
                if(x > 0 && y < worldHeight - 1) { // bottom left
                    Uint32 val2 = world[x - 1][y + 1].r*(256*256) + world[x - 1][y + 1].g*(256) + world[x - 1][y + 1].b;
                    val += (val2 - val)/330;
                }
                if(y < worldHeight - 1) { // bottom
                    Uint32 val2 = world[x][y + 1].r*(256*256) + world[x][y + 1].g*(256) + world[x][y + 1].b;
                    val += (val2 - val)/330;
                }
                if(x < worldWidth - 1 && y < worldHeight - 1) { // bottom right
                    Uint32 val2 = world[x + 1][y + 1].r*(256*256) + world[x + 1][y + 1].g*(256) + world[x + 1][y + 1].b;
                    val += (val2 - val)/330;
                }
                if(val >= 0x00FFFFFF) {
                    val = 0x00FFFFFF;
                } else if(val <= 0) {
                    val = 0x00FFFFFF;
                }
                world[x][y].r = (val & 0x00FF0000) / (256*256);
                world[x][y].g = (val & 0x0000FF00) / (256);
                world[x][y].b = val & 0x000000FF;
            //    cout << " R " << (int)world[x][y].r << " G " << (int)world[x][y].g << " B " << (int)world[x][y].b << endl;
            }
        }
}



void overpop() {
        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                int r = world[x][y].r;
                int g = world[x][y].g;
                int b = world[x][y].b;
                int o = world[x][y].o;
                b = 0;
                /*
                  o o o
                  o x o
                  o o o
                */
                // RULES
                if(r > 0) {
                    int sorround = 0;
                    if(x > 0 && y > 0) { // top left
                        if(world[x - 1][y - 1].r > 200) {
                            sorround++;
                        }
                    }
                    if(y > 0) { // top
                        if(world[x][y - 1].r > 200) {
                            sorround++;
                        }
                    }
                    if(x < worldWidth - 1 && y > 0) { // top right
                        if(world[x + 1][y - 1].r > 200) {
                            sorround++;
                        }
                    }
                    if(x > 0) { //  left
                        if(world[x - 1][y].r > 200) {
                            sorround++;
                        }
                    }
                    if(x < worldWidth - 1) { // right
                        if(world[x + 1][y].r > 200) {
                            sorround++;
                        }
                    }
                    if(x > 0 && y < worldHeight - 1) { // bottom left
                        if(world[x - 1][y + 1].r > 200) {
                            sorround++;
                        }
                    }
                    if(y < worldHeight - 1) { // bottom
                        if(world[x][y + 1].r > 200) {
                            sorround++;
                        }
                    }
                    if(x < worldWidth - 1 && y < worldHeight - 1) { // bottom right
                        if(world[x + 1][y + 1].r > 200) {
                            sorround++;
                        }
                    }
                    if(sorround > 3) { // over population, kill cell
                 //       cout << "KILL" << endl;
                        r = 0;
                        g = 255;
                 //       b = 0;
                    } else { // enriched, but lose some energy

                    }
                }

                if(r < 0) {
                    r = 0;
                }
                if(g < 0) {
                    g = 0;
                }

                if(r > 255) {
                    r = 255;
                }
                if(g > 255) {
                    g = 255;
                }

                world[x][y].r = r;
                world[x][y].g = g;
                world[x][y].b = b;
                world[x][y].o = o;
            }
        }
}

void giveEnergy() {
        for(int x = 0; x < worldWidth; x++) {
            for(int y = 0; y < worldHeight; y++) {
                int r = world[x][y].r;
                int g = world[x][y].g;
                int b = world[x][y].b;
                int o = world[x][y].o;
                /*
                  o o o
                  o x o
                  o o o
                */
                // RULES
                if(g < 10 && r < 10 && b < 10) {
                    g = 255;
                }
                 if(g > 0) { // it's food, feed the neighbors
                 //   r = 0;
                    if(x > 0 && y > 0) { // top left
                        world[x - 1][y - 1].r += 10;
                    }
                    if(y > 0) { // top
                        world[x][y - 1].r += 10;
                    }
                    if(x < worldWidth - 1 && y > 0) { // top right
                        world[x + 1][y - 1].r += 10;
                    }
                    if(x > 0) { //  left
                        world[x - 1][y].r += 10;
                    }
                    if(x < worldWidth - 1) { // right
                        world[x + 1][y].r += 10;
                    }
                    if(x > 0 && y < worldHeight - 1) { // bottom left
                        world[x - 1][y + 1].r += 10;
                    }
                    if(y < worldHeight - 1) { // bottom
                        world[x][y + 1].r += 10;
                    }
                    if(x < worldWidth - 1 && y < worldHeight - 1) { // bottom right
                        world[x + 1][y + 1].r += 10;
                    }
                    g -= 30;
                }

                if(r < 0) {
                    r = 0;
                }
                if(g < 0) {
                    g = 0;
                }

                if(r > 255) {
                    r = 255;
                }
                if(g > 255) {
                    g = 255;
                }

                world[x][y].r = r;
                world[x][y].g = g;
                world[x][y].b = b;
                world[x][y].o = o;
            }
        }
}




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
