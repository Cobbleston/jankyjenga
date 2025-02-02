#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#define ROWS 3
#define COLS 3
#define LEVELS 18 // Standard Jenga height

typedef struct {
    int present;
} Block;

void initializeTower(Block tower[LEVELS][ROWS][COLS]) {
    for (int level = 0; level < LEVELS; level++) {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                tower[level][row][col].present = 1;
            }
        }
    }
}

void printTower(Block tower[LEVELS][ROWS][COLS]) {
    for (int level = LEVELS - 1; level >= 0; level--) {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (tower[level][row][col].present) {
                    printf("[]");
                } else {
                    printf("  ");
                }
            }
            printf("\n");
        }
    }
}

int removeBlock(Block tower[LEVELS][ROWS][COLS], int level, int row, int col) {
    level--;
    row--;
    col--;

    if (level < 0 || level >= LEVELS || row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0; // Invalid input
    }

    if (tower[level][row][col].present) {
        tower[level][row][col].present = 0;
        return 1; // Block removed
    } else {
        return 0; // Block already removed or invalid
    }
}

int isUnstable(Block tower[LEVELS][ROWS][COLS]) {
    for (int l = 1; l < LEVELS; l++) { // Start from level 1 (second level)
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (tower[l][r][c].present) { // Check if block exists
                    int emptyBelow = 0;
                    for (int bl = 0; bl < l; bl++) { // Check all blocks below it
                        if (!tower[bl][r][c].present) {
                            emptyBelow++;
                        }
                    }
                    if (emptyBelow >= l) { // If number of empty spaces below is equal or greater than the current level, the tower is unstable
                        return 1; // Unstable
                    }
                }
            }
        }
    }
    return 0; // Stable
}

void clearScreen() {
    printf("\033[2J");
    printf("\033[H");
}

int main() {
    while (1) { // Main game loop (for playing again)
        Block tower[LEVELS][ROWS][COLS];
        initializeTower(tower);
        int gameOver = 0;
        int currentPlayer = 1;
        int winner = 0; //Keep track of the winner

        while (!gameOver) {
            clearScreen();
            printTower(tower);

            int level, row, col;

            printf("Player %d's turn (Level Row Column - e.g., 1 1 1): ", currentPlayer);
            if (scanf("%d %d %d", &level, &row, &col) != 3) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }

            if (removeBlock(tower, level, row, col)) {
                if (isUnstable(tower)) {
                    gameOver = 1;
                    clearScreen();
                    printTower(tower);
                    printf("Tower collapsed! Player %d loses.\n", currentPlayer);
                    winner = (currentPlayer == 1) ? 2 : 1; // Other player wins
                    printf("Player %d Wins!\n", winner);
                } else {
                    if (isUnstable(tower)) {
                        printf("Tower is getting unstable!\n");
                    }
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                }
            } else {
                printf("Invalid move.\n");
            }
        }

        char playAgain;
        printf("Play again? (y/n): ");
        scanf(" %c", &playAgain);
        if (playAgain != 'y' && playAgain != 'Y') {
            break; // Exit the outer loop
        }
    }
    return 0;
}