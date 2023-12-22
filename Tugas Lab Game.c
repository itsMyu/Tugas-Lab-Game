#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_PLAYERS 10

struct Player {
    char name[50];
    int score;
};

union PlayerData {
    char name[50];
    int score;
};

void displayMenu();
void startGame();
int generateRandomNumber();
int getPlayerGuess();
void saveScore(struct Player player);
void displayHighScores();
void sortHighScores(struct Player highScores[], int numScores);
int binarySearch(struct Player highScores[], int numScores, char playerName[]);

int main() {
    srand(time(NULL));

    int choice;

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startGame();
                break;
            case 2:
                displayHighScores();
                break;
            case 3:
                printf("Exiting the game. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

void displayMenu() {
    printf("\n=== Number Guessing Game ===\n");
    printf("1. Start Game\n");
    printf("2. Display High Scores\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void startGame() {
    printf("\n=== Game Started ===\n");

    int randomNumber = generateRandomNumber();
    int playerGuess;
    int attempts = 0;

    printf("Guess the number between 1 and 100.\n");

    do {
        playerGuess = getPlayerGuess();
        attempts++;

        if (playerGuess < randomNumber) {
            printf("Too low. Try again.\n");
        } else if (playerGuess > randomNumber) {
            printf("Too high. Try again.\n");
        } else {
            printf("Congratulations! You guessed the number in %d attempts.\n", attempts);

            struct Player currentPlayer;
            printf("Enter your name: ");
            scanf("%s", currentPlayer.name);
            currentPlayer.score = attempts;
            saveScore(currentPlayer);

            break;
        }
    } while (1);
}

int generateRandomNumber() {
    return rand() % 100 + 1;
}

int getPlayerGuess() {
    int guess;
    printf("Enter your guess: ");
    scanf("%d", &guess);
    return guess;
}

void saveScore(struct Player player) {
    FILE *file = fopen("high_scores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %d\n", player.name, player.score);
        fclose(file);
        printf("Score saved successfully.\n");
    } else {
        printf("Error saving score.\n");
    }
}

void displayHighScores() {
    printf("\n=== High Scores ===\n");

    FILE *file = fopen("high_scores.txt", "r");
    if (file != NULL) {
        struct Player highScores[MAX_PLAYERS];
        int numScores = 0;

        while (fscanf(file, "%s %d", highScores[numScores].name, &highScores[numScores].score) == 2) {
            numScores++;
        }

        fclose(file);

        sortHighScores(highScores, numScores);
		
		int i;
        for (i = 0; i < numScores; i++) {
            printf("%s: %d attempts\n", highScores[i].name, highScores[i].score);
        }

        char searchName[50];
        printf("\nEnter a player's name to search for their score: ");
        scanf("%s", searchName);
        int index = binarySearch(highScores, numScores, searchName);

        if (index != -1) {
            printf("Score found: %s: %d attempts\n", highScores[index].name, highScores[index].score);
        } else {
            printf("Player not found in high scores.\n");
        }
    } else {
        printf("No high scores available.\n");
    }
}

void sortHighScores(struct Player highScores[], int numScores) {
    int i;
	for (i = 0; i < numScores - 1; i++) {
		int j;
        for (j = 0; j < numScores - i - 1; j++) {
            if (highScores[j].score > highScores[j + 1].score) {
                struct Player temp = highScores[j];
                highScores[j] = highScores[j + 1];
                highScores[j + 1] = temp;
            }
        }
    }
}

int binarySearch(struct Player highScores[], int numScores, char playerName[]) {
    int left = 0, right = numScores - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (strcmp(highScores[mid].name, playerName) == 0) {
            return mid;
        }

        if (strcmp(highScores[mid].name, playerName) > 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;
}

