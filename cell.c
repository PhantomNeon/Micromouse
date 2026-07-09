typedef struct
{
    // Coordinates: 0 - 15

    // Orientation: Bitshift left to rotate left
    // FRONT - 0001 - 1 - N
    // LEFT  - 0010 - 2 - W
    // BACK  - 0100 - 4 - S
    // RIGHT - 1000 - 8 - E

    unsigned int x : 4;
    unsigned int y : 4;

    unsigned int parentX : 4;
    unsigned int parentY : 4;

    int floodValue;
    unsigned int walls : 4;
} Cell;