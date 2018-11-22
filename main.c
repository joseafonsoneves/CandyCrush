#include "bolhas.h"

int main (int argc, char *argv[]){
  CONFIGS configs;
  //Configurações do jogo
  int cores[9][3]={VERMELHO_RGB, ROXO_RGB, AZUL_RGB, CYAN_RGB,
                   VERDE_RGB, AMARELO_RGB, CASTANHO_RGB,
                   PRETO_RGB, BRANCO_RGB};
  //Conjunto de cores das bolhas
  ENTRADA *topo;
  //Apontador para o primeiro elemento do vetor que armazena as colunas
  DADOS dados;
  //Conjunto dos dados de funcionamento necessários ao programa
  SDL_Event event;
  //Estrutura que armazenará dados sobre cada evento introduzido pelo jogador
  RESULTADO resultados[20];
  //Guardará o conjunto de resultados  retirados do ficheiro "resultados.txt"

  inicializacao(&configs, &topo, &dados, argc, argv);

  game_loop(&dados, &event, configs, topo, cores);

  regista_resultado(dados.pontuacao, dados.vitoria, resultados);

  return 0;
}
