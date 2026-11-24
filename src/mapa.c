#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "jogo.h"
#define TAM_CELULA 30

int telaCabe(Jogo *j){
    // ta certo isso??
    (void)j;
    return 1;
}

int carregarMapa(const char *mapa1, Jogo *j){
    FILE *arq = fopen(mapa1, "r");
    if(arq == NULL){
        snprintf(j->mensagem, TAM_MSG, "Impossível abrir arquivo");
        return 0;
    }

    char linhaBuff[MAX_COLUNAS + 3];
    // REGRA LINHAS E COLUNAS
    if(fgets(linhaBuff, sizeof(linhaBuff), arq) == NULL || sscanf(linhaBuff, "%d %d", &j->linhas, &j->colunas) != 2){
        snprintf(j->mensagem, TAM_MSG, "Erro ao ler, dimensoes erradas");
        fclose(arq);
        return 0;
    }

    if(j->linhas < 6 || j->linhas > MAX_LINHAS || j->colunas < 8 || j->colunas > MAX_COLUNAS){
        snprintf(j->mensagem, TAM_MSG, "Erro, dimensoes fora do limite");
        fclose(arq);
        return 0;
    }

    // REGRA CRISTAIS E ENERGIA
    if(fgets(linhaBuff, sizeof(linhaBuff), arq) == NULL || sscanf(linhaBuff, "%d %d", &j->energia, &j->minimoCristais) != 2){
        snprintf(j->mensagem, TAM_MSG, "Invalido valores de energia e/ou cristais");
        fclose(arq);
        return 0;
    }

    int countP = 0, countS = 0, totalCristal = 0;

    for(int i = 0; i < j->linhas; i++){
        if(fgets(linhaBuff, sizeof(linhaBuff), arq) == NULL){
            snprintf(j->mensagem, TAM_MSG, "Arquivo possui menos linhas do que informado");
            fclose(arq);
            return 0;
        }

        linhaBuff[strcspn(linhaBuff, "\r\n")] = '\0';

        if((int)strlen(linhaBuff) != j->colunas){
            snprintf(j->mensagem, TAM_MSG, "Linha %d com tamanho incorreto", i + 1);
            fclose(arq);
            return 0;
        }

        for(int k = 0; k < j->colunas; k++){
            char c = linhaBuff[k];
            if(c == 'P'){
                countP++;
                j->linhaJ = i;
                j->colunaJ = k;
                j->mapa[i][k] = '.';
            }else if(c == 'S'){
                countS++;
                j->mapa[i][k] = c;
            }else if(c == 'C'){
                totalCristal++;
                j->mapa[i][k] = c;
            }else if(c == '.' || c == '#' || c == 'A' || c == 'R' || c == 'E'){
                j->mapa[i][k] = c;
            }else{
                snprintf(j->mensagem, TAM_MSG, "Simbolo '%c' invalido na linha %d", c, i+1);
                fclose(arq);
                return 0;
            }
        }
    }

    fclose(arq);

    if(countP != 1 || countS != 1){
        snprintf(j->mensagem, TAM_MSG, "Erro, deve conter apenas um P (personagem) e um S (saida)");
        return 0;
    }

    if(j->minimoCristais > totalCristal){
        snprintf(j->mensagem, TAM_MSG, "Minimo de cristais acima do total existente");
        return 0;
    }
    return 1;
}


int movimentoValido(Jogo *j, int comando){
    int nl = j->linhaJ;
    int nc = j->colunaJ;

    moverJogador(&nl, &nc, comando);

    if(nl < 0 || nl >= j->linhas || nc < 0 || nc >= j->colunas){
        return 0;
    }
    if(j->mapa[nl][nc] == '#'){
        return 0;
    }
    return 1;
}

void desenharJogo(Jogo *j){
    for(int i = 0; i < j->linhas; i++){
        for(int k = 0; k < j->colunas; k++){
            int posx = k * TAM_CELULA;
            int posy = i * TAM_CELULA;
            char cel = j->mapa[i][k];

            if(cel == '#'){
                DrawRectangle(posx, posy, TAM_CELULA, TAM_CELULA, DARKGRAY);
            }else{
                DrawRectangle(posx, posy, TAM_CELULA, TAM_CELULA, LIGHTGRAY);

                if(cel == 'C'){
                    DrawCircle(posx + 15, posy + 15, 8, GOLD);
                }else if(cel == 'R'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, GREEN);
                    DrawText("+", posx + 10, posy + 5, 18, WHITE);
                }else if(cel == 'A'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, RED);
                    DrawText("!", posx + 12, posy + 5, 18, WHITE);
                }else if(cel == 'E'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, PURPLE);
                    DrawText("?", posx + 10, posy + 5, 18, WHITE);
                }else if(cel == 'S'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, BLUE);
                    DrawText("S", posx + 10, posy + 5, 18, WHITE);
                }
            }
            DrawRectangleLines(posx, posy, TAM_CELULA, TAM_CELULA, GRAY);
        }
    }

    int px = j->colunaJ * TAM_CELULA;
    int py = j->linhaJ * TAM_CELULA;
    DrawRectangle(px + 3, py + 3, 24, 24, MAGENTA);
    DrawText("P", px + 9, py + 5, 18, WHITE);

    int starty = j->linhas * TAM_CELULA + 10;
    DrawText(TextFormat("Energia: %d    Cristais: %d/%d    Turnos: %d",
                        j->energia, j->cristais, j->minimoCristais, j->turnos), 10, starty, 18, MAROON);

    if(strlen(j->mensagem) > 0){
        DrawText(j->mensagem, 10, starty + 28, 16, RED);
    }

    DrawText("Setas ou WASD: MOVER    Q: DESISTIR", 10, starty + 52, 14, DARKGRAY);
    DrawText("Legenda: P PLAYER  # PAREDE C CRISTAIS S SAIDA R +ENERGIA A ARMADILHA E EVENTO",
             10, starty + 72, 11, GRAY);
}
