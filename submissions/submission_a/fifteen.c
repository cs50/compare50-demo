/**
 * fifteen.c
 *
 * Computer Science 50
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Constants
#define DIM_MIN 3
#define DIM_MAX 9
#define COLOR "\033[32m"

// Board
int board[DIM_MAX][DIM_MAX];

// Dimensions
int d;

// Prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);


// Clears screen using ANSI escape sequences
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

// Greets player
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

// Initializes the game's board with tiles numbered 1 through d*d - 1
// (i.e., fills 2D array with values but does not actually print them)
void init(void)
{
    int total = d * d;
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = --total;
        }
    }
    
    if ((d * d) % 2 == 0)
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    }
}

// Prints the board in its current state
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf("  _");
            }
            else 
            {
                printf("%3i", board[i][j]);
            }
        }
        
        printf("\n\n");
    }
}

// Returns true if game is won (i.e., board is in winning configuration), else false
bool move(int tile)
{
    if (tile > d * d - 1 || tile < 1)
    {
        return false;
    }

    int x = 0;
    int y = 0;

    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] == tile)
            {
                x = row;
                y = col;
            }
        }
    }

    // swap if tile is adjacent to moveable tile
    if (x - 1 >= 0 && board[x - 1][y] == 0)
    {
        board[x - 1][y] = board[x][y];
        board[x][y] = 0;
        return true;
    }
    else if (x + 1 < d && board[x + 1][y] == 0)
    {
        board[x + 1][y] = board[x][y];
        board[x][y] = 0;
        return true;
    }
    else if (y - 1 >= 0 && board[x][y - 1] == 0)
    {
        board[x][y - 1] = board[x][y];
        board[x][y] = 0;
        return true;
    }
    else if (y + 1 < d && board[x][y + 1] == 0)
    {
        board[x][y + 1] = board[x][y];
        board[x][y] = 0;
        return true;
    }

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // located position of tile
    int position = 1;

    // check has user won the game
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            // ascending order means user has won
            if (board[row][column] == position)
            {
                // counts from 1 to max tile
                position++;
            }
            else
            {
                return false;
            }

            if ((d * d) == position)
            {
                return true;
            }
        }
    }
    return true;
}


int main(int argc, string argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // Ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // Open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // Greet user with instructions
    greet();

    // Initialize the board
    init();

    // Accept moves until game is won
    while (true)
    {
        // Clear the screen
        clear();

        // Draw the current state of the board
        draw();

        // Log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // Check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // Prompt for move
        int tile = get_int("Tile to move: ");

        // Quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // Log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // Move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // Sleep thread for animation's sake
        usleep(50000);
    }

    // Close log
    fclose(file);

    // Success
    return 0;
}