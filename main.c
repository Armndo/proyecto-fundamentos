#include <stdio.h>
#include <stdlib.h> // para llamar a malloc
#include <time.h> // para crear la semilla con srand para crear números aleatorios

/*
 * Programa en C que permite ejectuar el juego del gato
 * contra otro jugador, o contra la computadora.
 * Programó: Armando Omar González González
 * 07/01/2025
 */

// Constante que almacena las condiciones de victoria (de forma bitwise)
const int win[] = { 7, 56, 448, 73, 146, 292, 273, 84 };

void printBoard(char** board) {
    // función void que imprime el tablero del juego
    // apuntador de apuntadores board, que nos permite imprimir cada caracter usando operaciones con apuntadores
    for (int i = 0; i < 9; i++) {
        printf("[%c]", *(*board + i));  // accede al valor que está en el apuntador en la posición i e imprime el caracter entre corchetes

        if ((i + 1) % 3 == 0) {         // cada 3 elementos agrega un salto de linea
            printf("\n");
        }
    }

    printf("\n");
}

void fillBoard(char* board, int pattern) {
    // función void que llena el tablero del juego con la jugada ganadora
    // apuntador board, el cual sobreescribimos con caracteres '-' y '*'
    // entero pattern, el cual contiene el patrón que nos permite llenar el tablero usando este mismo como una máscara y con operaciones bitwise
    for (int i = 0; i < 9; i++) {
        if (pattern >> i & 1) { // el pattern que es un número entero se mueve i veces a la izquierda bitwise y luego aplica una máscara
            *board = '*';       // con un and bitwise para determinar si es 1 o 0 en la posición requerida y agrega '*' al tablero
        } else {                // de lo contrario agrega '-' al tablero
            *board = '-';
        }

        board++;
    }
}

void fillPlayers(char** players, int* pvp, int repeat) {
    // función void que nos permite llenar el nombre de nuestros jugadores
    // apuntador de apuntadores players, que contendrá los nombres de los jugadores
    // apuntador pvp, que nos permite traer el valor de la bandera si estamos jugando contra otro jugador
    // int repeat, que nos trae la bandera para verificar si se ha repetido el juego
    if (repeat) {
        getchar();
    }

    char* buffer = malloc(sizeof(char) * 30);               // iniciamos el buffer con un tamaño de 30 carácteres
    printf("\nIntroduce el nombre del Jugador 1:\n");
    scanf(" %[^\n]", buffer);
    getchar();
    *players = buffer;                                      // almacenamos el valor del buffer en la primera posición del apuntador

    if (*pvp == 1) {                                        // si estamos jugando contra otro jugador reinicializamos el buffer
        buffer = malloc(sizeof(char) * 30);
        printf("\nIntroduce el nombre del Jugador 2:\n");
        scanf(" %[^\n]", buffer);
        getchar();
        *(players + 1) = buffer;                            // y almacenamos su valor en la segunda posición del apuntador
    } else {
        *(players + 1) = "CPU";                             // de lo contrario le asignamos el valor de "CPU" al segundo jugador
    }

    system("clear");
}

int checkWin(char* board, int flag) {
    // funcion int que nos retorna si ganó el jugador 1 o el jugador 2, o sigue en empate el juego
    // apuntador board, que contiene las jugadas del tablero
    // int flag, bandera que dependiendo si está en 1 o en 0 nos arroja el jugador que ganó o el patrón de la jugada ganadora
    int check = 0;

    // checar si x ganó
    for (int i = 0; i < 9; i++) {           // se iteran los valores del tablero y si en esa posición se detecta una 'x'
        if (*(board + i) == 'x') {          // se aplica or bitwise al valor de check para ir generando una máscara que nos permite
            check |= 1 << i;                // determinar si el jugador 1 ganó la partida
        }
    }

    for (int i = 0; i < 8; i++) {           // iterando nuevamente pero ahora los posibles valores de las condiciones de victoria
        if ((check & win[i]) == win[i]) {   // se hace un and bitwise para determinar si la máscara en check es identica a la condición de victoria
            return flag ? win[i] : 1;       // dependiento del flag se retorna la jugada ganadora o el jugador ganador
        }
    }

    check = 0;

    // checar si o ganó
    for (int i = 0; i < 9; i++) {           // se repite el mismo proceso pero para validar las jugadas de 'o', es exactamente lo mismo
        if (*(board + i) == 'o') {
            check |= 1 << i;
        }
    }

    for (int i = 0; i < 8; i++) {
        if ((check & win[i]) == win[i]) {
            return flag ? win[i] : 2;
        }
    }

    return 0;                               // se retorna 0 en caso de que el juego siga en curso o no se haya considerado a ningun ganador (empate)
}

int checkUsed(char *board, int coord) {
    // funcion int que determina si una coordenada ya está usada jugada
    // apuntador board, que contiene las jugadas del tablero
    // int coord, que contiene la jugada que vamos a validar
    coord -= '0';                               // se resta el valor del caracter '0' para quitar el offset de los caracteres y acceder al apuntador en la posición correspondiente
    board += coord - 1;                         // se incrementa el valor del apuntador para acceder al valor en dicha posición

    return !(*board >= '1' && *board <= '9');   // se retorna una bandera con la condición de que lo que esté en la posición jugada sea algo distinto a los caracteres del '1' al '9'
}

char cpuMove(char* board) {
    // funcion char que regresa el valor de la coordenada aleatoria que juega la computadora
    // apuntador board, contiene las jugadas del tablero
    int counter = 0;                                        // se inicializa el contador de coordenadas útiles en 0
    char res;

    for (int i = 0; i < 9; i++) {
        if (*(board + i) != 'x' && *(board + i) != 'o') {   // se itera el tablero y si el valor en esa posición del apuntador es distinto a 'x' y a 'o', i.e., del '1' al '9'
            counter++;                                      // se incrementa el contador de coordenadas útiles a elegir
        }
    }

    for (int i = 0, j = 0; j < rand() % counter + 1; i++) { // se itera nuevamente el tablero pero con la condición de paro siendo un número aleatorio máximo hasta el valor de nuestro contador + 1
        if (*(board + i) != 'x' && *(board + i) != 'o') {   // si el valor es distinto a 'x' o a 'o'
            res = *(board + i);                             // se actualiza el valor de retorno, i.e, la coordenada a retornar
            j++;                                            // se incrementa el contador secundario
        }
    }

    return res;
}

int play(char* board, char** players, int* turn, int pvp) {
    // función int que retorna el estado de la partida
    // apuntador board, contiene las jugadas del tablero
    // apuntador de apuntador de caracteres players que contiene los nombres de los jugadores
    // apuntador turn, que pasa por memoria el turno actual
    // int pvp, bandera que determina si estamos jugando contra otro jugador o contra la computadora
    char coord = 0;                                                     // la coordenada a jugar se inicia en 0
    int player = *turn % 2 == 0;                                        // se determina si le toca al jugador 1 o al 2 dependiendo si el turno es non o par
    char input = player ? 'o' : 'x';                                    // se determina el caracter que le corresponde la jugador 1 o al 2

    if (pvp == 1 || !player) {                                          // si se juega contra otro jugador o si es el turno del jugador 1 (para evitar mostrar las jugadas de la computadora)
        printBoard(&board);                                             // se imprime el tablero
        printf("Turno %d \"%s\" (%c)\n", *turn, *(players + player), input);
        printf("Selecciona una coordenada: ");
        scanf(" %c", &coord);
        getchar();

        while (coord < '1' || coord > '9' || checkUsed(board, coord)) { // si la coordenada no está entre '1' y '9' y está usada se repite el proceso de ingresar la coordenada
            printf("Selecciona una coordenada válida: ");
            scanf(" %c", &coord);
            getchar();
        }
    }

    if (pvp != 1 && player) {                                           // si estás jugando contra la computadora y le toca al jugador 2 (la computadora)
        coord = cpuMove(board);                                         // se llama a la función cpuMove
    }

    board += coord - '0' - 1;                                           // se cambia la posición del apuntador para que corresponda a una coordenada entre 0 y 8
    *board = input;                                                     // se cambia el valor de ese caracter en esa posición
    board -= coord - '0' - 1;                                           // se regresa la posición del apuntador a la original

    (*turn)++;                                                          // se incrementa el turno

    if(*turn > 9) {                                                     // si el turno excede 9 (el número máximo de turnos en una partida)
        *turn = 0;                                                      // se reinicia al turno 0 (condición de finalización del juego)
    }

    return checkWin(board, 0);                                          // se retorna la condición de victoria o de empate (el estado de la partida)
}

void init(char* board, char** players, int* turn, int* state, int* pvp, int repeat) {
    // función void que inicializa el juego
    // apuntador board, que contendrá las jugadas del tablero
    // apuntador de apuntador players, que contendrá los nombres de los jugadores
    // apuntador turn, que contiene el turno de la partida
    // apuntador state, que contiene el estado de la partida
    // apuntador pvp, que sirve como bandera si se está jugando contra otro jugador o contra la computadora
    // int repeat, bandera que determina si se repiten los jugadores
    *state = 0;                                         // se cambia el estado del juego a 0
    *turn = 1;                                          // se inicia el juego con el turno 1
    system("clear");

    if (!repeat) {                                      // si no se repiten los jugadores
        printf("JUEGO DEL GATO\nArmando Gonzalez\n");
        *pvp = 0;                                       // se cambia el valor de la bandera pvp a 0 para entrar en el while

        while (*pvp < 1 || *pvp > 2) {                  // mientras el valor de pvp no sea el correcto se repite la lectura de esta bandera
            printf("\n1. Jugar con otra persona.\n2. Jugar contra la computadora.\n");
            scanf("%d", pvp);
            getchar();
        }

        fillPlayers(players, pvp, repeat);              // se llenan los nombres de los jugadores en la función fillPlayers
    }


    for (int i = 0; i < 10; i++) {
        board[i] = i < 9 ? (i + 49) : 0;                // se inicializa el tablero con los caracteres del '0' al '9' y se agrega el caracter \0 al final 
    }
}

int main() {
    int playing = 1;                                            // se inicializa la bandera de ejecución para el while
    char* board = (char *)malloc(sizeof(char) * 10);            // se inicializa el tablero con malloc para 10 posiciones (9 para las coordenadas y 1 para el caracter \0)
    int turn = 1;                                               // se inicializa el turno en 1
    int state = 0;                                              // se inicializa el estado del juego en 0
    int pvp = 0;                                                // se inicializa la bandera de jugador contra jugador en 0
    int repeat = 0;                                             // se inicializa la bandera de repetir jugadores en 0
    char** players = malloc(sizeof(char*) * 2);                 // se inicializa el apuntador de apuntadores para los nombres de los jugadores con amlloc para que contenga dos apuntadores de caracteres
    srand(time(NULL));                                          // se inicializa la semilla para los números aleatoreos

    while (playing == 1) {                                      // mientras playing esté en 1, se ejecutará el juego
        init(board, players, &turn, &state, &pvp, repeat);      // se inicializa el juego con la función init

        while (turn > 0 && state == 0) {                        // mientras el turno sea mayor a 0 y el estado del juego se denote como 0
            state = play(board, players, &turn, pvp);           // se ejecuta la función play que permite ingresar la coordenada al jugador correspondiente dependiendo del turno
            system("clear");
        }

        printBoard(&board);                                     // se imprime la última jugada

        if (state > 0) {                                        // si el estado es mayor a 0 (i.e., ganó el jugador 1 o el jugador 2)
            printf("\"%s\" ganó!\n\n", *(players + state - 1)); // se imprime el nombre del jugador ganador utilizando state como la posición del apuntador
            printf("La jugada ganadora fue:\n\n");
            fillBoard(board, checkWin(board, 1));               // se imprime la jugada ganadora utilizando en conjunto las funciones fillBoard y como patrón la jugada ganadora de la máscara que se obtiene con checkWin
            printBoard(&board);
        } else {
            printf("empate :(\n\n");                            // de lo contrario se muestra un mensaje de empate
        }

        int valid = 0;                                          // se inicializa una bandera para corroborar que la entrada sea correcta
        char sel = 'S';                                         // se inicializa el caracter selector para el menú con valor por defecto 'S'

        while (!valid) {                                        // mientras no sea válido el input
            printf("Volver a jugar? S/n ");
            valid = scanf("%c", &sel);

            if (sel == 'S' || sel == 's' || sel == 10) {        // verifica que el valor de sel sea 'S' o 's' o salto de linea (para que haga caso a la opción por defecto S)
                playing = 1;                                    // mantiene la bandera del while en 1
            } else if (sel == 'N' || sel == 'n') {              // si encuentra que el valor de sel es 'N' o 'n'
                playing = 0;                                    // cambia la bandera del while a 0 por lo cual terminará la ejecución
            } else {
                valid = 0;
            }

            if (!valid) {
                printf("Introduzca un valor válido.\n");
            }
        }

        valid = 0;                                              // reinicia el valor de valid a 0

        while (!valid && playing) {
            printf("Usar a los mismos jugadores? S/n ");        // ejecuta un proceso similar, pero en este caso sólo cambia el valor de la bandera repeat, para utilizar los jugadores de la ronda anterior
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