#include "bolhas.h"

SDL_Window* g_pWindow=NULL;
SDL_Renderer* g_pRenderer=NULL;

void liberta_coluna(LBOLHAS* aux2){
  LBOLHAS *aux3;

  while(aux2!=NULL){
    aux3=aux2;
    //Copia o endereço guardado em aux2 para aux3

    aux2=aux2->prox;
    //Coloca aux2 a apontar para o endereço

    free(aux3);
    //Liberta o bloco apontado por aux3
  }
  //Percorre toda a coluna até ao último elemento
}
void liberta_colunas(ENTRADA topo[], int largura){
  ENTRADA *aux1;
  LBOLHAS *aux2;

  aux1=topo;
  //Copia o endereço guardado em topo para a variável aux1

  for(int i=0; i<largura; i++){
    aux2=aux1[i].bolha_inicial;
    //Copia o endereço para a primeira bolha da coluna para aux2

    if(aux2!=NULL)
    //Caso nenhuma bolha exista na coluna, evita invocar a função, embora isso
    //não alterasse o resultado do programa
      liberta_coluna(aux2);
      //Liberta a coluna apontada por aux2

    aux1[i].bolha_inicial=NULL;
    //Coloca o apontador de cada entrada para a bolha inicial a NULL para evitar
    //acessos a memória que não pertence ao programa
  }
  //Percorre todas as entradas
}
void desmarca_bolha(ENTRADA topo[], int largura){
  LBOLHAS *aux;

  for(int i=0; i<largura; i++){
    aux=topo[i].bolha_inicial;
    //Copia para a variável auxiliar o endereço da primeira bolha da coluna
    //guardado na entrda dessa coluna
    for(;aux!=NULL; aux=aux->prox){
      if(aux->bolha.marca==CLICADA || aux->bolha.marca==TROCADA)
      //Caso a bolha em que se encontra estiver clicada ou trocada
        aux->bolha.marca=CRIADA;
        //Reinicia a sua marca
    }
    //Percorre toda a coluna
  }
  //Percorre todo o vetor de entradas
}
void desenha_cruz(int linha, int coluna, int cor, int altura, int bolha){
  if(cor==3 || cor==4 || cor==5 || cor==6 || cor==9)
    SDL_SetRenderDrawColor(g_pRenderer, PRETO, 255);
  //Caso a bolha não seja preta, seleciona preto como a cor de desenho
  else
    SDL_SetRenderDrawColor(g_pRenderer, BRANCO, 255);
  //Caso contrário, seleciona branco

  SDL_RenderDrawLine(g_pRenderer, bolha*(2*coluna-1),
                     bolha*2*(altura-linha)+1,
                     bolha*(2*coluna-1),
                     bolha*2*(altura-linha+1));
  //Linha vertical

  SDL_RenderDrawLine(g_pRenderer, bolha*2*(coluna-1),
                     bolha*(2*altura-2*linha+1),
                     bolha*2*coluna,
                     bolha*(2*altura-2*linha+1));
  //Linha horizontal
  //O +1 na ordenada do ponto mais acima tem como objetivo evitar que se apague
  //a linha que do tabuleiro sem serem necessárias outras medidas que
  //tornariam o jogo mais lento
}
void desenha_x(int linha, int coluna, int cor, int altura, int bolha){
  if(cor==3 || cor==4 || cor==5 || cor==6 || cor==9)
    SDL_SetRenderDrawColor(g_pRenderer, PRETO, 255);
  //Caso a bolha não seja preta, seleciona preto como a cor de desenho
  else
    SDL_SetRenderDrawColor(g_pRenderer, BRANCO, 255);
  //Caso contrário, seleciona branco

  SDL_RenderDrawLine(g_pRenderer, bolha*2*(coluna-1),
                     bolha*2*(altura-linha)+1,
                     bolha*2*coluna,
                     bolha*2*(altura-linha+1));
  //Linha com declive negativo

  SDL_RenderDrawLine(g_pRenderer, bolha*2*(coluna-1),
                     bolha*2*(altura-linha+1),
                     bolha*2*coluna,
                     bolha*2*(altura-linha)+1);
  //Linha com declive positivo

  //O +1 na ordenada do ponto mais acima tem como objetivo evitar que se apague
  //a linha que do tabuleiro sem serem necessárias outras medidas que
  //tornariam o jogo mais lento
}
void desenha_marcas(ENTRADA topo[], int largura, int altura, int bolha,
                    int marca){
  LBOLHAS *aux2=NULL;

  for(int i=0; i<largura; i++){
    aux2=topo[i].bolha_inicial;
    //Copia o endereço guardado na entrada da coluna para a variável apontadora
    for(int j=1; aux2!=NULL; aux2=aux2->prox, j++){
      if(aux2->bolha.marca==marca){
        if(marca==CLICADA)
        //Caso esteja apenas clicada desenhalhe uma cruz em cima
          desenha_cruz(j, i+1, aux2->bolha.cor, altura, bolha);
        if(marca==A_ELIMINAR)
        //Caso a bolha seja para eliminar desenha-lhe um x em cima
          desenha_x(j, i+1, aux2->bolha.cor, altura, bolha);
      }
      //Se encontrar alguma bolha clicada, desenha-lhe uma cruz
    }
    //Percorre toda a coluna até ao último elemento
  }
  //Percorre todas as entradas
}

//------------------------------------------------------------------------------
int verifica_comandos(int argc, char *argv[]){
  if(argc!=7 &&  argc!=8){
      printf(ERRO1);
      return 0;
  }
  //Verifica o número de argumentos introduzido
  if(argc==8 && (strcmp(argv[7],"-d")!=0)){
      printf(ERRO2);
      return 0;
  }
  //Em caso de terem sido 8, verifica se o último foi "-d"
  for(int i=1;i<6;i=i+2)
    if(strlen(argv[i])!=2){
      printf(ERRO3);
      return 0;
    }
  //Verifica se foram introduzidos apenas dois carateres para indicar cada opção
  for(int i=1;i<6;i=i+2)
    if(*argv[i]!='-'){
      printf(ERRO3);
      return 0;
    }
  //Verifica se o primeiro carater de cada opção é '-'
  for(int i=1;i<6;i=i+2)
    if(*(argv[i]+1)!='l' && *(argv[i]+1)!='b' && *(argv[i]+1)!='a'){
      printf(ERRO3);
      return 0;
    }
  //Verifica se o segundo carater de cada opção é válido
  if(*(argv[1]+1)==*(argv[3]+1) || *(argv[1]+1)==*(argv[5]+1)
     || *(argv[3]+1)==*(argv[5]+1)){
    printf(ERRO3);
    return 0;
  }
  //Verifica se nenhuma das opções está em repetido
  for(int i=2; i<7; i=i+2){
    for(int k=0; k<strlen(argv[i]); k++){
      if(*(argv[i]+k)<48 || *(argv[i]+k)>57){
        printf(ERRO3);
        return 0;
      }
    }
  }
  //Verifica se os valores para as opções são números

  //Caso nenhum dos erros assinalados tenha sido encontrado
  return 1;
}
int verifica_configs_individual(int argc, char *argv[]){
  for(int i=1;i<6;i=i+2){
  //Percorre os elementos que contêm indicação para a configuração e depois
  //verifica o valor de cada um deles
    if(*(argv[i]+1)=='l')
      if((atoi(argv[i+1]))<L_MIN || atoi(argv[i+1])>L_MAX){
        printf(ERRO4);
        return 0;
      }
    if(*(argv[i]+1)=='b')
      if((atoi(argv[i+1]))<B_MIN || atoi(argv[i+1])>B_MAX){
        printf(ERRO5);
        return 0;
      }
    if(*(argv[i]+1)=='a')
      if((atoi(argv[i+1]))<A_MIN || atoi(argv[i+1])>A_MAX){
        printf(ERRO6);
        return 0;
      }
  }

  //Caso nenhum dos erros assinalados tenha sido encontrado
  return 1;
}
int verifica_configs_relacoes(int argc, char *argv[]){
  for(int i=1; i<6; i=i+2)
  //Percorre os elementos que contêm indicação para a configuração
    if(*(argv[i]+1)=='b'){
    //Se encontrar o do raio
      for(int j=1; j<6;j=j+2)
      //Percorre-os novamente
        if(j!=i){
        //Quando estiver nos outros verifica a sua relação com o raio
          if(*(argv[j]+1)=='a' &&
             (atoi(argv[i+1])*(atoi(argv[j+1]))*2)>A_JANELA_MAX){
            printf(ERRO7);
            return 0;
          }
          if(*(argv[j]+1)=='l' &&
             (atoi(argv[i+1])*(atoi(argv[j+1]))*2)>L_JANELA_MAX){
            printf(ERRO8);
            return 0;
          }
        }
      break;
    }

  //Caso nenhum dos erros assinalados tenha sido encontrado
  return 1;
}
void retira_configs(int argc, char *argv[], CONFIGS *configs_end){
  for(int i=1;i<6;i=i+2){
    if(*(argv[i]+1)=='l')
      configs_end->largura=atoi(argv[i+1]);
    if(*(argv[i]+1)=='b')
      configs_end->bolha=atoi(argv[i+1]);
    if(*(argv[i]+1)=='a')
      configs_end->altura=atoi(argv[i+1]);
  }
  //Retira cada configuração da linha de comandos não interessando a sua posição
  if(argc==8)
    configs_end->debug=1;
  //Se o número de argumentos for 8, assinala que a opção de debug foi ativada
}
CONFIGS le_comandos(int argc, char *argv[]){
  CONFIGS configs;
  //Guardará as configurações retiradas

  configs.largura=0;
  configs.altura=0;
  configs.bolha=0;
  configs.debug=0;
  //Inicializa configs

  if(!verifica_comandos(argc,argv))
  //Se verifica_comandos tiver encontrado algum erro
    exit(0);

  if(!verifica_configs_individual(argc,argv))
  //Se verifica_configs tiver encontrado algum erro
    exit(0);

  if(!verifica_configs_relacoes(argc,argv))
  //Se verifica_configs tiver encontrado algum erro
    exit(0);

  retira_configs(argc,argv,&configs);
  //Retira e armazena em configs os dados introduzidos pelo utilizador

  return configs;
}

void teste_alocacao_entradas(ENTRADA topo[]){
  if (topo==NULL){
    printf("Falta de memoria durante a criacao do vetor de entradas\n");
    exit(0);
  }
  //Se nao for possivel alocar memória, informa o utilizador e
  //termina o programa
}
void inicializa_entradas(ENTRADA topo[], int largura){
  for(int i=0; i<largura; i++)
    topo[i].bolha_inicial=NULL;
  //Inicializa os espaços do vetor dinâmico com o endereço da bolha inicial
  //como NULL
}
ENTRADA *cria_entradas(int largura){
  ENTRADA *topo=NULL;

  topo=(ENTRADA *) calloc(largura, sizeof(ENTRADA));
  //Cria um vetor dinâmico para servir de base de acesso às colunas de bolhas

  teste_alocacao_entradas(topo);

  inicializa_entradas(topo, largura);

  return topo;
}

void cria_semente_aleatoriedade(int debug){
  time_t t;
  //Guarda o tempo de calendário

  if(!debug)
    srand((unsigned) time(&t));
  //Se o programa não estiver em modo de debug, cria uma semente de
  //aleatoriedade a partir do tempo de calendário
  else
    srand(1);
  //Caso contrário, cria-a a partir de 1
}
void teste_alocacao_bolhas(LBOLHAS *novo){
  if (novo==NULL){
    printf("Falta de memoria durante a criacao da lista de bolhas\n");
    exit(0);
  }
  //Se nao for possivel alocar memória, informa o utilizador e
  //termina o programa
}
void inicializa_bolha(LBOLHAS **novo){
  (*novo)->bolha.marca=CRIADA;
  (*novo)->bolha.cor=rand()%9+1;
  (*novo)->prox=NULL;
  //Inicializa os valores de cada bloco bolha
}
void cria_bolhas(CONFIGS configs, ENTRADA topo[]){
  ENTRADA *aux;
  //Guardará o ponto de acesso à lista
  LBOLHAS *novo=NULL, *ultimo=NULL;
  //Guardam os endereços do último e do novo elemento da lista

  cria_semente_aleatoriedade(configs.debug);

  aux=topo;
  //Copia o endereço guardado em topo para aux

  for(int j=0; j<configs.largura; j++){
    for(int i=0;i<configs.altura-2;i++){
    //Enquanto não se criarem tantas bolhas como a altura da janela
      novo=(LBOLHAS*) calloc(1,sizeof(LBOLHAS));
      //Cria um novo elemento para a lista

      teste_alocacao_bolhas(novo);
      inicializa_bolha(&novo);

      if(aux[j].bolha_inicial==NULL){
        aux[j].bolha_inicial=novo;
        ultimo=novo;
      }
      //Se o campo de endereço para a bolha inicial da entrada da coluna em que
      //se está estiver a NULL, atribui-lhe o valor do novo elemento
      else{
        ultimo->prox=novo;
        ultimo=novo;
      }
      //Caso contrário adiciona o novo elemento no final da lista
    }
  }
  //Percorre toda a lista de entradas
}

DADOS inicializa_dados(void){
  DADOS dados_inicializar;

  dados_inicializar.fim=0;
  dados_inicializar.fase=NENHUMA_BOLHA_CLICADA;
  dados_inicializar.vitoria=0;
  dados_inicializar.pontuacao=0;

  dados_inicializar.rato.x=0;
  dados_inicializar.rato.y=0;

  dados_inicializar.marcacao.x=0;
  dados_inicializar.marcacao.y=0;

  return dados_inicializar;
}

void abre_janela(CONFIGS configs){
  if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
  //Se a inicialização for bem sucedida
    g_pWindow = SDL_CreateWindow("!!!Bolhas!!!",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                configs.largura*2*configs.bolha,
                configs.altura*2*configs.bolha, SDL_WINDOW_SHOWN);
    //Cria a janela com o cabeçalho "!!!Bolhas!!!" e as dimensoẽs especificadas

    if(g_pWindow != 0)
    //Se a criação da janela for bem sucedida
      g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
      //Cria o renderer
  }
  else{
    printf("Problemas na inicialização de conteúdos relacionados com a");
    printf(" biblioteca SDL");
    exit(1);
  }
  //Se a inicialização de conteúdos relacionados com a biblioteca SDL não tiver
  //sido possível informa o utilizador e termina o programa
}

void inicializacao(CONFIGS *configs_end, ENTRADA **topo_end, DADOS *dados_end,
                  int argc, char *argv[]){
  *configs_end=le_comandos(argc, argv);
  *topo_end=cria_entradas(configs_end->largura);
  cria_bolhas(*configs_end, *topo_end);
  *dados_end=inicializa_dados();
  abre_janela(*configs_end);
}

//------------------------------------------------------------------------------
int analisa_existencia(POSICAO rato, ENTRADA topo[], int largura, int bolha,
                      int altura){
  LBOLHAS *aux=NULL;
  int n_bolhas=0;

  aux=topo[rato.x/(2*bolha)].bolha_inicial;
  //Descobre a coluna em que está o rato e atribui à auxiliar o endereço da
  //primeira bolha dessa coluna

  if(aux!=NULL)
  //Se a coluna tiver algum elemento
    for(n_bolhas=1; aux->prox!=NULL; n_bolhas++, aux=aux->prox);
    //conta o número de bolhas da coluna

  if(rato.y<=(altura-n_bolhas)*2*bolha)
  //Caso o clique do rato tenha sido acima das bolhas
    return 0;

  return 1;
}
POSICAO calcula_centro(POSICAO clique, int bolha, int altura){
  POSICAO aux;

  aux.x=0;
  aux.y=0;
  //Inicializa a variável auxiliar

  aux.x=bolha*(2*(clique.x/(2*bolha)+1)-1);
  aux.y=bolha*(2*altura-2*(altura-clique.y/(2*bolha))+1);
  //Atribui os resultados calculados variável auxiliar

  return aux;
  //Retorna a posição calculada
}
int verifica_clique(POSICAO aux, POSICAO marcacao, int bolha){
  if((aux.x-marcacao.x==2*bolha || marcacao.x-aux.x==2*bolha) &&
     aux.y==marcacao.y)
        return 1;
  //Caso a bolha clicada esteja horizontalmente adjacente à clicada
  //anteriormente, retorna 1

  if((aux.y-marcacao.y==2*bolha || marcacao.y-aux.y==2*bolha) &&
     aux.x==marcacao.x)
     return 1;
  //Caso a bolha clicada esteja verticalmente adjacente à clicada anteriormente,
  //retorna 1

  return 0;
  //Caso a bolha clicada não esteja de uma forma válida adjacente à já clicada,
  //retorna 0
}
void processa_clique_tabuleiro(DADOS *dados_end, ENTRADA topo[],
                               CONFIGS configs){
  POSICAO aux;

  aux.x=0;
  aux.y=0;
  //Inicializa aux

  if(dados_end->fase==UMA_BOLHA_CLICADA){
    aux=calcula_centro(dados_end->rato, configs.bolha, configs.altura);
    //Guarda a posição do centro da bolha em que se clicou
    if(verifica_clique(aux, dados_end->marcacao, configs.bolha)){
    //Se o clique tiver sido numa bolha adjacente à já clicada
      dados_end->marcacao=aux;
      //Copia a nova posição para marcação
      dados_end->fase=DUAS_BOLHAS_CLICADAS;
    }
    else{
      desmarca_bolha(topo, configs.largura);
      dados_end->fase=NENHUMA_BOLHA_CLICADA;
      dados_end->marcacao.x=0;
      dados_end->marcacao.y=0;
    }
    //Caso contrário, retorna o jogo para a fase inicial
  }
  else if(dados_end->fase==NENHUMA_BOLHA_CLICADA){
    dados_end->fase=UMA_BOLHA_CLICADA;
    dados_end->marcacao=calcula_centro(dados_end->rato, configs.bolha,
                                   configs.altura);
    //Guarda a posição do centro da bolha em que se clicou
  }
}
void input(DADOS *dados_end, SDL_Event event, CONFIGS configs,
                    ENTRADA topo[]){
  SDL_WaitEvent(&event);

  SDL_GetMouseState(&(dados_end->rato.x), &(dados_end->rato.y));

  switch (event.type){
    case SDL_QUIT:
    //Se o evento for do tipo de pedido de saída da aplicação
      dados_end->fim=1;
    break;
    case SDL_MOUSEBUTTONUP:
    //Se o evento for do tipo deixar de clicar no rato
      if(event.button.button==SDL_BUTTON_RIGHT){
      //E se o botão usado tiver sido o direito
        if(dados_end->rato.x>=configs.bolha/2 &&
           dados_end->rato.x<=4.5*configs.bolha &&
           dados_end->rato.y>=configs.bolha/2 &&
           dados_end->rato.y<=3.5*configs.bolha)
        //E se a ponta do rato estiver dentro da caixa "Fim"
          dados_end->fim=1;
        if(dados_end->rato.x>=5*configs.bolha &&
           dados_end->rato.x<=13*configs.bolha &&
           dados_end->rato.y>=configs.bolha/2 &&
           dados_end->rato.y<=3.5*configs.bolha){
        //Se a ponta do rato estiver dentro da caixa "Novo jogo"
          liberta_colunas(topo, configs.largura);
          cria_bolhas(configs, topo);
          dados_end->pontuacao=0;
          dados_end->fase=NENHUMA_BOLHA_CLICADA;
          //Retorna o jogo para a fase inicial
        }
      }
      if(event.button.button==SDL_BUTTON_LEFT &&
         analisa_existencia(dados_end->rato, topo, configs.largura,
                            configs.bolha, configs.altura))
      //Se o clique tiver sido no tabuleiro de bolhas
        processa_clique_tabuleiro(dados_end, topo, configs);
    break;
  }
}

void calcula_lugar(POSICAO marcacao, int altura, int bolha, int aux_relativa[]){
  aux_relativa[LINHA]=altura-0.5*(marcacao.y/bolha-1);
  aux_relativa[COLUNA]=0.5*(marcacao.x/bolha+1);
  //Atribui os valores calculados ao vetor
}
void marca_bolha(POSICAO marcacao, ENTRADA topo[], CONFIGS configs){
  int aux_relativa[2];
  //Guardará a posição relativa na lista e no vetor da bolha que se pretende
  //marcar no formato linha, coluna
  LBOLHAS *aux_lista=NULL;

  calcula_lugar(marcacao, configs.altura, configs.bolha, aux_relativa);

  aux_lista=topo[aux_relativa[COLUNA]-1].bolha_inicial;
  //Atribui a aux_lista o endereço da primeira bolha da coluna em que está a
  //bolha que se pretende marcar

  for(int j=1; aux_lista!=NULL && j<aux_relativa[LINHA];
      j++, aux_lista=aux_lista->prox);
  //Percorre a lista da coluna até que aux_lista passe a apontar para a bolha
  //que se quer marcar

  aux_lista->bolha.marca=CLICADA;
  //Marca a bolha com o valor CLICADA
}

void encontra_clicadas(int largura, LBOLHAS **bolha1, LBOLHAS **bolha2,
                       ENTRADA topo[]){
  int bolhas_encontradas=0;
  LBOLHAS* atual=NULL;

  for(int i=0; i<largura && bolhas_encontradas<2; i++){
  //Percorre todo o vetor de entradas enquanto não tiver encontrado os endereços
  //procurados
    atual=topo[i].bolha_inicial;
    //Copia o endereço para a primeira bolha da coluna guardado na entrada

    for(int j=1; atual!=NULL && bolhas_encontradas<2; atual=atual->prox, j++){
      //Percorre a coluna de bolhas enquanto não tiver encontrado os endereços
      //procurados
      if(atual->bolha.marca==CLICADA){
      //Caso a bolha em que se encontra estiver clicada
        if(bolhas_encontradas==1){
        //Caso já se tenha encontrado uma bolha
          *bolha1=atual;
          bolhas_encontradas=2;
          //Guarda em bolha1 o endereço de uma das bolhas encontradas e atualiza
          //o número de bolhas encontradas
        }
        if(bolhas_encontradas==0){
        //Caso ainda não se tenha encontrado nenhuma bolha
          *bolha2=atual;
          bolhas_encontradas=1;
          //Guarda em bolha1 o endereço de uma das bolhas encontradas e atualiza
          //o número de bolhas encontradas
        }
      }
    }
  }
}
void troca_info(LBOLHAS *bolha1, LBOLHAS *bolha2){
  int aux=0;

  aux=bolha1->bolha.cor;
  bolha1->bolha.cor=bolha2->bolha.cor;
  bolha2->bolha.cor=aux;
  //Copia de uma para a outra as cores de cada uma

  bolha1->bolha.marca=TROCADA;
  bolha2->bolha.marca=TROCADA;
  //Coloca a marca de cada uma a TROCADA
}
void troca_bolhas(ENTRADA topo[], int largura){
  LBOLHAS *bolha1, *bolha2;

  encontra_clicadas(largura, &bolha1, &bolha2, topo);
  //Descobre os endereços de cada bolha a trocar

  troca_info(bolha1, bolha2);
}

int adjacencia_acima(LBOLHAS *a_analisar){
  if(a_analisar->prox!=NULL){
  //Caso exista uma bolha a seguir à que se está a analisar
    if(a_analisar->bolha.cor==a_analisar->prox->bolha.cor &&
       (a_analisar->bolha.marca!=A_ELIMINAR || a_analisar->prox->bolha.marca!=A_ELIMINAR)){
    //e as sua cores sejam iguais e pelo menos uma delas já não esteja marcada
      a_analisar->bolha.marca=A_ELIMINAR;
      a_analisar->prox->bolha.marca=A_ELIMINAR;
      //Marca as bolhas para eliminação
      return 1;
      //Indica que se marcou bolhas
    }
  }

  return 0;
  //Indica que não se marcou bolhas
}
int adjacencia_abaixo(ENTRADA topo[], LBOLHAS *a_analisar, int largura,
                      int linha, int entrada){

  if(a_analisar!=topo[entrada].bolha_inicial){
  //Caso a bolha não seja a primeira da coluna
    LBOLHAS *aux=topo[entrada].bolha_inicial;
    //Inicializa a varíável auxiliar com o endereço da primeira bolha da coluna

    for(int j=1; j<linha-1; j++, aux=aux->prox);
    //Percorre a lista até que aux fique a apontar para o elemento abaixo

    if(aux->bolha.cor==a_analisar->bolha.cor &&
      (a_analisar->bolha.marca!=A_ELIMINAR ||
       aux->bolha.marca!=A_ELIMINAR)){
    //Se as sua cores sejam iguais e pelo menos uma delas já não esteja marcada
      aux->bolha.marca=A_ELIMINAR;
      a_analisar->bolha.marca=A_ELIMINAR;
      //Marca as bolhas para eliminação

      return 1;
      //Indica que se marcou bolhas
    }
  }

  return 0;
  //Indica que não se marcou bolhas
}
int adjacencia_esquerda(ENTRADA topo[], LBOLHAS *a_analisar, int largura,
                        int linha, int entrada){
  int j=0;

  if(entrada!=0 && topo[entrada-1].bolha_inicial!=NULL){
  //Caso não se esteja na primeira coluna
    LBOLHAS *aux=topo[entrada-1].bolha_inicial;
    //Inicializa a varíável auxiliar com o endereço da primeira bolha da coluna
    //à esquerda

    for(j=1; j<linha && aux->prox!=NULL; j++, aux=aux->prox);
    //Percorre a lista até que aux fique a apontar para o elemento ao lado
    //esquerda

    if(j!=linha)
      return 0;
    //Termina se não exisir uma bolha à esquerda

    if(aux->bolha.cor==a_analisar->bolha.cor &&
       (a_analisar->bolha.marca!=A_ELIMINAR ||
        aux->bolha.marca!=A_ELIMINAR)){
    //Se as sua cores forem iguais e pelo menos uma delas já não esteja marcada
      aux->bolha.marca=A_ELIMINAR;
      a_analisar->bolha.marca=A_ELIMINAR;
      //Marca as bolhas para eliminação

      return 1;
      //Indica que se marcou bolhas
    }
  }

  return 0;
  //Indica que não se marcou bolhas
}
int adjacencia_direita(ENTRADA topo[], LBOLHAS *a_analisar, int largura,
                       int linha, int entrada){
  int j=1;

  if(entrada!=largura-1 && topo[entrada+1].bolha_inicial!=NULL){
  //Caso não se esteja na última coluna
    LBOLHAS *aux=topo[entrada+1].bolha_inicial;
    //Inicializa a varíável auxiliar com o endereço da primeira bolha da coluna
    //à direita

    for(; j<linha && aux->prox!=NULL; j++, aux=aux->prox);
    //Percorre a lista até que aux fique a apontar para o elemento ao lado
    //direito

    if(j!=linha)
      return 0;
    //Termina se não exisir uma bolha à direita

    if(aux->bolha.cor==a_analisar->bolha.cor &&
       (a_analisar->bolha.marca!=A_ELIMINAR ||
        aux->bolha.marca!=A_ELIMINAR)){
    //Se as sua cores forem iguais e pelo menos uma delas já não esteja marcada
      aux->bolha.marca=A_ELIMINAR;
      a_analisar->bolha.marca=A_ELIMINAR;
      //Marca as bolhas para eliminação

      return 1;
      //Indica que se marcou bolhas
    }
  }

  return 0;
  //Indica que não se marcou bolhas
}
void marca_acima(ENTRADA topo[], int largura){
  LBOLHAS *aux;

  for(int i=0; i<largura; i++){
    aux=topo[i].bolha_inicial;
    //Copia para a variável auxiliar o endereço da primeira bolha da coluna
    //guardado na entrda dessa coluna
    for(;aux!=NULL; aux=aux->prox){
      if(aux->bolha.marca==A_ELIMINAR){
      //Caso encontre uma bola a eliminar
        for(;aux!=NULL; aux=aux->prox){
          if(aux->bolha.marca!=A_ELIMINAR)
            aux->bolha.marca=TROCADA;
        }
        //Coloca as bolhas a cima com marca trocada
        break;
      }
    }
      //Percorre toda a coluna
  }
  //Percorre todo o vetor de entradas
}
int marca_adjacentes(ENTRADA topo[], int largura){
  LBOLHAS *aux;
  int marcou=1, deve_eliminar=0;

  while(marcou){
  //Enquanto tiver marcado alguma bolha continua
    marcou=0;
    //Altera o valor de marcou para que o ciclo apenas se repita caso neste se
    //marque mais alguma bolha

    for(int i=0; i<largura; i++){
    //Percorre todo o vetor de entradas
      if(topo[i].bolha_inicial!=NULL)
        aux=topo[i].bolha_inicial;
      else
        continue;
      //Copia para a variável auxiliar o endereço da primeira bolha da coluna
      //guardado na entrada dessa coluna

      for(int j=1; aux!=NULL; aux=aux->prox, j++){
      //Percorre toda a coluna
        if(aux->bolha.marca==TROCADA || aux->bolha.marca==A_ELIMINAR){
        //Caso a bolha seja trocada ou a eliminar
          if(adjacencia_acima(aux)
          || adjacencia_abaixo(topo, aux, largura, j, i)
          || adjacencia_esquerda(topo, aux, largura, j, i)
          || adjacencia_direita(topo, aux, largura, j, i)){
          //Caso esteja adjacente a pelo menos uma de outra cor
            marcou=1;
            deve_eliminar=1;
          }
        }
      }
    }
  }

  desmarca_bolha(topo, largura);
  //Desmarca todas as bolhas que não tenham sido marcadas para eliminar e
  //estejam marcadas como trocadas ou clicadas
  marca_acima(topo, largura);
  //Marca já as bolhas que resultarão da eliminação das que foram marcadas
  //para eliminar

  if(deve_eliminar)
  //Caso se deva eliminar alguma bolha
    return 1;

  return 0;
}

void pinta_quadrado_branco(int linha, int coluna, int altura, int bolha){
  SDL_SetRenderDrawColor(g_pRenderer, BRANCO, 255);

  SDL_Rect ret_branco={bolha*2*(coluna-1), bolha*2*(altura-linha)+1,
                       2*bolha, 2*bolha};
  //O +1 na ordenada do ponto tem como objetivo evitar que se apague a linha que
  //separa os botões do tabuleiro sem serem necessárias outras medidas que se
  //tornariam o jogo mais lento

  SDL_RenderFillRect(g_pRenderer, &ret_branco);

  SDL_RenderDrawRect(g_pRenderer, &ret_branco);
}
void pinta_quadrados_brancos(ENTRADA topo[], int largura, int altura,
                             int bolha){
  LBOLHAS *aux2=NULL;

  for(int i=0; i<largura; i++){
    aux2=topo[i].bolha_inicial;
    //Copia o endereço guardado na entrada da coluna para a variável apontadora
    for(int j=1; aux2!=NULL; aux2=aux2->prox, j++){
      if(aux2->bolha.marca==A_ELIMINAR){
        pinta_quadrado_branco(j, i+1, altura, bolha);
      }
      //Se encontrar alguma bolha clicada, desenha-lhe uma cruz
    }
    //Percorre toda a coluna até ao último elemento
  }
  //Percorre todas as entradas

  SDL_RenderPresent(g_pRenderer);
}
void espera_espaco(char str[]){
  do{
    printf("Clique na tecla espaco e no <enter> para avançar\n");
    fflush(stdout);

    fgets(str,100,stdin);
    fflush(stdin);
  } while(*str!=' ');
  //Enquanto o primeiro carater introduzido não for um espaço, não mostra a
  //movimentação
}
void executa_debug(ENTRADA topo[], int largura, int altura, int bolha){
  char str[100];

  desenha_marcas(topo, largura, altura, bolha, A_ELIMINAR);
  //Mostra as bolhas que serão eliminadas com um x
  SDL_RenderPresent(g_pRenderer);

  espera_espaco(str);

  pinta_quadrados_brancos(topo, largura, altura, bolha);

  espera_espaco(str);
}

void elimina(int largura, ENTRADA topo[], int *pontuacao, int *eliminou){
  LBOLHAS *eliminar=NULL, *antes=NULL;

  for(int i=0; i<largura; i++){
    eliminar=topo[i].bolha_inicial;
    //Copia o endereço guardado na entrada para a variável apontadora

    while(eliminar!=NULL){
      if(eliminar->bolha.marca==A_ELIMINAR){
      //Caso a bolha seja para eliminar
        *eliminou=1;
        //Regista a eliminação de pelo menos uma bolha

        if(eliminar!=topo[i].bolha_inicial){
        //e essa bolha não seja a primeira da coluna
          antes->prox=eliminar->prox;
          free(eliminar);
          eliminar=antes->prox;
          (*pontuacao)++;
          //Coloca o bolha anterior a apontar para a seguinte, liberta a
          //memória que ocupava, coloca-a a apontar para a bolha a seguir e
          //atualiza a pontuação
        }
        else{
        //e essa bolha seja a primeira da coluna
          topo[i].bolha_inicial=(topo[i].bolha_inicial)->prox;
          free(eliminar);
          eliminar=topo[i].bolha_inicial;
          (*pontuacao)++;
          //Coloca o bolha anterior a apontar para a seguinte, liberta a
          //memória ocupava, coloca-a a apontar para a bolha a seguir e
          //atualiza a pontuação
        }
      }
      else{
      //Caso contrário, passa-lhe à frente
        antes=eliminar;
        eliminar=eliminar->prox;
      }
    }
    //Percorre toda a coluna
  }
  //Percorre todo o vetor de entradas
}

int verifica_esvaziamento(ENTRADA topo[], int largura){
  for(int i=0; i<largura; i++)
  //Percorre todo o vetor de entradas
    if(topo[i].bolha_inicial!=NULL)
    //Caso uma das entradas contiver o endereço para uma bolha
      return 0;
      //retorna o valor 0 que será interpretado como o tabuleiro não está vazio

  return 1;
  //Caso contrário retorna 1 que será interpretado como o tabuleiro está vazio
}
void atualiza_dados(ENTRADA topo[], DADOS *dados_end, int largura,
                    int eliminou){
  dados_end->marcacao.x=0;
  dados_end->marcacao.y=0;
  //Reinicia a variável marcação

  if(eliminou)
    dados_end->fase=CICLO_DE_ELIMINACAO;
    //Marca a entrada num ciclo de eliminação
  else{
    dados_end->fase=NENHUMA_BOLHA_CLICADA;
    //Retorna ao estado inicial
    desmarca_bolha(topo, largura);
    //Desmarca todas as bolhas que não foram eliminadas
  }

  if(verifica_esvaziamento(topo, largura)){
  //Caso o tabuleiro esteja vazio
    dados_end->fase=NENHUMA_BOLHA_CLICADA;
    dados_end->fim=1;
    dados_end->vitoria=1;
    //Termina o jogo e regista a vitória do jogador
  }
}

void pinta_branco(void){
  SDL_SetRenderDrawColor(g_pRenderer, BRANCO, 255);
  SDL_RenderClear(g_pRenderer);
}
void desenha_bolhas(CONFIGS configs, ENTRADA *topo, int cores[][3]){
  ENTRADA *aux;
  LBOLHAS *atual;

  aux=topo;
  //Copia o endereço guardado em topo para aux

  for(int i=0; i<configs.largura; i++){
    atual=aux[i].bolha_inicial;
    //Copia o endereço para cada lista coluna de bolhas guardado na entrada
    //da coluna

    for(int j=0; atual!=NULL; atual=atual->prox, j++){
      filledCircleRGBA(g_pRenderer, configs.bolha*(i*2+1),
                       configs.bolha*(configs.altura*2-2*j-1), configs.bolha,
                       cores[atual->bolha.cor-1][0],
                       cores[atual->bolha.cor-1][1],
                       cores[atual->bolha.cor-1][2], 255);
      //Desenha o preenchimento da bolha com a cor da bola na matriz
      circleRGBA(g_pRenderer, configs.bolha*(i*2+1),
                 configs.bolha*(configs.altura*2-2*j-1), configs.bolha,
                 PRETO, 255);
      //Desenha o perímetro da bolha a preto
    }
  }
}
void desenha_caixas(CONFIGS configs){
  SDL_SetRenderDrawColor(g_pRenderer, PRETO, 255);
  //Seleciona preto como a cor de desenho

  SDL_RenderDrawLine(g_pRenderer, 0, 4*configs.bolha,
                     configs.largura*2*configs.bolha, 4*configs.bolha);
  //Linha que separa o tabuleiro dos botões de fim e novo jogo

  SDL_Rect caixa_fim={configs.bolha/2, configs.bolha/2, 4*configs.bolha,
                      3*configs.bolha};
  SDL_RenderDrawRect(g_pRenderer, &caixa_fim);
  //Caixa do fim do jogo

  SDL_Rect caixa_novo_jogo={5*configs.bolha, configs.bolha/2, 8*configs.bolha,
                            3*configs.bolha};
  SDL_RenderDrawRect(g_pRenderer, &caixa_novo_jogo);
  //Caixa Novo Jogo

  SDL_Rect caixa_pontos={configs.bolha*(2*configs.largura-12.5),
                         configs.bolha/2, 12*configs.bolha,3*configs.bolha};
  SDL_RenderDrawRect(g_pRenderer, &caixa_pontos);
  //Caixa Pontos
}
void desenha_letras(CONFIGS configs, DADOS dados){
  char fim_txt[]="FIM", novo_jogo_txt[]="NOVO JOGO";
  char pontos_c_bolha_mat[]="0000000";
  char pontos_txt[15]="Pontos:";

  sprintf(pontos_c_bolha_mat, "%d", dados.pontuacao);
  //Transforma a pontuação numa string
  strcat(pontos_txt, pontos_c_bolha_mat);
  //Junta a string da pontuação e a que diz "Pontos: "
  pontos_txt[14]='\0';

  stringRGBA(g_pRenderer, configs.bolha, configs.bolha, fim_txt, PRETO,
             255);
  //Escreve "Fim"
  stringRGBA(g_pRenderer, 5.5*configs.bolha, configs.bolha, novo_jogo_txt,
             PRETO, 255);
  //Escreve dentro da respetiva caixa "Novo Jogo"
  stringRGBA(g_pRenderer, configs.bolha*(configs.largura*2-12),
             configs.bolha, pontos_txt, PRETO, 255);
  //Escreve dentro da respetiva caixa "Pontos: (PONTUAÇÂO)"
}
void rendering(CONFIGS configs, ENTRADA topo[], DADOS dados, int
               cores[][3]){
    pinta_branco();
    desenha_bolhas(configs, topo, cores);
    desenha_marcas(topo, configs.largura, configs.altura, configs.bolha,
                   CLICADA);
    desenha_caixas(configs);
    desenha_letras(configs, dados);

    SDL_RenderPresent(g_pRenderer);
}

void game_loop(DADOS *dados_end, SDL_Event *event_end, CONFIGS configs,
               ENTRADA topo[], int cores[][3]){
  int eliminou;

  while(!dados_end->fim && dados_end->pontuacao<PONTUACAO_MAX){
  //Enquanto o jogo não tiver acabado e a pontuacao for menor
  //que a máxima
    eliminou=0;

    if(dados_end->fase!=CICLO_DE_ELIMINACAO)
    //Caso o jogo não esteja em ciclo de eliminação, analisa o input do jogador
      input(dados_end, *event_end, configs, topo);

    if(dados_end->fase==UMA_BOLHA_CLICADA ||
      dados_end->fase==DUAS_BOLHAS_CLICADAS)
    //Caso o jogo tenha uma ou duas bolhas clicadas, marca a bolha que ainda não
    //foi marcada
      marca_bolha(dados_end->marcacao, topo, configs);

    if(dados_end->fase==DUAS_BOLHAS_CLICADAS){
    //Caso o jogo já tenha duas bolhas clicadas
      troca_bolhas(topo, configs.largura);
      //Troca as bolhas
      if(configs.debug)
        desenha_bolhas(configs, topo, cores);
        //Regista logo o desenho das cores que será apresentado quando se
        //mostrar as bolha a eliminar
    }

    if(dados_end->fase==DUAS_BOLHAS_CLICADAS ||
       dados_end->fase==CICLO_DE_ELIMINACAO){
      //Caso o jogo tenha duas bolha marcadas ou esteja em ciclo de eliminação
      if(marca_adjacentes(topo, configs.largura)){
      //Caso se tenha marcado alguma bolha para eliminar
        if(configs.debug)
        //Caso se esteja em modo de debug
          executa_debug(topo, configs.largura, configs.altura, configs.bolha);

        elimina(configs.largura, topo, &(dados_end->pontuacao), &eliminou);
      }

      atualiza_dados(topo, dados_end, configs.largura, eliminou);
    }

    rendering(configs, topo, *dados_end, cores);
    //No final de cada ciclo, desenha o que foi criado nesse ciclo
  }

  liberta_colunas(topo, configs.largura);
  free(topo);
  //Quando o jogo terminar, liberta as colunas de bolhas que existam e o vetor
  //de entradas

  SDL_Quit();
}

//------------------------------------------------------------------------------
void inicializa_resultados(RESULTADO resultados[]){
  for(int i=0; i<20; i++){
    resultados[i].nome[0]='\0';
    resultados[i].pontuacao=0;
  }
}
void retira_nome(char nome[]){
  do{
    printf("Insira o seu nome sem espaços (máximo de 10 carateres):");
    fgets(nome, DIM_NOME+1, stdin);
    fflush(stdin);
    //Retira o nome do jogador

  } while(*nome=='\0');
  //Enquanto o jogador não escrever um nome, pede-lho

  for(int i=0; i<DIM_NOME; i++)
    if(nome[i]=='\n')
      nome[i]='\0';
  //Transforma o '\n' que surge no meio da string caso o nome introduzido tenha
  //dimensão superior à máxima num '\0'
}
int verifica_leitura(int nconv, int pontuacao, char nome[]){
  int j=0;

  if(nconv!=2){
  //Caso o número de parâmetros por linha seja diferente de 2
    printf("Corrupção do ficheiro de resultados\n");
    printf("Corrija-o e volte a tentar\n");
    exit(0);
    //Informa o jogador e aborta a execução do programa
  }
  if(pontuacao<1 || pontuacao>PONTUACAO_MAX){
  //Caso a conversão não tenha sido possível ou a pontuação retirada não
  //estivesse dentro dos parâmetros normais
    printf("Corrupção do ficheiro de resultados\n");
    printf("Corrija-o e volte a tentar\n");
    exit(0);
    //Informa o jogador e aborta a execução do programa
  }

  for(; j<100; j++)
    if(nome[j]=='\0')
      break;
  //Percorre a string até encontrar o primeiro carater indicador de final
  if(j>DIM_NOME || j==0){
  //Caso esse carater esteja numa posição a seguir à décima primeira ou na
  //primeira
    printf("Corrupção do ficheiro de resultados\n");
    printf("Corrija-o e volte a tentar\n");
    exit(0);
    //Informa o jogador e aborta a execução do programa
  }

  for(int k=0; k<DIM_NOME; k++)
    if(nome[k]==' '){
    //Existe um espaço no meio do nome
      printf("Corrupção do ficheiro de resultados\n");
      printf("Corrija-o e volte a tentar\n");
      exit(0);
      //Informa o jogador e aborta a execução do programa
    }

  return j;
}
void leitura(FILE *pFILE_r_resul, RESULTADO resultados[], int i){
  int nconv, pontuacao=0, j=0;
  char nome[100], aux[100];

  nconv=fscanf(pFILE_r_resul, "%s\t%s", nome, aux);
  pontuacao=atoi(aux);

  j=verifica_leitura(nconv, pontuacao, nome);

  resultados[i].pontuacao=pontuacao;
  //Copia a pontuação para um elemento do vetor resultados
  strncpy(resultados[i].nome, nome, j+1);
  //Copia os elementos antes do '\0' inclusive para um elemento do vetor
  //resultados
}
void retira_resultados(RESULTADO resultados[]){
  FILE* pFILE_r_resul;

  pFILE_r_resul=fopen ("resultados.txt", "r");
  //Abre o ficheiro em modo de leitura

  if(pFILE_r_resul==NULL)
  //Se não se conseguir abrir o ficheiro, termina a execução da função
    return;
  else
    for(int i=0; !feof(pFILE_r_resul); i++)
      leitura(pFILE_r_resul, resultados, i);
    //Enquanto não atingir o final do ficheiro

  fclose(pFILE_r_resul);
  //Fecha o ficheiro
}
void adiciona_novo_resultado(RESULTADO resultados[], char nome[],
                            int pontuacao){
    resultados[19].pontuacao=pontuacao;
    strcpy(resultados[19].nome, nome);
    //Adiciona o novo resultado ao fim da lista
}
void troca_numeros(int *apx, int *apy) {
  int aux;

  aux = *apx;
  *apx = *apy;
  *apy = aux;
}
void troca_nomes(char vx[],char vy[]){
  char aux[20];

  strcpy(aux,vx);
	strcpy(vx,vy);
	strcpy(vy,aux);
}
void ordena_bubble_sort(RESULTADO resultados[]) {
  int troca = 1;
  int i;
  while(troca){
     troca = 0;
     for (i = 0; i <19; ++i)
      if (resultados[i].pontuacao < resultados[i+1].pontuacao){
       troca_numeros(&(resultados[i].pontuacao), &(resultados[i+1].pontuacao));
       troca_nomes((resultados[i].nome), (resultados[i+1].nome));

       troca = 1;
     }
  }
}
void processa_pontuacao(int pontuacao, char aux[]){
  sprintf(aux, "%7d", pontuacao);
  //Transforma a pontuação numa string

  for(int i=0; i<8; i++)
    if(aux[i]==' ')
      aux[i]='0';
  //Substitui os espaços que existiam até ao valor numérico da pontuação por 0's
}
void escreve_resultado(RESULTADO resultados[]){
  FILE* pFILE_r_resul;
  char aux[8];

  pFILE_r_resul=fopen("resultados.txt", "w");
  //Abre o ficheiro no modo de escrita

  for(int linha=0; resultados[linha].pontuacao>0 && linha<N_MAX_RESUL; linha++){
    processa_pontuacao(resultados[linha].pontuacao, aux);

    if(linha!=0)
      fprintf(pFILE_r_resul, "\n");
    //À exceção da primeira linha, passa sempre a escrever numa nova linha
    //o resultado seguinte

    fprintf(pFILE_r_resul,"%s\t%s", resultados[linha].nome, aux);
  }

  fclose(pFILE_r_resul);
  //Fecha o ficheiro
}

void regista_resultado(int pontuacao, int vitoria, RESULTADO resultados[]){
  char nome[DIM_NOME+1];
  //Guarda o nome do jogador

  if(pontuacao==PONTUACAO_MAX || vitoria)
    printf("\nParabéns!\nGanhou o jogo!\n");
  //Saúda o jogador caso este vença o jogo

  printf("\nFim do jogo!\n");
  //Comunica ao jogador o fim do jogo

  if(pontuacao>0){
    inicializa_resultados(resultados);
    //Inicializa o vetor de resultados
    retira_resultados(resultados);
    //Armazena num vetor os dados que estavam escritos em resultados.txt

    if(resultados[19].pontuacao==0 || resultados[19].pontuacao<pontuacao){
    //Se a pontuação do jogador for superior à última guardada no ficheiro
      retira_nome(nome);
      //Pede e recebe do jogador o seu nome
      adiciona_novo_resultado(resultados, nome, pontuacao);
      //Adiciona a pontuação e o nome do jogador ao vetor resultados
      ordena_bubble_sort(resultados);
      //Ordena as pontuacoes
      escreve_resultado(resultados);
      //Escreve os resultados armazenados no vetor no ficeheiro resultados.txt
    }
    else{
      printf("A sua pontuação não foi alta o suficiente para ficar guardada");
      printf("entre os 20 primeiros, tente outra vez!\n");
    }
  }

  printf("Foi um prazer jogar consigo!\n\n");
  //Despede-se do jogador
}
