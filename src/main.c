/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int naveXBaixo = 35; // Posição inicial de NAVEZINHA BAIXO no eixo X
int naveYBaixo = 22; // Posição inicial de NAVEZINHA BAIXO no eixo Y
int naveXCima = 35;  // Posição inicial de NAVEZINHA CIMA no eixo X
int yPosicaoCima = 2; // Posição inicial de NAVEZINHA CIMA no eixo Y

// Códigos ASCII para setas (em sequências de escape ANSI)
#define TECLA 27
#define ARROW_PREFIX '['
#define LEFT_ARROW 'D'
#define RIGHT_ARROW 'C'

// Pontuação dos jogadores
int pontosBaixo = 0; // Pontos do jogador que controla NAVEZINHA BAIXO
int pontosCima = 0;  // Pontos do jogador que controla NAVEZINHA CIMA

void printBalaBaixo(int naveX, int naveY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(naveX, naveY - 1); // Posição um pixel acima da nave
    printf("|");
}

void printBalaCima(int naveX, int naveY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(naveX, naveY + 1); // Posição um pixel abaixo da nave
    printf("|");
}

void clearBala(int x, int y) {
    screenGotoxy(x, y);
    printf(" ");
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

int readArrowKey() {
    int ch = readch();
    if (ch == TECLA) {
        if (readch() == ARROW_PREFIX) {
            ch = readch();
            if (ch == LEFT_ARROW || ch == RIGHT_ARROW) {
                return ch;
            }
        }
    }
    return ch;
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
        ch = readArrowKey();

        if (timerTimeOver()) {
            // Movimenta NAVEZINHA BAIXO para a esquerda ('a') ou direita ('d')
            if (ch == 'a' && naveXBaixo > 0) // Limite para não sair da tela
            {
                screenGotoxy(naveXBaixo, naveYBaixo);
                printf("               "); // Apaga a nave na posição atual
                naveXBaixo--; // Move a nave para a esquerda
                printNaveBaixo(naveXBaixo, naveYBaixo);
                displayScore(); // Atualiza o placar
                screenUpdate();
            }
            else if (ch == 'd' && naveXBaixo < (MAXX - 15)) // Limite à direita (ajustar se necessário)
            {
                screenGotoxy(naveXBaixo, naveYBaixo);
                printf("               "); // Apaga a nave na posição atual
                naveXBaixo++; // Move a nave para a direita
                printNaveBaixo(naveXBaixo, naveYBaixo);
                displayScore(); // Atualiza o placar
                screenUpdate();
            }

            // Movimenta NAVEZINHA CIMA para a esquerda (seta esquerda) ou direita (seta direita)
            if (ch == 'g' && naveXCima > 0)
            {
                screenGotoxy(naveXCima, yPosicaoCima);
                printf("               ");
                naveXCima--; // Move a nave para a esquerda
                printNaveCima(naveXCima, yPosicaoCima);
                displayScore(); // Atualiza o placar
                screenUpdate();
            }
            else if (ch == 'j' && naveXCima < (MAXX - 15))
            {
                screenGotoxy(naveXCima, yPosicaoCima);
                printf("               ");
                naveXCima++; // Move a nave para a direita
                printNaveCima(naveXCima, yPosicaoCima);
                displayScore(); // Atualiza o placar
                screenUpdate();
            }

            // Movimenta a bala para cima ao pressionar 'f'
            if (ch == 'f') 
            {
                balaYBaixo = naveYBaixo; // Define a posição inicial da bala como a posição da nave
                while (balaYBaixo > yPosicaoCima) 
                {
                    if (timerTimeOver()) {
                        clearBala(naveXBaixo, balaYBaixo);       // Apaga a bala na posição atual
                        balaYBaixo--;                            // Atualiza a posição da bala para o próximo y
                        printBalaBaixo(naveXBaixo, balaYBaixo);  // Desenha a bala na nova posição acima da nave
                        screenUpdate();

                        // Verifica se a bala bateu na NAVEZINHA CIMA
                        if (balaYBaixo == yPosicaoCima && balaYBaixo < naveYBaixo) {
                            pontosCima++; // Adiciona ponto ao jogador de cima
                            displayScore(); // Atualiza o placar
                            break; // Sai do loop ao atingir a nave
                        }
                    }
                }
                clearBala(naveXBaixo, balaYBaixo); // Apaga a bala na última posição ao atingir y = 2
            }

            // Movimenta a bala para baixo ao pressionar 'k'
            if (ch == 'k') 
            {
                balaYCima = yPosicaoCima; // Define a posição inicial da bala como a posição da nave
                while (balaYCima < naveYBaixo) 
                {
                    if (timerTimeOver()) {
                        clearBala(naveXCima, balaYCima);         // Apaga a bala na posição atual
                        balaYCima++;                             // Atualiza a posição da bala para o próximo y
                        printBalaCima(naveXCima, balaYCima);     // Desenha a bala na nova posição abaixo da nave
                        screenUpdate();

                        // Verifica se a bala bateu na NAVEZINHA BAIXO
                        if (balaYCima == naveYBaixo && balaYCima > yPosicaoCima) {
                            pontosBaixo++; // Adiciona ponto ao jogador de baixo
                            displayScore(); // Atualiza o placar
                            break; // Sai do loop ao atingir a nave
                        }
                    }
                }
                clearBala(naveXCima, balaYCima); // Apaga a bala na última posição ao atingir y = 22
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
