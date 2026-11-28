#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "jogo.h"

void processarCasa(Jogo *j){
    char cel = j->mapa[j->linhaJ][j->colunaJ];

    if(cel == 'C'){

        j->cristais++;
        printf("Cristal coletado!\n");
        j->mapa[j->linhaJ][j->colunaJ] = '.';

    }else if(cel == 'R'){

        j->energia += 5;
        printf("Energia recuperada (+5 de energia)!!\n");
        j->mapa[j->linhaJ][j->colunaJ] = '.';

    }else if(cel == 'A'){

        j->energia -= 3;
        printf("Haha caiu em uma armadilha lerdao! (-3 de energia)\n");
        j->mapa[j->linhaJ][j->colunaJ] = '.';

    }else if(cel == 'E'){

        j->cristais++;
        j->energia += 2;
        printf("Evento aleatorio!! +2 de energia!!\n");
        j->mapa[j->linhaJ][j->colunaJ] = '.';

    }
}

int chegouNaSaida(Jogo *j){
    return (j->mapa[j->linhaJ][j->colunaJ] == 'S');
}

int mostrarFimDeJogo(Jogo *j, int resultado){
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        if(resultado == VITORIA){
            DrawText("VITORIA - Voce ganhou!", 20, 30, 22, GREEN);
        }else if(resultado == DERROTA){
            DrawText("DERROTA - Voce perdeu!", 20, 30, 22, RED);
        }else{
            DrawText("DESISTENCIA - Voce desistiu", 20, 30, 22, BLUE);
        }

        DrawText(TextFormat("Cristais: %d (minimo: %d)", j->cristais, j->minimoCristais), 20, 80, 18, BLACK);
        DrawText("\n[R] Jogar Denovo: ", 20, 200, 18, BLUE);
        DrawText("[Q] Sair: ", 20, 200, 18, RED);

        EndDrawing();

        int tecla = GetKeyPressed();
        if(tecla == KEY_R){
            return 1;
        }
        if(tecla == KEY_Q){
            return 0;
        }
    }

    return 0;
}
