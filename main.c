#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int win[] = { 7, 56, 448, 73, 146, 292, 273, 84 };

void printBoard(char** board) {
    for (int i = 0; i < 9; i++) {
        printf("[%c]", *(*board + i));

        if ((i + 1) % 3 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

void fillBoard(char* board, int pattern) {
    for (int i = 0; i < 9; i++) {
        if (pattern >> i & 1) {
            *board = '*';
        } else {
            *board = '-';
        }

        board++;
    }
}

void fillPlayers(char** players, int* pvp, int repeat) {
    if (repeat) {
        getchar();
    }

    char* buffer = malloc(sizeof(char) * 30);
    printf("\nIntroduce el nombre del Jugador 1:\n");
    scanf(" %[^\n]", buffer);
    getchar();
    *players = buffer;

    if (*pvp == 1) {
        buffer = malloc(sizeof(char) * 30);
        printf("\nIntroduce el nombre del Jugador 2:\n");
        scanf(" %[^\n]", buffer);
        getchar();
        *(players + 1) = buffer;
    } else {
        *(players + 1) = "CPU";
    }

    system("clear");
}

int checkWin(char* board, int flag) {
    int check = 0;

    // checar si x ganó
    for (int i = 0; i < 9; i++) {
        if (*(board + i) == 'x') {
            check |= 1 << i;
        }
    }

    for (int i = 0; i < 8; i++) {
        if ((check & win[i]) == win[i]) {
            return flag ? win[i] : 1;
        }
    }

    check = 0;

    // checar si o ganó
    for (int i = 0; i < 9; i++) {
        if (*(board + i) == 'o') {
            check |= 1 << i;
        }
    }

    for (int i = 0; i < 8; i++) {
        if ((check & win[i]) == win[i]) {
            return flag ? win[i] : 2;
        }
    }

    return 0;
}

int checkUsed(char *board, int coord) {
    coord -= '0';
    board += coord - 1;

    return !(*board >= '1' && *board <= '9');
}

char cpuMove(char* board) {
    int counter = 0;
    char res;

    for (int i = 0; i < 9; i++) {
        if (*(board + i) != 'x' && *(board + i) != 'o') {
            counter++;
        }
    }

    for (int i = 0, j = 0; j < rand() % counter + 1; i++) {
        if (*(board + i) != 'x' && *(board + i) != 'o') {
            res = *(board + i);
            j++;
        }
    }

    return res;
}

int play(char* board, char** players, int* turn, int pvp) {
    char coord = 0;
    int player = *turn % 2 == 0;
    char input = player ? 'o' : 'x';

    if (pvp == 1 || !player) {
        printBoard(&board);
        printf("Turno %d \"%s\" (%c)\n", *turn, *(players + player), input);
        printf("Selecciona una coordenada: ");
        scanf(" %c", &coord);
        getchar();

        while (coord < '1' || coord > '9' || checkUsed(board, coord)) {
            printf("Selecciona una coordenada válida: ");
            scanf(" %c", &coord);
            getchar();
        }
    }

    if (pvp != 1 && player) {
        coord = cpuMove(board);
    }

    board += coord - '0' - 1;
    *board = input;
    board -= coord - '0' - 1;

    (*turn)++;

    if(*turn > 9) {
        *turn = 0;
    }

    return checkWin(board, 0);
}

void init(char* board, char** players, int* turn, int* state, int* pvp, int repeat) {
    *state = 0;
    *turn = 1;
    system("clear");

    if (!repeat) {
        printf("JUEGO DEL GATO\nArmando Gonzalez\n");
        *pvp = 0;

        while (*pvp < 1 || *pvp > 2) {
            printf("\n1. Jugar con otra persona.\n2. Jugar contra la computadora.\n");
            scanf("%d", pvp);
            getchar();
        }

        fillPlayers(players, pvp, repeat);
    }


    for (int i = 0; i < 10; i++) {
        board[i] = i < 9 ? (i + 49) : 0;
    }
}

int main() {
    int playing = 1;
    char* board = (char *)malloc(sizeof(char) * 10);
    int turn = 1;
    int state = 0;
    int pvp = 0;
    int repeat = 0;
    char** players = malloc(sizeof(char*) * 2);
    srand(time(NULL));

    while (playing == 1) {
        init(board, players, &turn, &state, &pvp, repeat);

        while (turn > 0 && state == 0) {
            state = play(board, players, &turn, pvp);
            system("clear");
        }

        printBoard(&board);

        if (state > 0) {
            printf("\"%s\" ganó!\n\n", *(players + state - 1));
            printf("La jugada ganadora fue:\n\n");
            fillBoard(board, checkWin(board, 1));
            printBoard(&board);
        } else {
            printf("empate :(\n\n");
        }

        int valid = 0;
        char sel = 'S';

        while (!valid) {
            printf("Volver a jugar? S/n ");
            valid = scanf("%c", &sel);

            if (sel == 'S' || sel == 's' || sel == 10) {
                playing = 1;
            } else if (sel == 'N' || sel == 'n') {
                playing = 0;
            } else {
                valid = 0;
            }

            if (!valid) {
                printf("Introduzca un valor válido.\n");
            }
        }

        valid = 0;

        while (!valid && playing) {
            printf("Usar a los mismos jugadores? S/n ");
            valid = scanf("%c", &sel);

            if (sel == 'S' || sel == 's' || sel == 10) {
                repeat = 1;
            } else if (sel == 'N' || sel == 'n') {
                repeat = 0;
            } else {
                valid = 0;
            }

            if (!valid) {
                printf("Introduzca un valor válido.\n");
            }
        }

        if (playing == 1) {
            system("clear");
        } else {
            printf("\nadios.\n");
        }
    }

    return 0;
}