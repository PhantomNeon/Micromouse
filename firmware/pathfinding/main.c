#include "main.h"

// Debug function to log flood value of entire maze
void LogMazeFlood(Cell maze[MazeWidth][MazeHeight])
{
    for (int x = 0; x < MazeWidth; x++)
    {
        for (int y = 0; y < MazeHeight; y++)
        {
            fprintf(stderr, " %d ", maze[x][y].floodValue);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n--------------------------------------------\n");
    fflush(stderr);
}

// Debug function to log walls of entire maze
void LogMazeWalls(Cell maze[MazeWidth][MazeHeight])
{

    for (int x = 0; x < MazeWidth; x++)
    {
        for (int y = 0; y < MazeHeight; y++)
        {
            fprintf(stderr, " %d ", maze[x][y].walls);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n--------------------------------------------\n");
    fflush(stderr);
}

// Debug function to log a string
void LogString(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

// Debug function to log an integer
void LogInt(int text)
{
    fprintf(stderr, "%d\n", text);
    fflush(stderr);
}

// Convert integer to string
void intToStr(int N, char *str)
{
    sprintf(str, "%d", N);
}

// Set all walls to 0
// Set all flood values to -1
Cell InitializeMaze(Cell maze[MazeWidth][MazeHeight])
{
    for (int x = 0; x < MazeWidth; x++)
    {
        for (int y = 0; y < MazeHeight; y++)
        {
            maze[x][y].x = y;
            maze[x][y].y = x;

            maze[x][y].parentX = -1;
            maze[x][y].parentY = -1;

            maze[x][y].walls = 0;
            maze[x][y].floodValue = -1;
        }
    }
}

// Moves bot forward by 1 unit and updates coordinates
// Returns false if crash
bool MoveForward()
{
    if (API_moveForward("1"))
    {
        switch (botFacing)
        {
        case NORTH:
            botY++;
            break;
        case WEST:
            botX--;
            break;
        case SOUTH:
            botY--;
            break;
        case EAST:
            botX++;
            break;
        default:
            break;
        }
        return true;
    }

    return false;
}

// Convert an orientation relative to the bot to an orientation relative to the world
int BotOrientationToWorldOrientation(int num)
{
    switch (botFacing)
    {
    case NORTH:
        return num;
        break;

    case WEST:
        if (num == EAST)
            return NORTH;

        return num << 1;
        break;

    case EAST:
        if (num == NORTH)
            return EAST;

        return num >> 1;
        break;

    case SOUTH:
        if (num == NORTH)
            return SOUTH;
        else if (num == WEST)
            return EAST;
        else if (num == SOUTH)
            return NORTH;
        else if (num == EAST)
            return WEST;
        break;

    default:
        break;
    }
}

// Calculates the wall configuration of the cell bot currently is in
int CalculateWallConfiguration()
{
    int WallConfiguration = 0;

    if (API_wallFront())
        WallConfiguration += BotOrientationToWorldOrientation(NORTH);
    if (API_wallRight())
        WallConfiguration += BotOrientationToWorldOrientation(EAST);
    if (API_wallLeft())
        WallConfiguration += BotOrientationToWorldOrientation(WEST);

    return WallConfiguration;
}

// Update the maze walls with new information
void UpdateMazeWalls(Cell maze[MazeWidth][MazeHeight], int x, int y, int config)
{
    maze[x][y].walls = maze[x][y].walls | config;

    if (isWalled(config, NORTH))
    {
        if ((y + 1) < MazeHeight)
        {
            maze[x][y + 1].walls = maze[x][y + 1].walls | SOUTH;
        }
    }

    if (isWalled(config, WEST))
    {
        if ((x - 1) >= 0)
        {
            maze[x - 1][y].walls = maze[x - 1][y].walls | EAST;
        }
    }

    if (isWalled(config, SOUTH))
    {
        if ((y - 1) >= 0)
        {
            maze[x][y - 1].walls = maze[x][y - 1].walls | NORTH;
        }
    }

    if (isWalled(config, EAST))
    {
        if ((x + 1) < MazeWidth)
        {
            maze[x + 1][y].walls = maze[x + 1][y].walls | WEST;
        }
    }
}

// Reset flood values of all cells to -1
void ResetMazeFloodValues(Cell maze[MazeWidth][MazeHeight])
{
    for (int x = 0; x < MazeWidth; x++)
    {
        for (int y = 0; y < MazeHeight; y++)
        {
            maze[x][y].floodValue = -1;
        }
    }
}

// Update flood value of given cell and add neighbouring cells to queue
void FloodCell(Cell maze[MazeWidth][MazeHeight], Cell cell)
{
    if (maze[cell.x][cell.y].floodValue == -1)
    {
        maze[cell.x][cell.y] = cell;

        if (!isWalled(cell.walls, NORTH) && (cell.y + 1) < MazeHeight)
        {
            Cell front = {cell.x, cell.y + 1, cell.x, cell.y, cell.floodValue + 1, maze[cell.x][cell.y + 1].walls};

            if (!contains(&CellsToFlood, front) && !(front.x == cell.parentX && front.y == cell.parentY))
                enqueue(&CellsToFlood, front);
        }

        if (!isWalled(cell.walls, WEST) && (cell.x - 1) >= 0)
        {
            Cell left = {cell.x - 1, cell.y, cell.x, cell.y, cell.floodValue + 1, maze[cell.x - 1][cell.y].walls};

            if (!contains(&CellsToFlood, left) && !(left.x == cell.parentX && left.y == cell.parentY))
                enqueue(&CellsToFlood, left);
        }

        if (!isWalled(cell.walls, SOUTH) && (cell.y - 1) >= 0)
        {
            Cell rear = {cell.x, cell.y - 1, cell.x, cell.y, cell.floodValue + 1, maze[cell.x][cell.y - 1].walls};

            if (!contains(&CellsToFlood, rear) && !(rear.x == cell.parentX && rear.y == cell.parentY))
                enqueue(&CellsToFlood, rear);
        }

        if (!isWalled(cell.walls, EAST) && (cell.x + 1) < MazeWidth)
        {
            Cell right = {cell.x + 1, cell.y, cell.x, cell.y, cell.floodValue + 1, maze[cell.x + 1][cell.y].walls};

            if (!contains(&CellsToFlood, right) && !(right.x == cell.parentX && right.y == cell.parentY))
                enqueue(&CellsToFlood, right);
        }
    }
}

// Start the flooding loop
void InitiateFlood(Cell maze[MazeWidth][MazeHeight])
{
    ResetMazeFloodValues(maze);
    Cell center = maze[goalX][goalY];
    center.floodValue = 0;

    initializeQueue(&CellsToFlood);
    enqueue(&CellsToFlood, center);

    while (!isEmpty(&CellsToFlood))
    {
        FloodCell(maze, peek(&CellsToFlood));
        dequeue(&CellsToFlood);
    }
}

// Moves the bot to the neighbour with lowest flood value
// Return true when goal reached
bool Move(Cell maze[MazeWidth][MazeHeight])
{
    int lowestFloodValue = maze[botX][botY].floodValue;
    int direction = botFacing;

    if (lowestFloodValue == 0)
        return true;

    if (!isWalled(maze[botX][botY].walls, NORTH) && (botY + 1) < MazeHeight)
    {
        if (maze[botX][botY + 1].floodValue < lowestFloodValue)
        {
            direction = 1;
            lowestFloodValue = maze[botX][botY + 1].floodValue;
        }
    }

    if (!isWalled(maze[botX][botY].walls, WEST) && (botX - 1) >= 0)
    {
        if (maze[botX - 1][botY].floodValue < lowestFloodValue)
        {
            direction = 2;
            lowestFloodValue = maze[botX - 1][botY].floodValue;
        }
    }

    if (!isWalled(maze[botX][botY].walls, SOUTH) && (botY - 1) >= 0)
    {
        if (maze[botX][botY - 1].floodValue < lowestFloodValue)
        {
            direction = 4;
            lowestFloodValue = maze[botX][botY - 1].floodValue;
        }
    }

    if (!isWalled(maze[botX][botY].walls, EAST) && (botX + 1) < MazeWidth)
    {
        if (maze[botX + 1][botY].floodValue < lowestFloodValue)
        {
            direction = 8;
            lowestFloodValue = maze[botX + 1][botY].floodValue;
        }
    }

    if (direction == botFacing)
    {
    }

    else if (((direction / botFacing) == 2) || (direction == 1 && botFacing == 8))
    {
        API_turnLeft();
        botFacing = BotOrientationToWorldOrientation(2);
    }

    else if (((botFacing / direction) == 2) || (direction == 8 && botFacing == 1))
    {
        API_turnRight();
        botFacing = BotOrientationToWorldOrientation(8);
    }

    else if (((direction == 4 && botFacing) == 1) || (direction == 1 && botFacing == 4) || (direction == 8 && botFacing == 2) || (direction == 2 && botFacing == 8))
    {
        API_turnLeft();
        API_turnLeft();
        botFacing = direction;
    }

    if (!MoveForward())
        return false;

    return false;
}

// Send API instructions to render known maze
void RenderMaze(int wallConfig, Cell maze[MazeWidth][MazeHeight])
{
    if (isWalled(wallConfig, NORTH))
        API_setWall(botX, botY, 'n');
    if (isWalled(wallConfig, WEST))
        API_setWall(botX, botY, 'w');
    if (isWalled(wallConfig, SOUTH))
        API_setWall(botX, botY, 's');
    if (isWalled(wallConfig, EAST))
        API_setWall(botX, botY, 'e');

    for (int x = 0; x < MazeWidth; x++)
    {
        for (int y = 0; y < MazeHeight; y++)
        {
            char text[10];
            intToStr(maze[x][y].floodValue, text);
            API_setText(x, y, text);
        }
    }
}

int main()
{
    LogString("Start Up.");

    MazeHeight = API_mazeHeight();
    MazeWidth = API_mazeWidth();

    Cell Maze[MazeWidth][MazeHeight];
    InitializeMaze(Maze);

    initializeQueue(&CellsToFlood);

    LogString("Initiating Run Loop.");
    while (1)
    {
        int walls = CalculateWallConfiguration();
        UpdateMazeWalls(Maze, botX, botY, walls);

        InitiateFlood(Maze);

        RenderMaze(walls, Maze);

        if (Move(Maze))
            return 0;
    }
}