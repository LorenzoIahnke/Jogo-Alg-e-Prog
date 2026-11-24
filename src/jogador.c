#include "raylib.h"
#include "jogo.h"

void moverJogador(int *linha, int *coluna, int comando){
    if(comando == KEY_D || comando == KEY_RIGHT){
        (*coluna)++;
    }else if(comando == KEY_A || comando == KEY_LEFT){
        (*coluna)--;
    }else if(comando == KEY_W || comando == KEY_UP){
        (*linha)--;
    }else if(comando == KEY_S || comando == KEY_DOWN){
        (*linha)++;
    }
}


int esperarTecla(Jogo *j){
    int tecla = 0;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        desenharJogo(j);
        EndDrawing();

        tecla = GetKeyPressed();
        if(tecla != 0){
            return tecla;
        }
    }
    return KEY_Q;
}
