// Orientation: Bitshift left to rotate left
// FRONT - 0001 - 1 - N
// LEFT  - 0010 - 2 - W
// BACK  - 0100 - 4 - S
// RIGHT - 1000 - 8 - E
#define NORTH 1
#define WEST 2
#define SOUTH 4
#define EAST 8

// Check if given wall configuration is walled in the given direction
#define isWalled(walls, direction) ((walls & direction) != 0)

Queue CellsToFlood;

int botFacing = 1;
int botX = 0;
int botY = 0;

int goalX = 7;
int goalY = 7;

int MazeWidth; // Number of columns
int MazeHeight; // Number of rows