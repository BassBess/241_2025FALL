#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    for (int i = 0; i < 2; i++)
    {
        printf("Enter name for Player %d: ", i + 1);
        scanf(" %49[^\n]", players[i].name);

        printf("Enter symbol for %s: ", players[i].name);

        char symbol_input[100];
        scanf(" %99[^\n]", symbol_input);
        players[i].symbol = symbol_input[0];

        if (i == 1 && players[1].symbol == players[0].symbol)
        {
            printf("Symbol already taken! Using '%c' instead.\n", players[1].symbol + 1);
            players[1].symbol++;
        }
    }
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
    for (int start = 0; start <= 3; start++)
    {
        int count = 0;
        for (int offset = 0; offset < 4; offset++)
        {
            int c = col + start + offset;
            if (c < COLS && b[row][c] == p)
                count++;
        }
        if (count == 4)
            return 1;
    }

    for (int start = 0; start <= 2; start++)
    {
        int count = 0;
        for (int offset = 0; offset < 4; offset++)
        {
            int r = row + start + offset;
            if (r < ROWS && b[r][col] == p)
                count++;
        }
        if (count == 4)
            return 1;
    }

    for (int start_r = 0; start_r <= 2; start_r++)
    {
        for (int start_c = 0; start_c <= 3; start_c++)
        {
            int count = 0;
            for (int offset = 0; offset < 4; offset++)
            {
                int r = start_r + offset;
                int c = start_c + offset;
                if (r < ROWS && c < COLS && b[r][c] == p)
                    count++;
            }
            if (count == 4)
                return 1;
        }
    }

    for (int start_r = 0; start_r <= 2; start_r++)
    {
        for (int start_c = 3; start_c < COLS; start_c++)
        {
            int count = 0;
            for (int offset = 0; offset < 4; offset++)
            {
                int r = start_r + offset;
                int c = start_c - offset;
                if (r < ROWS && c >= 0 && b[r][c] == p)
                    count++;
            }
            if (count == 4)
                return 1;
        }
    }

    return 0;
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
    char **board = createBoard();
    Player players[2];
    int current = 0, gameOver = 0;

    printf("Welcome to Connect 4!!!!!!!!!!!!!!!!\n\n");
    setupPlayers(players);
    char temp[100];
    fgets(temp, sizeof(temp), stdin);
    printf("\nGame starting!\n");
    PrintBoard(board);

    while (!gameOver)
    {
        printf("\n%s (%c), choose a column (1-7): ", players[current].name, players[current].symbol);

        char input[10];
        fgets(input, sizeof(input), stdin);

        int column;
        if (sscanf(input, "%d", &column) != 1)
        {
            printf("Please enter a valid number (1-7).\n");
            continue;
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
            printf("\n%s (%c) wins!\n", players[current].name, players[current].symbol);
            gameOver = 1;
        }
        else if (isBoardFull(board))
        {
            printf("\nIt's a draw!\n");
            gameOver = 1;
        }
        else
        {
            current = !current;
        }
    }

    for (int i = 0; i < ROWS; i++)
        free(board[i]);
    free(board);
    return 0;
}
