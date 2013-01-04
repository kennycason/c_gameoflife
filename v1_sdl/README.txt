NAME:  JOHN CONWAY's GAME OF LIFE
AUTHOR: Kenny Cason
WEBSITE: www.ken-soft.com
DATE:   2009 DEC 21

Enjoy, do what ever you want with it, maybe give me some credit :)

ABOUT:
The rules of the game (straight from wikipedia):
The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, live or dead. Every cell interacts with its eight neighbors, which are the cells that are directly horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:

   1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
   2. Any live cell with more than three live neighbours dies, as if by overcrowding.
   3. Any live cell with two or three live neighbours lives on to the next generation.
   4. Any dead cell with exactly three live neighbours becomes a live cell.

The initial pattern constitutes the seed of the system. The first generation is created by applying the above rules simultaneously to every cell in the seed—births and deaths happen simultaneously, and the discrete moment at which this happens is sometimes called a tick (in other words, each generation is a pure function of the one before). The rules continue to be applied repeatedly to create further generations.


NOTE: To compile you will need to have SDL (www.libsdl.org) installed
COMPILE: 
g++ main.cpp -o LIFE -lSDL

RUN:
./LIFE


KEYS
M 	TOGGLE MUTATION
P 	PAUSE/UNPAUSE
R	RESET SIMULATION
UP/DOWN	SPEED UP/SLOW DOWN SIMULATION
Q/W	INCREASE/DECREASE GENERATION CHANCE UPON RESET
A/S	INCREASE/DECREASE MUTATION RATE WHEN MUTATION ABLED
Z/X  	INCREASE/DECREASE SIZE OF REGION THAT CELLS APPEAR UPON INITIALIZION OR RESETTING OF SIMULATION
MOUSE
RIGHT CLICK	TOGGLE CELL ALIVE/DEAD
LEFT CLICK	PAUSE
