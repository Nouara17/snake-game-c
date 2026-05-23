#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Pour Windows (_kbhit, _getch)
#include <unistd.h> // Pour usleep sur Linux/Mac
#include <time.h>

#define WIDTH 20
#define HEIGHT 20
#define EMPTY ' '
#define WALL '#'
#define SNAKE 'O'
#define FRUIT '*'
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

typedef struct {
    int x;
    int y;
} Point;

Point snake[WIDTH * HEIGHT];
int snake_length = 1;
Point fruit;
int direction = RIGHT;
int score = 0;

void initializeGame();
void drawBoard();
void updateSnake();
int checkCollision(Point new_head);
int checkFruit(Point new_head);
Point generateFruit();
void handleInput();

int main() {
    initializeGame();

    while (1) {
        drawBoard();
        handleInput();
        updateSnake();
        usleep(200000); // Contrôle la vitesse du jeu (200ms)
    }

    return 0;
}

void initializeGame() {
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    fruit = generateFruit();
    score = 0;
}

void drawBoard() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                printf("%c", WALL);
            } else {
                int is_snake = 0;
                for (int i = 0; i < snake_length; i++) {
                    if (snake[i].x == x && snake[i].y == y) {
                        printf("%c", SNAKE);
                        is_snake = 1;
                        break;
                    }
                }
                if (!is_snake) {
                    printf("%c", (fruit.x == x && fruit.y == y) ? FRUIT : EMPTY);
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void updateSnake() {
    Point new_head = snake[0];
    switch (direction) {
        case UP: new_head.y--; break;
        case DOWN: new_head.y++; break;
        case LEFT: new_head.x--; break;
        case RIGHT: new_head.x++; break;
    }

    if (checkCollision(new_head)) {
        printf("Game Over! Score final: %d\n", score);
        exit(0);
    }

    for (int i = snake_length; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    if (checkFruit(new_head)) {
        score += 10;
        snake_length++;
        fruit = generateFruit();
    }
}

void handleInput() {
    if (_kbhit()) {
        char input = _getch();
        switch (input) {
            case 'z': if (direction != DOWN) direction = UP; break;
            case 's': if (direction != UP) direction = DOWN; break;
            case 'q': if (direction != RIGHT) direction = LEFT; break;
            case 'd': if (direction != LEFT) direction = RIGHT; break;
        }
    }
}

int checkCollision(Point new_head) {
    if (new_head.x <= 0 || new_head.x >= WIDTH - 1 || new_head.y <= 0 || new_head.y >= HEIGHT - 1) {
        return 1;
    }
    for (int i = 1; i < snake_length; i++) {
        if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
            return 1;
        }
    }
    return 0;
}

int checkFruit(Point new_head) {
    return (new_head.x == fruit.x && new_head.y == fruit.y);
}

Point generateFruit() {
    Point new_fruit;
    int is_valid;
    do {
        is_valid = 1;
        new_fruit.x = rand() % (WIDTH - 2) + 1;
        new_fruit.y = rand() % (HEIGHT - 2) + 1;
        for (int i = 0; i < snake_length; i++) {
            if (new_fruit.x == snake[i].x && new_fruit.y == snake[i].y) {
                is_valid = 0;
                break;
            }
        }
    } while (!is_valid);
    return new_fruit;
}
