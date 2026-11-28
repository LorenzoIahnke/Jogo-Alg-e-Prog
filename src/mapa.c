#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "jogo.h"
#include <locale.h>
#define TAM_CELULA 30


int carregarMapa(const char *mapa, Jogo *j){
    setlocale(LC_ALL, "Portuguese");

    FILE *f;

    f = fopen(mapa, "r");

    if(f == NULL){
        printf("Impossível abrir arquivo!\n");
        return 0;
    }

    if(fscanf(f, "%d %d", &j->linhas, &j->colunas) != 2){
        printf("Dimensões inválidas!\n");
        fclose(f);
        return 0;
    }

    if(j->linhas < 6 || j->linhas > MAX_LINHAS ||
       j->colunas < 8 || j->colunas > MAX_COLUNAS){
        printf("Dimensões fora do limite!\n");
        fclose(f);
        return 0;
    }

    if(fscanf(f, "%d %d", &j->energia, &j->minimoCristais) != 2){
        printf("Energia ou cristais inválidos!\n");
        fclose(f);
        return 0;
    }

    int countP = 0, countS = 0, totalCristal = 0;
    char c;

    c = fgetc(f);

    if(c == '\r'){
        c = fgetc(f);
    }

    if(c != '\n'){
        printf("Erro na quebra de linha do cabeçalho!\n");
        fclose(f);
        return 0;
    }

    for(int i = 0; i < j->linhas; i++){

        for(int k = 0; k < j->colunas; k++){

            c = fgetc(f);

            if(c == EOF || c == '\n' || c == '\r'){
                printf("Linha %d com tamanho incorreto!\n", i + 1);
                fclose(f);
                return 0;
            }

            if(c == 'P'){
                countP++;
                j->linhaJ = i;
                j->colunaJ = k;
                j->mapa[i][k] = '.';
            }
            else if(c == 'S'){
                countS++;
                j->mapa[i][k] = c;
            }
            else if(c == 'C'){
                totalCristal++;
                j->mapa[i][k] = c;
            }
            else if(c == '.' || c == '#' || c == 'A' ||
                    c == 'R' || c == 'E'){
                j->mapa[i][k] = c;
            }
            else{
                printf("Símbolo inválido na linha %d!\n", i + 1);
                fclose(f);
                return 0;
            }
        }

        c = fgetc(f);

        if(c == '\r'){
            c = fgetc(f);
        }

        if(i < j->linhas - 1 && c != '\n'){
            printf("Linha %d com tamanho incorreto!\n", i + 1);
            fclose(f);
            return 0;
        }

        if(i == j->linhas - 1 && c != '\n' && c != EOF){
            printf("Linha %d com tamanho incorreto!\n", i + 1);
            fclose(f);
            return 0;
        }
    }

    fclose(f);

    if(countP != 1 || countS != 1){
        printf("Deve existir apenas um P e um S!\n");
        return 0;
    }

    if(j->minimoCristais > totalCristal){
        printf("Mínimo de cristais acima do total existente!\n");
        return 0;
    }

    return 1;
}


int movimentoValido(Jogo *j, int comando){
    int nl = j->linhaJ;
    int nc = j->colunaJ;

    moverJogador(&nl, &nc, comando);

    if(nl < 0 || nl >= j->linhas ||
       nc < 0 || nc >= j->colunas){
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
                DrawRectangle(posx, posy, TAM_CELULA,
                              TAM_CELULA, SKYBLUE);
            }
            else{
                DrawRectangle(posx, posy, TAM_CELULA,
                              TAM_CELULA, LIGHTGRAY);

                if(cel == 'C'){
                    DrawCircle(posx + 15, posy + 15, 8, YELLOW);
                }
                else if(cel == 'R'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, GREEN);
                    DrawText("R", posx + 10, posy + 5, 18, WHITE);
                }
                else if(cel == 'A'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, RED);
                    DrawText("A", posx + 12, posy + 5, 18, WHITE);
                }
                else if(cel == 'E'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, PURPLE);
                    DrawText("E", posx + 10, posy + 5, 18, WHITE);
                }
                else if(cel == 'S'){
                    DrawRectangle(posx + 5, posy + 5, 20, 20, BLUE);
                    DrawText("S", posx + 10, posy + 5, 18, WHITE);
                }
            }

            DrawRectangleLines(posx, posy, TAM_CELULA,
                               TAM_CELULA, BLACK);
        }
    }

    int px = j->colunaJ * TAM_CELULA;
    int py = j->linhaJ * TAM_CELULA;

    DrawRectangle(px + 3, py + 3, 24, 24, MAGENTA);
    DrawText("P", px + 9, py + 5, 18, WHITE);

    int starty = j->linhas * TAM_CELULA + 10;

    DrawText(TextFormat("Energia: %d    Cristais: %d/%d    Turnos: %d",
                        j->energia, j->cristais,
                        j->minimoCristais, j->turnos),
             10, starty, 18, MAROON);

    if(strlen(j->mensagem) > 0){
        DrawText(j->mensagem, 10, starty + 28, 16, RED);
    }

    DrawText("Setas ou WASD: MOVER    Q: DESISTIR",
             10, starty + 52, 14, DARKGRAY);

    DrawText("Legenda: P PLAYER  # PAREDE C CRISTAIS S SAIDA R +ENERGIA A ARMADILHA E EVENTO",
             10, starty + 72, 11, RED);
}
