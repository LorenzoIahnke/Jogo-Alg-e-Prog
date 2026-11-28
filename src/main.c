#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "jogo.h"
#include <locale.h>
#define TAM_CELULA 30
#define ALTURA_PAINEL 100

int esperarTecla(Jogo *j);

const char* selecionarMapa(){
    InitWindow(600, 400, "Selecione o Mapa");
    SetTargetFPS(60);

    const char* arquivoSelecionado = NULL;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        DrawText("JOGUINHO LEGAL DE ALG E PROG!", 80, 50, 20, BLUE);
        DrawText("Selecione o Mapa", 120, 110, 18, GRAY);

        DrawText("[1] Mapa Fácil", 150, 170, 18, GREEN);
        DrawText("[2] Mapa Normal", 150, 210, 18, YELLOW);
        DrawText("[3] Mapa Difícil", 150, 250, 18, RED);

        EndDrawing();

        if(IsKeyPressed(KEY_ONE)){
            arquivoSelecionado = "mapas/mapa1.txt";
            break;
        }
        else if(IsKeyPressed(KEY_TWO)){
            arquivoSelecionado = "mapas/mapa2.txt";
        }
        else if(IsKeyPressed(KEY_THREE)){
            arquivoSelecionado = "mapas/mapa3.txt";
        }
    }
    CloseWindow();
    return arquivoSelecionado;
}


int main(){
    setlocale(LC_ALL, "Potuguese");
    Jogo jogo;
    int comando, resultado, jogarDenovo;

    do{
        const char *arquivoMapa = selecionarMapa();

        if(arquivoMapa == NULL){
            return 0;
        }

        if(!carregarMapa(arquivoMapa, &jogo)){
            printf("Erro ao carregar %s: %s. \n", arquivoMapa, jogo.mensagem);
            return 1;
        }

        int largura = jogo.colunas * TAM_CELULA;
        if(largura < 500) largura = 500;
        int altura = jogo.linhas * TAM_CELULA + ALTURA_PAINEL;

        InitWindow(largura, altura, "Trabalho");
        SetTargetFPS(60);

        jogo.cristais = 0;
        jogo.turnos = 0;
        jogo.mensagem[0] = '\0';
        resultado = EM_ANDAMENTO;

        while(resultado == EM_ANDAMENTO && !WindowShouldClose()){
            comando = esperarTecla(&jogo);
            jogo.mensagem[0] = '\0';

            if(comando == KEY_Q){
                resultado = DESISTENCIA;
            }else if(movimentoValido(&jogo, comando)){
                moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando);
                jogo.energia--;
                jogo.turnos++;
                processarCasa(&jogo);

                if(chegouNaSaida(&jogo)){
                    if(jogo.cristais >= jogo.minimoCristais){
                        resultado = VITORIA;
                    }else{
                        printf("Cristais insuficientes para sair!\n");
                    }
                }

                if(resultado == EM_ANDAMENTO && jogo.energia <= 0){
                    resultado = DERROTA;
                }
            }else{
                printf("Movimento inválido!!\n");
            }
        }

        if(resultado == EM_ANDAMENTO && WindowShouldClose()){
            resultado = DESISTENCIA;
        }

        jogarDenovo = mostrarFimDeJogo(&jogo, resultado);

        CloseWindow();
    }while(jogarDenovo);

    return 0;
}
