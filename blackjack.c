"// C¢digo inicial del Blackjack en C" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int obtenerCarta() {
    return rand() % 11 + 1; // Cartas entre 1 y 11
}

int main() {
    srand(time(NULL));

    int jugador = obtenerCarta() + obtenerCarta();
    int computadora = obtenerCarta() + obtenerCarta();

    printf("Tus cartas suman: %d\n", jugador);
    printf("Las cartas de la computadora suman: %d\n", computadora);

    if (jugador > computadora && jugador <= 21) {
        printf("¡Ganaste!\n");
    } else if (computadora > jugador && computadora <= 21) {
        printf("La computadora gana.\n");
    } else {
        printf("Empate o ambos excedieron 21.\n");
    }

    return 0;
}

