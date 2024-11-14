#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"


int naveXBaixo = 35; 
int naveYBaixo = 22; 
int naveXCima = 35; 
int yPosicaoCima = 2; 

int pontosBaixo = 0;
int pontosCima = 0; 


#define LIMITE_SUPERIOR 1
#define LIMITE_INFERIOR 26
#define PONTOS_PARA_VENCER 5

void printBalaBaixo(int naveX, int naveY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(naveX + 2, naveY - 1); 
    printf("|");
}

void printBalaCima(int naveX, int naveY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(naveX + 2, naveY + 1); 
    printf("|");
}

void clearBala(int x, int startY, int endY, int naveYBaixo, int naveYCima) {
    screenSetColor(BLACK, DARKGRAY); 

    for (int y = startY; y != endY; y += (startY < endY ? 1 : -1)) {
        
        if (y == naveYBaixo) continue;

        
        if (y == naveYCima) continue;

        screenGotoxy(x + 2, y); 
        printf(" "); 
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

void displayWinner(int jogador) {
    screenClear();
    screenGotoxy(MAXX / 2 - 15, MAXY / 2);
    if (jogador == 1) {
        printf("Parabéns Jogador Baixo! Você ganhou a partida!");
    } else {
        printf("Parabéns Jogador Cima! Você ganhou a partida!");
    }
    screenUpdate();
    sleep(3); 
}

void displayScore() {
    screenGotoxy(MAXX - 10, 1); 
    printf("Pontos: %d", pontosBaixo);
    screenGotoxy(2, 1); 
    printf("Pontos: %d", pontosCima);
}


int main() 
{
    static int ch = 0;
    int balaYBaixo = naveYBaixo;
    int balaYCima = yPosicaoCima;

    screenInit(1);
    keyboardInit();
    timerInit(50);  


    printNaveBaixo(naveXBaixo, naveYBaixo);
    printNaveCima(naveXCima, yPosicaoCima);
    displayScore(); 
    screenUpdate();

    while (ch != 27) 
    {
        ch = readch(); 

        if (timerTimeOver()) {
            if (ch == 'a' && naveXBaixo > 2) 
            {
                screenGotoxy(naveXBaixo, naveYBaixo);
                printf("     ");
                naveXBaixo--; 
                printNaveBaixo(naveXBaixo, naveYBaixo);
                displayScore(); 
                screenUpdate();
            }
            else if (ch == 'd' && naveXBaixo < (MAXX - 6)) 
            {
                screenGotoxy(naveXBaixo, naveYBaixo);
                printf("     ");
                naveXBaixo++; 
                printNaveBaixo(naveXBaixo, naveYBaixo);
                displayScore(); 
                screenUpdate();
            }

        
            if (ch == 'g' && naveXCima > 2)
            {
                screenGotoxy(naveXCima, yPosicaoCima);
                printf("     ");
                naveXCima--; 
                printNaveCima(naveXCima, yPosicaoCima);
                displayScore(); 
                screenUpdate();
            }
            else if (ch == 'j' && naveXCima < (MAXX - 6))
            {
                screenGotoxy(naveXCima, yPosicaoCima);
                printf("     ");
                naveXCima++; 
                printNaveCima(naveXCima, yPosicaoCima);
                displayScore(); 
                screenUpdate();
            }

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

            
                        if (balaYBaixo == yPosicaoCima && abs(naveXBaixo - naveXCima) <= 2) {
                            pontosBaixo++;
                            displayScore();
                            if (pontosBaixo == PONTOS_PARA_VENCER) {
                                displayWinner(1);
                                return 0;
                            }
                            break;
                        }
                    
                        if (balaYBaixo == LIMITE_SUPERIOR) {
                            clearBala(naveXBaixo, naveYBaixo, LIMITE_SUPERIOR, naveYBaixo, yPosicaoCima);
                            break;
                        }
                    }
                }
            
                clearBala(naveXBaixo, naveYBaixo, LIMITE_SUPERIOR, naveYBaixo, yPosicaoCima);
                }
                

           
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

                
                        if (balaYCima == naveYBaixo && abs(naveXCima - naveXBaixo) <= 2) {
                            pontosCima++;
                            displayScore();
                            if (pontosCima == PONTOS_PARA_VENCER) {
                                displayWinner(2);
                                return 0;
                            }
                            break;
                         } 

                        
                        if (balaYCima == LIMITE_INFERIOR) {
                            clearBala(naveXCima, yPosicaoCima, LIMITE_INFERIOR, naveYBaixo, yPosicaoCima);
                            break;
                        }
                    }
                }
               
                clearBala(naveXCima, yPosicaoCima, LIMITE_INFERIOR, naveYBaixo, yPosicaoCima);
            }
        }
    }

    return 0;
}
