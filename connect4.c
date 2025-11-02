#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ROWS 6
#define COLS 7
#define EMPTY '.'

typedef struct
{
    char name[50];
    char symbol;
} Player;

void setupPlayers(Player players[2])
{
    strcpy(players[0].name, "Player A");
    players[0].symbol = 'A';
    
    strcpy(players[1].name, "Player B");
    players[1].symbol = 'B';
    
    printf("Players set: %s (%c) vs %s (%c)\n", 
           players[0].name, players[0].symbol,
           players[1].name, players[1].symbol);
}

char **createBoard()
{
    char **b = malloc(ROWS * sizeof(char *));
    for (int i = 0; i < ROWS; i++)
    {
        b[i] = malloc(COLS * sizeof(char));
        for (int j = 0; j < COLS; j++)
            b[i][j] = EMPTY;
    }
    return b;
}

void PrintBoard(char **b)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
            printf("%c ", b[i][j]);
        printf("\n");
    }
    printf("1 2 3 4 5 6 7\n");
}

int placePiece(char **b, int col, char p, int *row_placed)
{
    col--;
    if (b[0][col] != EMPTY)
    {
        printf("Column full. Try another.\n");
        return 0;
    }
    for (int i = ROWS - 1; i >= 0; i--)
    {
        if (b[i][col] == EMPTY)
        {
            b[i][col] = p;
            if (row_placed)
                *row_placed = i;
            return 1;
        }
    }
    return 0;
}

int checkWin(char **b, int row, int col, char p)
{
    int count;
    
    count = 0;
    for (int i = 0; i < COLS; i++) {
        if (b[row][i] == p) {
            count++;
            if (count == 4) return 1;
        } else {
            count = 0;
        }
    }
    
    count = 0;
    for (int i = 0; i < ROWS; i++) {
        if (b[i][col] == p) {
            count++;
            if (count == 4) return 1;
        } else {
            count = 0;
        }
    }
    
    count = 0;
    int r = row, c = col;
    while (r > 0 && c > 0) { r--; c--; } 
    while (r < ROWS && c < COLS) {
        if (b[r][c] == p) {
            count++;
            if (count == 4) return 1;
        } else {
            count = 0;
        }
        r++; c++;
    }
    
    count = 0;
    r = row; c = col;
    while (r > 0 && c < COLS - 1) { r--; c++; } 
    while (r < ROWS && c >= 0) {
        if (b[r][c] == p) {
            count++;
            if (count == 4) return 1;
        } else {
            count = 0;
        }
        r++; c--;
    }
    
    return 0;
}
int canWin(char **b, int col, char s) {
    if (b[0][col-1] != EMPTY) return 0;
    
    int row = -1;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (b[i][col-1] == EMPTY) {
            row = i;
            break;
        }
    }
    if (row == -1) return 0;
    
    b[row][col-1] = s;
    int wins = checkWin(b, row, col-1, s);
    b[row][col-1] = EMPTY;
    
    return wins;
}

int medBot(char **b, char botS, char playerS) {
    for (int col = 1; col <= COLS; col++) {
        if (canWin(b, col, botS)) return col;
    }
    
    for (int col = 1; col <= COLS; col++) {
        if (canWin(b, col, playerS)) return col;
    }

    int centers[] = {4, 3, 5, 2, 6, 1, 7};
    for (int i = 0; i < COLS; i++) {
        int col = centers[i];
        if (b[0][col-1] == EMPTY) return col;
    }

    for (int col = 1; col <= COLS; col++) {
        if (b[0][col-1] == EMPTY) return col;
    }
    return 1;
}
int isBoardFull(char **b)
{
    for (int i = 0; i < COLS; i++)
        if (b[0][i] == EMPTY)
            return 0;
    return 1;
}

int main()
{
    srand(time(NULL));
    int playAgain = 1;
    
    while (playAgain)
    {
        char **board = createBoard();
        Player players[2];
        int current = 0, gameOver = 0;
        int bot = 0;  

        printf("Welcome to Connect 4!!!!!!!!!!!!!!!!\n\n");
        printf("Choose game mode:\n");
printf("1 - Player vs Player\n");
printf("2 - Player vs Computer (Easy)\n");
printf("3 - Player vs Computer (Medium)\n");
printf("Enter your choice (1, 2, or 3): ");
        int choice;
        scanf("%d", &choice);
        while (getchar() != '\n');
        
     int botDiff = 0;
if (choice == 2) {
    bot = 1;
    botDiff = 1;
    strcpy(players[1].name, "Computer (Easy)");
} else if (choice == 3) {
    bot = 1;
    botDiff = 2;
    strcpy(players[1].name, "Computer (Medium)");
}
        setupPlayers(players);
        printf("\nGame starting!\n");
        PrintBoard(board);

        while (!gameOver)
        {
            int column = -1;

            
          if (bot && current == 1)  
{
    if (botDiff == 1) {
     
        do {
            column = (rand() % 7) + 1;  
        } while (board[0][column - 1] != EMPTY);
    } else if (botDiff == 2) {
        
        column = medBot(board, players[current].symbol, players[0].symbol);
    }
    printf("\n%s (%c) chooses column %d\n", players[current].name, players[current].symbol, column);
}
            else
            {
                
                printf("\n%s (%c), choose a column (1-7): ", players[current].name, players[current].symbol);

                char input[10];
                fgets(input, sizeof(input), stdin);

                if (sscanf(input, "%d", &column) != 1)
                {
                    printf("Please enter a valid number (1-7).\n");
                    continue;
                }
            }

            if (column < 1 || column > 7)
            {
                printf("Invalid column. Choose between 1 and %d.\n", COLS);
                continue;
            }

            int row_placed;
            if (!placePiece(board, column, players[current].symbol, &row_placed))
                continue;

            PrintBoard(board);

            if (checkWin(board, row_placed, column - 1, players[current].symbol))
            {
                printf("\n%s (%c) wins!\nPlay again? Press 'y' for YES, or any other key to quit: \n", players[current].name, players[current].symbol);
  
                gameOver = 1;
                
                char response[10];
                fgets(response, sizeof(response), stdin);
                
                if (response[0] == 'y' || response[0] == 'Y') {
                    playAgain = 1;
                    printf("\nStarting new game...\n\n");
                } else {
                    playAgain = 0;
                    printf("Thanks for playing!\n");
                }
            }
            else if (isBoardFull(board))
            {
                printf("\nIt's a draw!\nPlay again? Press 'y' for YES, or any other key to quit: \n");
      
                gameOver = 1;
                
                char response[10];
                fgets(response, sizeof(response), stdin);
                
                if (response[0] == 'y' || response[0] == 'Y') {
                    playAgain = 1;
                    printf("\nStarting new game...\n\n");
                } else {
                    playAgain = 0;
                    printf("Thanks for playing!\n");
                }
            }
            else
            {
                current = !current;
            }
        }

        for (int i = 0; i < ROWS; i++)
            free(board[i]);
        free(board);
    }
    
    return 0;
}
