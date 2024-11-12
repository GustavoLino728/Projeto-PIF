#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// Declarações das variáveis
int naveXBaixo = 35; // Posição inicial de NAVEZINHA BAIXO no eixo X
int naveYBaixo = 22; // Posição inicial de NAVEZINHA BAIXO no eixo Y
int naveXCima = 35;  // Posição inicial de NAVEZINHA CIMA no eixo X
int yPosicaoCima = 2; // Posição inicial de NAVEZINHA CIMA no eixo Y

// Códigos ASCII para setas (em sequências de escape ANSI)
#define ESC "\033"
#define ARROW_PREFIX '['
#define LEFT_ARROW 'D'
#define RIGHT_ARROW 'C'

// Pontuação dos jogadores
int pontosBaixo = 0; // Pontos do jogador que controla NAVEZINHA BAIXO
int pontosCima = 0;  // Pontos do jogador que controla NAVEZINHA CIMA

// Definição dos limites da tela
#define LIMITE_SUPERIOR 1
#define LIMITE_INFERIOR 26

void printBalaBaixo(int naveX, int naveY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(naveX + 2, naveY - 1); // Posição um pixel acima da nave
    printf("|");
}

void printBalaCima(int naveX, int naveY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(naveX + 2, naveY + 1); // Posição um pixel abaixo da nave
    printf("|");
}

void clearBala(int x, int startY, int endY, int naveYBaixo, int naveYCima) {
    screenSetColor(BLACK, DARKGRAY); // Define a cor de fundo como preta para "apagar"

    for (int y = startY; y != endY; y += (startY < endY ? 1 : -1)) {
        // Verifica se a posição atual está sobrepondo a nave de baixo
        if (y == naveYBaixo) continue;

        // Verifica se a posição atual está sobrepondo a nave de cima
        if (y == naveYCima) continue;

        screenGotoxy(x + 2, y); // Ajuste para apagar na posição exata da bala
        printf(" "); // Apaga o caractere da bala
    }
    screenUpdate();
}

void printNaveBaixo(int naveX, int naveY) {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(naveX, naveY);
    printf("/-|-/");
}

void printNaveCima(int naveX, int yPosicaoCima) {
    screenSetColor(RED, DARKGRAY);
    screenGotoxy(naveX, yPosicaoCima);
    printf("/-|-/");
}

void displayScore() {
    screenGotoxy(MAXX - 10, 1); // Coloca a pontuação no canto direito superior
    printf("Pontos: %d", pontosBaixo);
    screenGotoxy(2, 1); // Coloca a pontuação do jogador de cima no canto esquerdo superior
    printf("Pontos: %d", pontosCima);
}


int main() 
{
    static int ch = 0;
    int balaYBaixo = naveYBaixo;
    int balaYCima = yPosicaoCima;

    screenInit(1);
    keyboardInit();
    timerInit(50);  // Define o temporizador com um intervalo de 50 ms


    printNaveBaixo(naveXBaixo, naveYBaixo);
    printNaveCima(naveXCima, yPosicaoCima);
    displayScore(); // Exibe a pontuação inicial
    screenUpdate();

    while (ch != 10) // enter
    {
        ch = getchar(); // Lê a tecla pressionada

        if (timerTimeOver()) {
            // Movimenta NAVEZINHA BAIXO para a esquerda ('a') ou direita ('d')
            if (ch == 'a' && naveXBaixo > 0) 
            {
                screenGotoxy(naveXBaixo, naveYBaixo);
                printf("               ");
                naveXBaixo--; 
                printNaveBaixo(naveXBaixo, naveYBaixo);
                displayScore(); 
                screenUpdate();
            }
            else if (ch == 'd' && naveXBaixo < (MAXX - 15)) 
            {
                screenGotoxy(naveXBaixo, naveYBaixo);
                printf("               ");
                naveXBaixo++; 
                printNaveBaixo(naveXBaixo, naveYBaixo);
                displayScore(); 
                screenUpdate();
            }

            // Movimenta NAVEZINHA CIMA para a esquerda ('g') ou direita ('j')
            if (ch == 'g' && naveXCima > 0)
            {
                screenGotoxy(naveXCima, yPosicaoCima);
                printf("               ");
                naveXCima--; 
                printNaveCima(naveXCima, yPosicaoCima);
                displayScore(); 
                screenUpdate();
            }
            else if (ch == 'j' && naveXCima < (MAXX - 15))
            {
                screenGotoxy(naveXCima, yPosicaoCima);
                printf("               ");
                naveXCima++; 
                printNaveCima(naveXCima, yPosicaoCima);
                displayScore(); 
                screenUpdate();
            }

            // Movimenta a bala para cima ao pressionar 'f'
            if (ch == 'f') 
            {
                balaYBaixo = naveYBaixo;
                while (balaYBaixo > LIMITE_SUPERIOR) 
                {
                    if (timerTimeOver()) {
                        clearBala(naveXBaixo, naveYBaixo, balaYBaixo, naveYBaixo, yPosicaoCima);
                        balaYBaixo--;
                        printBalaBaixo(naveXBaixo, balaYBaixo);
                        screenUpdate();

                        // Verifica se a bala bateu na NAVEZINHA CIMA
                        if (balaYBaixo == yPosicaoCima && naveXBaixo == naveXCima || balaYBaixo == yPosicaoCima && naveXBaixo == naveXCima + 1 || balaYBaixo == yPosicaoCima && naveXBaixo == naveXCima - 1 || balaYBaixo == yPosicaoCima && naveXBaixo == naveXCima + 2 || balaYBaixo == yPosicaoCima && naveXBaixo == naveXCima - 2) {
                            pontosBaixo++;
                            displayScore(); 
                            break;
                            }

                        // Se a bala atingir o limite superior, apaga todos os rastros
                        if (balaYBaixo == LIMITE_SUPERIOR) {
                            clearBala(naveXBaixo, naveYBaixo, LIMITE_SUPERIOR, naveYBaixo, yPosicaoCima);
                            break;
                        }
                    }
                }
                // Apaga qualquer rastro restante da bala após a movimentação
                clearBala(naveXBaixo, naveYBaixo, LIMITE_SUPERIOR, naveYBaixo, yPosicaoCima);
            }

            // Movimenta a bala para baixo ao pressionar 'k'
            if (ch == 'k') 
            {
                balaYCima = yPosicaoCima;
                while (balaYCima < LIMITE_INFERIOR) 
                {
                    if (timerTimeOver()) {
                        clearBala(naveXCima, yPosicaoCima, balaYCima, naveYBaixo, yPosicaoCima);
                        balaYCima++;
                        printBalaCima(naveXCima, balaYCima);
                        screenUpdate();

                        // Verifica se a bala bateu na NAVEZINHA BAIXO
                        if (balaYCima == naveYBaixo && naveXCima == naveXBaixo || balaYCima == naveYBaixo && naveXCima == naveXBaixo + 1 || balaYCima == naveYBaixo && naveXCima == naveXBaixo - 1 || balaYCima == naveYBaixo && naveXCima == naveXBaixo + 2 || balaYCima == naveYBaixo && naveXCima == naveXBaixo - 2) {
                            pontosCima++;
                            displayScore(); 
                            break;
                        }

                        // Se a bala atingir o limite inferior, apaga todos os rastros
                        if (balaYCima == LIMITE_INFERIOR) {
                            clearBala(naveXCima, yPosicaoCima, LIMITE_INFERIOR, naveYBaixo, yPosicaoCima);
                            break;
                        }
                    }
                }
                // Apaga qualquer rastro restante da bala após a movimentação
                clearBala(naveXCima, yPosicaoCima, LIMITE_INFERIOR, naveYBaixo, yPosicaoCima);
            }
        }
    }

    return 0;
}
