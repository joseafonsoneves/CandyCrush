#ifndef BOLHAS_H
#define BOLHAS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL.h>
#include <time.h>


#define L_MAX 30
#define L_MIN 13
#define A_MAX 23
#define A_MIN 5
#define B_MAX 25
#define B_MIN 10
#define A_JANELA_MAX 600
#define L_JANELA_MAX 1000
#define ERRO1 "DEVEM SER INTRODUZIDOS 7 OU 8 DADOS DE CONFIGURACAO\n"
#define ERRO2 "SE USAR 8 DADOS DE CONFIGURACAO, O ULTIMO DEVERA SER O DEBUG\n"
#define ERRO3 "OS ARGUMENTOS PODEM ESTAR POR QUALQUER ORDEM MAS A SEGUIR A \
CADA UM DEVE VIR O VALOR QUE REPRESENTA DE ACORDO COM O EXEMPLO: \
-l 20 -b 18 -a 17 EM QUE O -l INDICA QUE O NUMERO SEGUINTE \
QUE É INDICADO É A LARGURA DA JANELA EM BOLHAS, -a A ALTURA e -b O RAIO DA \
BOLHA EM PIXEIS(PARA A OPCAO DE DEBUG BASTA ACRESCENTAR -d AO EXEMPLO)\n"
#define ERRO4 "O VALOR DA LARGURA DEVE ESTAR COMPREENDIDO ENTRE 13 E 30\n"
#define ERRO5 "O VALOR DO RAIO DA BOLHA DEVE ESTAR COMPREENDIDO ENTRE 10 E \
25\n"
#define ERRO6 "O VALOR DA ALTURA DEVE ESTAR COMPREENDIDO ENTRE 5 E 23\n"
#define ERRO7 "A ALTURA DA JANELA NÂO DEVE EXCEDER OS 600 PÍXEIS\n"
#define ERRO8 "A LARGURA DA JANELA NÂO DEVE EXCEDER OS 1000 PÍXEIS\n"

#define VERMELHO_RGB {247,  53,  32}
#define ROXO_RGB {153,   0, 150}
#define AZUL_RGB { 67,  47, 249}
#define CYAN_RGB {  0, 255, 255}
#define VERDE_RGB { 89, 223,  55}
#define AMARELO_RGB {245, 252,   7}
#define CASTANHO_RGB {166,  83,   0}
#define PRETO_RGB {  0,   0,   0}
#define BRANCO_RGB {255, 255, 255}
#define PRETO 0,0,0
#define BRANCO 255,255,255

#define NENHUMA_BOLHA_CLICADA 0
#define UMA_BOLHA_CLICADA 1
#define DUAS_BOLHAS_CLICADAS 2
#define CICLO_DE_ELIMINACAO 3
//Índice de significado de fases de jogo

#define CRIADA 0
#define CLICADA 1
#define TROCADA 2
#define A_ELIMINAR 3
//Índice de significados de marcas de bolhas

#define LINHA 0
#define COLUNA 1

#define PONTUACAO_MAX 9999999

#define N_MAX_RESUL 20
#define DIM_NOME 10


typedef struct configs_stru {
  int largura;
  int altura;
  int bolha;
  int debug;
} CONFIGS;

typedef struct bolha_stru{
  int cor;
  int marca;
} BOLHA;

typedef struct lbolhas_stru{
  BOLHA bolha;
  struct lbolhas_stru *prox;
} LBOLHAS;

typedef struct entrada_stru {
  LBOLHAS *bolha_inicial;
} ENTRADA;
//Facilita a interpretação do código, embora não seja imperativa a criação deste
//novo tipo estruturado

typedef struct posicao_stru{
  int x;
  int y;
} POSICAO;

typedef struct dados_stru{
  int fim;
  //Armazena 0 caso ainda não se tenha atingido o fim do jogo e 1 caso isso já
  //tiver acontecido
  int fase;
  //O jogo está dividido em quatro fases: na primeira, nenhuma bolha foi ainda
  //clicada (NENHUMA_BOLHA_CLICADA); na segunda, apenas uma bolha foi clicada,
  //pelo que ainda não se geram mais ações (UMA_BOLHA_CLICADA); na terceira,
  //o jogo realiza-se na sua totalidade com a troca de bolhas e possível
  //eliminação (DUAS_BOLHAS_CLICADAS); na quarta, o jogo vai eliminando e
  //desenhando os resultados da eliminação de bolhas (CLICO_DE_ELIMINACAO)
  int vitoria;
  //Armazena 0 se o jogador não tiver ganho e 1 caso isso tenha acontecido
  int pontuacao;
  POSICAO rato;
  POSICAO marcacao;
} DADOS;

typedef struct resultado_stru{
  char nome[11];
  int pontuacao;
} RESULTADO;


void desenha_marcas(ENTRADA topo[], int largura, int altura, int bolha,
                    int marca);
void liberta_coluna(LBOLHAS* aux2);
void liberta_colunas(ENTRADA topo[], int largura);

//------------------------------------------------------------------------------
int verifica_comandos(int argc, char *argv[]);
int verifica_configs(int argc, char *argv[]);
void retira_configs(int argc, char *argv[], CONFIGS *configs_end);
CONFIGS le_comandos(int argc, char *argv[]);

void teste_alocacao_entradas(ENTRADA topo[]);
void inicializa_entradas(ENTRADA topo[], int largura);
ENTRADA *cria_entradas(int largura);

void cria_semente_aleatoriedade(int debug);
void teste_alocacao_bolhas(LBOLHAS *novo);
void inicializa_bolha(LBOLHAS **novo);
void cria_bolhas(CONFIGS configs, ENTRADA topo[]);

DADOS inicializa_dados(void);

void abre_janela(CONFIGS configs);

void inicializacao(CONFIGS *configs_end, ENTRADA **topo_end, DADOS *dados_end,
                  int argc, char *argv[]);

//------------------------------------------------------------------------------
int analisa_existencia(POSICAO rato, ENTRADA topo[], int largura, int bolha,
                      int altura);
POSICAO calcula_centro(POSICAO clique, int bolha, int altura);
int verifica_clique(POSICAO aux, POSICAO marcacao, int bolha);
void desmarca_bolha(ENTRADA topo[], int largura);
void processa_clique_tabuleiro(DADOS *dados_end, ENTRADA topo[],
                               CONFIGS configs);
void input(DADOS *dados_end, SDL_Event event, CONFIGS configs,
                    ENTRADA topo[]);

void calcula_lugar(POSICAO marcacao, int altura, int bolha, int aux_relativa[]);
void marca_bolha(POSICAO marcacao, ENTRADA topo[], CONFIGS configs);

void encontra_clicadas(int largura, LBOLHAS **bolha1, LBOLHAS **bolha2,
                       ENTRADA topo[]);
void troca_info(LBOLHAS *bolha1, LBOLHAS *bolha2);
void troca_bolhas(ENTRADA topo[], int largura);

int adjacencia_acima(LBOLHAS *a_analisar);
int adjacencia_abaixo(ENTRADA topo[], LBOLHAS *a_analisar, int largura,
                      int linha, int entrada);
int adjacencia_esquerda(ENTRADA topo[], LBOLHAS *a_analisar, int largura,
                        int linha, int entrada);
int adjacencia_direita(ENTRADA topo[], LBOLHAS *a_analisar, int largura,
                       int linha, int entrada);
void marca_acima(ENTRADA topo[], int largura);
int marca_adjacentes(ENTRADA topo[], int largura);

void pinta_quadrado_branco(int linha, int coluna, int altura, int bolha);
void pinta_quadrados_brancos(ENTRADA topo[], int largura, int altura,
                             int bolha);
void espera_espaco(char str[]);
void executa_debug(ENTRADA topo[], int largura, int altura, int bolha);

void elimina(int largura, ENTRADA topo[], int *pontuacao, int *eliminou);

int verifica_esvaziamento(ENTRADA topo[], int largura);
void atualiza_dados(ENTRADA topo[], DADOS *dados_end, int largura,
                    int eliminou);

void pinta_branco(void);
void desenha_bolhas(CONFIGS configs, ENTRADA *topo, int cores[][3]);
void desenha_cruz(int linha, int coluna, int cor, int altura, int bolha);
void desenha_caixas(CONFIGS configs);
void desenha_letras(CONFIGS configs, DADOS dados);
void rendering(CONFIGS configs, ENTRADA topo[], DADOS dados, int
               cores[][3]);

void game_loop(DADOS *dados_end, SDL_Event *event_end, CONFIGS configs,
               ENTRADA topo[], int cores[][3]);

//------------------------------------------------------------------------------
void inicializa_resultados(RESULTADO resultados[]);
void retira_nome(char nome[]);
int verifica_leitura(int nconv, int pontuacao, char nome[]);
void leitura(FILE *pFILE_r_resul, RESULTADO resultados[], int i);
void retira_resultados(RESULTADO resultados[]);
void adiciona_novo_resultado(RESULTADO resultados[], char nome[],
                            int pontuacao);
void troca_numeros(int *apx, int *apy);
void troca_nomes(char vx[],char vy[]);
void ordena_bubble_sort(RESULTADO resultados[]);
void processa_pontuacao(int pontuacao, char aux[]);
void escreve_resultado(RESULTADO resultados[]);

void regista_resultado(int pontuacao, int vitoria, RESULTADO resultados[]);


#endif
