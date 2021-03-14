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


int loc_i;
int loc_j;
int emptytile_i;
int emptytile_j;

// Initializes the game's board with tiles numbered 1 through d*d - 1
// (i.e., fills 2D array with values but does not actually print them)
void init(void)
{
    int total = d * d;
    
    // set tiles on board
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // remove one from total
            total = total - 1

            // add to board
            board[i][j] = total;
        }
    }
    
    // swap
    if ((d * d) % 2 == 0)
    {
        int swap = board[d - 1][d - 3]
        board[d - 1][d - 3] = board[d - 1][d - 2];
        board[d - 1][d - 2] = swap;
    }
}

// Prints the board in its current state
void draw(void)
{
    // Loop through board array
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // Print line instead of zero
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

// If tile borders empty space, moves tile and returns true, else returns false
bool move(int tile)
{
    // search for location of the tile
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                loc_i = i;
                loc_j = j;
            }
        }
    }

if (abs(loc_i - emptytile_i) == 1 && loc_j == emptytile_j)
{
    // swap the empty tile location with the location of the tile
    int temp = board[loc_i][loc_j];
    board[loc_i][loc_j] = board[emptytile_i][emptytile_j];
    board[emptytile_i][emptytile_j] = temp;

    emptytile_i = loc_i;
    emptytile_j = loc_j;
    return true;
}

    if (abs(loc_j - emptytile_j) == 1 && loc_i == emptytile_i)
    {
        // swap the empty tile location with the location of the tile
        int temp = board[loc_i][loc_j];
        board[loc_i][loc_j] = board[emptytile_i][emptytile_j];
        board[emptytile_i][emptytile_j] = temp;

        emptytile_i = loc_i;
        emptytile_j = loc_j;
        return true;
    }
    
    return false;
}

// Returns true if game is won (i.e., board is in winning configuration), else false
bool won(void)
{
    // ensure counter increments correctly
    int counter = 1;

    // look for tile
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == counter)
            {
                counter = counter + 1;
            }
         
            // check that the '_' ends on the bottom right corner
            else if (board[d - 1][d - 1] != 0)
            {
                return false;
            }
        }
    }

    if (counter != d * d)
    {
        return false;
    }
    else 
    {
        return true;
    }
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