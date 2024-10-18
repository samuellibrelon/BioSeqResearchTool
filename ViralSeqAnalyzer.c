#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>//para reconhecer os caracteres do portugues

#define MAX_LINES_PER_VIRUS 600  // Número máximo de linhas por vírus
#define MAX_LINE_LENGTH 61   // Tamanho máximo da linha (60 + 1 para o null terminator)
#define MAX_VIRUSES 18000       // Número máximo de vírus no arquivo

typedef struct {
  char nome[60];
  char cpf[12];
  char formacao[60];
  char contato[15];
}Pesquisador;

typedef struct {
  char cpfPesquisador[12];
  char dataDeRealizacao[11]; // formato dd-mm-aaaa
  int tamAlvo; // tamanho da sequência que está está buscando.
  char resultado[1000]; // se achou ou não e qual sequência
}Experimento;

//to pensando em usar...
typedef struct{
int tamanhoDoVirus;
}VirusConvertido;

//MENUS - GERAIS
int menuPrincipal(); //EXIBE O MENU PRINCIPAL
void menuPesquisadores(); //EXIBE O MENU DE PESQUISADORES E CHAMA AS RESPECTIVAS FUNÇÕES
void menuExperimentos(); //EXIBE O MENU DE EXPERIMENTOS E CHAMA AS RESPECTIVAS FUNÇÕES

//FALTA REALIZAR VERIFICAÇÕES NO MOMENTO DE CADASTRAR O CPF
//FUNÇÕES - PESQUISADORES
void inserirPesquisador(); //INSERE UM NOVO Pesquisador NO ARQUIVO Pesquisadores.bin
void listarDadosPesquisadores(); //IMPRIME TODO O ARQUIVO Pesquisadores.bin
void verDadosPesquisador(); //REALIZA UMA BUSCA NOMINAL EM Pesquisadores.bin E IMPRIME OS DADOS DO PESQUISADOR ENCONTRADO
void alterarDadosPesquisador(); //REALIZA UMA BUSCA PELO CPF DE UM PESQUISADOR EM Pesquisadores.bin E PERMITE ALTERAR ALGUM DADO DO MESMO(EXCETO CPF)

//MENU - EXPERIMENTOS
void novoExperimento();//REALIZA UM NOVO EXPERIMENTO E SALVA EM Experimentos.bin
void emitirRelatorios();//LÊ O ARQUIVO Experimentos.bin E EMITE O RELATÓRIO DESEJADO

//ROTINAS PARA REALIZAR EXPERIMENTO
char *** lerVirus();//retorna uma matriz virus[posição][sequencia genética]
int *** converteParaInt(char *** virusEmChar, int qtdVirus);
unsigned long long int *compararPrimeiroeSegundoVirus(unsigned long long int *virus1, unsigned long long int *virus2, int *qtdComuns, VirusConvertido *virusConvertido);//busca os elementos em comum nas duas primeiras sequencias virais
int buscaBinaria(unsigned long long int *vetor, int tamanho, unsigned long long int valor);//função auxiliar a compararPrimeiroeSegundoVirus
unsigned long long int *emComumNoRestoDosVirus(unsigned long long int *emComumNosDoisPrimeiros, unsigned long long int **virusConvertidoOrdenado, int *qtdEmComum, int qtdVirus, VirusConvertido *virusConvertido);
void quicksort(unsigned long long int *v, int inicio, int fim);//ordenação
void troca(unsigned long long int *v, int i, int j);//faz parte da ordenação
unsigned long long int **baseConverter(int ***virusEmInt, int numVirus, int tamanhoEscolhido, VirusConvertido *virusConvertido);//converte para base decimal com base no tamanho escolhido
int isVirusSequence(char *linha);
unsigned long long int realizaConversaoBase10(int *sequenciaViral, int tamanhoEscolhido);//auxilia a baseConvert
char** converterBaseQuatro(int qtdEmComum, unsigned long long int* emComumTodosVirus);//converte de volta para a base 4
char* concatenarStrings(char** strings, int qtd);

//RELATÓRIOS
void experimentoPorPesquisador();//todos os experimentos de um pesquisador.
void experimentoPorAno();//todos os experimentos realizados em um determinado ano.
void experimentosPositivos();//todos os experimentos com resultados positivos.

//VERIFICAÇÕES
int verificarCPF(char *cpfDigitado);
int validaCPF(char *cpf);
int validarData(char *data);

int main(void) {
        setlocale(LC_ALL, "Portuguese");//para usar caracteres do portugues
        int opc;
        do{
          opc = menuPrincipal();
          if(opc == 1){
            menuPesquisadores();
          }
          else if(opc == 2){
            menuExperimentos();
          }
          else if(opc == 3){
            printf("\nEncerrando o programa...");
            exit(1);
          }
          else{
            printf("\nOpcao inválida!\n");
          }
        }while(opc != 3);

        return 0;
      }
int menuPrincipal(){
        int opc;
        printf("------------------------------------------\n");
        printf("          MENU PRINCIPAL           \n");
        printf("------------------------------------------\n");
        printf("Digite o numero da opcao escolhida:\n");
        printf("1 - Menu de Pesquisadores\n");
        printf("2 - Menu de Experimentos\n");
        printf("3 - Encerrar o programa\n");
        printf("\nOpcao Escolhida: ");
        scanf("%d", &opc);
        return opc;
      }
void menuPesquisadores(){
        int opc;
        do{
          printf("------------------------------------------\n");
          printf("          MENU PESQUISADORES           \n");
          printf("------------------------------------------\n");
          printf("Digite o numero da opcao escolhida:\n");
          printf("1 - Inserir novo\n");
          printf("2 - Listar\n");
          printf("3 - Ver os dados\n");
          printf("4 - Alterar dados\n");
          printf("5 - Sair\n");
          printf("\nOpcao Escolhida: ");
          scanf("%d", &opc);

          if(opc == 1){
            inserirPesquisador();
          }
          else if(opc == 2){
            listarDadosPesquisadores();
          }
          else if(opc == 3){
            verDadosPesquisador();
          }
          else if(opc == 4){
            alterarDadosPesquisador();
          }
          else if(opc == 5){
            printf("\nRetornando para o menu principal...\n\n");
          }
          else{
            printf("Opcao inválida!\n\n");
          }
        }while(opc != 5);
      }
void inserirPesquisador() {
        FILE *fp;
        char cpfNovo[12];
        Pesquisador novoPesquisador;

        printf("------------------------------------------\n");
        printf("          CADASTRO PESQUISADOR            \n");
        printf("------------------------------------------\n");
        printf("CPF: ");
        scanf("\n%[^\n]s", cpfNovo);

        if (validaCPF(cpfNovo) == 0) {
          printf("CPF INVÁLIDO! (deve ter 11 dígitos e ser numérico)\n");
          return;
        }
        if (verificarCPF(cpfNovo) != -1) {
          printf("O CPF digitado já está cadastrado no sistema!\n");
          return;
        }
          strcpy(novoPesquisador.cpf, cpfNovo);
          printf("Nome: ");
          scanf("\n%[^\n]s", novoPesquisador.nome);
          printf("Formacao: ");
          scanf("\n%[^\n]s", novoPesquisador.formacao);
          printf("Contato: ");
          scanf("\n%[^\n]s", novoPesquisador.contato);

          // Grava dados no arquivo
          fp = fopen("Pesquisadores.bin", "ab");
          if (fp != NULL) {
            fwrite(&novoPesquisador, sizeof(Pesquisador), 1, fp);
            fclose(fp);
          }
          else {
            printf("Erro ao abrir o arquivo para escrita.\n");
          }
      }
int verificarCPF(char *cpfDigitado) {
        Pesquisador pesquisadoresTemp;
        int controle = 0, encontrou = 0; // Inicializa temp como 0, indicando que o CPF não foi encontrado
        FILE *fp;

        fp = fopen("Pesquisadores.bin", "rb");
        if (fp == NULL) {
          // Se o arquivo não existir, consideramos que o CPF não está registrado
          return -1;
        }

        while (fread(&pesquisadoresTemp, sizeof(Pesquisador), 1, fp)) {
          if (strcmp(pesquisadoresTemp.cpf, cpfDigitado) == 0) {
            encontrou = 1; // CPF encontrado
            break; // Não precisamos continuar lendo o arquivo
          }
          controle++;
        }
        if(encontrou == 1)
          return controle;
        else
          return controle = -1;

        fclose(fp);
      }
int validaCPF(char *cpf) {
    int count = 0;

    // Verifica se o CPF tem exatamente 11 caracteres
    for (int i = 0; cpf[i] != '\0'; i++) {
        // Checa se o caractere é um dígito (caractere entre '0' e '9')
        if (cpf[i] >= '0' && cpf[i] <= '9') {
            count++;
        } else {
            // Se encontrar um caractere que não é dígito, retorna falso (0)
            return 0;
        }
    }

    // Verifica se a quantidade de dígitos é exatamente 11
    return count == 11;
}
void listarDadosPesquisadores(){
        Pesquisador pesquisadores;
        FILE *ptarq;
        ptarq = fopen ("Pesquisadores.bin", "rb");
        printf("\n------------------------------------------\n");
        printf("          PESQUISADORES CADASTRADOS           \n");
        printf("------------------------------------------\n");
        while(fread(&pesquisadores, sizeof (Pesquisador), 1, ptarq)){
          printf("\nNome: %s\nCPF: %s\nFormacao: %s\nContato: %s\n\n", pesquisadores.nome, pesquisadores.cpf, pesquisadores.formacao, pesquisadores.contato);
        }
        printf("------------------------------------------\n\n");
        fclose (ptarq);
      }
void verDadosPesquisador(){
        char nomePesquisadorBuscar[60];
        Pesquisador pesquisadoresTemp;
        int achou = 0;
        FILE *fp;
        fp = fopen("Pesquisadores.bin", "rb");
        printf("\n------------------------------------------\n");
        printf("          VER DADOS DE UM PESQUISADOR          \n");
        printf("------------------------------------------\n");
        printf("Digite o nome do Pesquisador: ");
        scanf("\n%[^\n]s", nomePesquisadorBuscar);

        if (fp == NULL) {
          // Se o arquivo não existir, consideramos que o nome não está registrado
          printf("Ainda nenhum dado registrado!");
        }

        while (fread(&pesquisadoresTemp, sizeof(Pesquisador), 1, fp)) {
          if (strcmp(pesquisadoresTemp.nome, nomePesquisadorBuscar) == 0) {
            printf("\n\nPesquisador Encontado:\n\n");
            printf("\nNome: %s\nCPF: %s\nFormacao: %s\nContato: %s\n\n", pesquisadoresTemp.nome, pesquisadoresTemp.cpf, pesquisadoresTemp.formacao, pesquisadoresTemp.contato);
            achou = 1;
            break; // Não precisamos continuar lendo o arquivo
          }
        }
        if(achou == 0){
          printf("\nNome não encontrado!\n");
        }
        fclose(fp);
      }
void alterarDadosPesquisador(){
        char cpfTemp[12];
        Pesquisador pesquisadoresTemp;
        int controle = 0;
        int opcaoTemp;

        printf("\n------------------------------------------\n");
        printf("          ALTERAR DADOS DO PESQUISADOR           \n");
        printf("------------------------------------------\n");
        printf("CPF do pesquisador: ");
        scanf("\n%[^\n]s", cpfTemp);
        FILE *ptArq;
        ptArq = fopen("Pesquisadores.bin", "r+b");
        if (ptArq == NULL) {
          // Se o arquivo não existir, consideramos que o CPF não está registrado
          printf("Nenhum registro está gravado no arquivo");
        }
        controle = verificarCPF(cpfTemp);//verifica se o cpf está cadastrado e retorna o índice
        fseek(ptArq, (controle)*sizeof(Pesquisador), SEEK_SET);//posiciona o ponteiro no início do registro do pesquisador encontrado
        fread(&pesquisadoresTemp, sizeof(Pesquisador), 1, ptArq); // Lê o registro do arquivo
        printf("\n\nDados Encontrados: \nNome: %s\nCPF: %s\nFormacao: %s\nContato: %s\n\n", pesquisadoresTemp.nome, pesquisadoresTemp.cpf, pesquisadoresTemp.formacao, pesquisadoresTemp.contato);
          if(controle != -1){
            //escolhe o dado que deseja alterar
            printf("Qual dado do pesquisador você deseja alterar (exceto CPF)\n");
            printf("1 - Nome\n");
            printf("2 - Formacao\n");
            printf("3 - Contato\n");
            printf("\nOpcao escolhida: ");
            scanf("%d", &opcaoTemp);
            //altera nome
            if (opcaoTemp == 1) {
                printf("Digite o novo nome: ");
                scanf("\n%[^\n]", pesquisadoresTemp.nome);
                printf("Nome alterado com sucesso!\n");
                //altera formacao
            } else if (opcaoTemp == 2) {
                printf("Digite a nova formaçao: ");
                scanf("\n%[^\n]", pesquisadoresTemp.formacao);
                printf("Formacao alterada com sucesso!\n");
                //altera contato
            } else if (opcaoTemp == 3) {
                printf("Digite o novo contato: ");
                scanf("\n%[^\n]", pesquisadoresTemp.contato);
                printf("Contato alterado com sucesso!\n");
            } else{
                printf("Opcao inválida!\n");
            }
            fseek(ptArq, controle * sizeof(Pesquisador), SEEK_SET); // Retorna ao início do registro
            fwrite (&pesquisadoresTemp, sizeof(Pesquisador), 1, ptArq);
          }
          else {
            printf("Registro não localizado!!!\n\n");
        }
        fclose(ptArq);
      }
void menuExperimentos(){
        int opc;
          do{
            printf("------------------------------------------\n");
            printf("          MENU EXPERIMENTOS           \n");
            printf("------------------------------------------\n");
            printf("Digite o numero da opcao escolhida:\n");
            printf("1 - Novo Experimento\n");
            printf("2 - Relatórios\n");
            printf("3 - Sair\n");
            printf("\nOpcao Escolhida: ");
            scanf("%d", &opc);

            if(opc == 1){
              novoExperimento();
            }
            else if(opc == 2){
              emitirRelatorios();
            }
            else if(opc == 3){
              printf("\nRetornando para o menu principal...\n\n");
            }
            else{
              printf("Opcao invalida!\n\n");
            }
          }while(opc != 3);
      }
void novoExperimento() {
    Experimento novoExperimento;
    FILE *fp;
    char cpfTemp[12];
    int tamanhoTemp;
    int encontrouCpf;
    char ***virus; // matriz virus[posição][sequencia genética]
    int ***virusConvertidoParaInt;
    unsigned long long int **virusConvertidoBaseDez;
    int numVirus; // qtd de vírus
    int qtdComuns = 0;
    unsigned long long int *elementosEmComum;
    unsigned long long int *resultadoFinalEmDigitos;
    char **resultadoFinalBaseQuatro;
    char *resultadoTemp;
    char resultadoNegativo[] = {"Não Encontrado"};

    printf("------------------------------------------\n");
    printf("          NOVO EXPERIMENTO           \n");
    printf("------------------------------------------\n");
    printf("CPF do pesquisador: ");
    scanf("\n%[^\n]s", cpfTemp);
    encontrouCpf = verificarCPF(cpfTemp);

    if (encontrouCpf != -1) {
        strcpy(novoExperimento.cpfPesquisador, cpfTemp);
        printf("Digite a data da realização: ");
        scanf("\n%[^\n]", novoExperimento.dataDeRealizacao);
        if (!validarData(novoExperimento.dataDeRealizacao)) {
            printf("Data Inválida\n\n");
            return; // Encerra a função caso a data seja inválida
        }
        printf("Digite o tamanho do alvo: ");
        scanf("%d", &tamanhoTemp);
        if (tamanhoTemp < 1 || tamanhoTemp > 30) {
            printf("Tamanho Inválido\n\n");
        } else {
            novoExperimento.tamAlvo = tamanhoTemp;
            virus = lerVirus(&numVirus);
            printf("\nLendo arquivo...\n");
            printf("Total de virus encontrados: %d...\n", numVirus);
            VirusConvertido *novoVirus = (VirusConvertido *)malloc(numVirus * sizeof(VirusConvertido));
            if (novoVirus == NULL) {
              printf("Erro na alocação de memória para VirusConvertido.\n");
              exit(1);
            }
            virusConvertidoParaInt = converteParaInt(virus, numVirus); // converte para int
            printf("\nConvertendo arquivo para numeros inteiros...\n");
            // Liberar memória para virus
            for (int i = 0; i < numVirus; i++) {
                int j = 0;
                while (virus[i][j] != NULL) {
                    free(virus[i][j]);
                    j++;
                }
                free(virus[i]);
            }
            free(virus);
            //converte para base 10
            virusConvertidoBaseDez = baseConverter(virusConvertidoParaInt, numVirus, novoExperimento.tamAlvo, novoVirus);
            printf("\nConvertendo virus para base 10...\n");
          // Liberar memória para virusConvertidoParaInt
          for (int i = 0; i < numVirus; i++) {
              int j = 0;
              while (virusConvertidoParaInt[i][j] != NULL) {
                  free(virusConvertidoParaInt[i][j]);
                  j++;
              }
              free(virusConvertidoParaInt[i]);
          }
          free(virusConvertidoParaInt);

            // Ordena os vetores para cada vírus
            for (int i = 0; i < numVirus; i++) {
              quicksort(virusConvertidoBaseDez[i], 0, novoVirus[i].tamanhoDoVirus - 1);
            }
            printf("\nOrdenando Virus...\n");
            //CHAMA FUNÇÃO COMPARAR O PRIMEIRO E SEGUNDO VIRUS ordenados (SALVAR EM VARIÁVEL)
            elementosEmComum = compararPrimeiroeSegundoVirus(virusConvertidoBaseDez[0],virusConvertidoBaseDez[1] ,&qtdComuns, novoVirus);
            printf("\nBuscando elementos em comum nos dois primeiros...\n");
            if(qtdComuns == 0){
              printf("Nenhum Elemento em comum encontrado!\n");
              strcpy(novoExperimento.resultado, resultadoNegativo);
              fp = fopen("Experimentos.bin", "ab");
              if (fp != NULL) {
                fwrite(&novoExperimento, sizeof(Experimento), 1, fp);
                fclose(fp);
              }
              printf("\n\nResultado Salvo!\n\n");
              return;
            }
            else{
              //CHAMA FUNÇÃO PARA pegar o resultado anterior e procurar ele no restante de virusOrdenados, atualiza qtdComuns
              printf("\nBuscando resultado no restante...\n");
              resultadoFinalEmDigitos = emComumNoRestoDosVirus(elementosEmComum, virusConvertidoBaseDez, &qtdComuns, numVirus, novoVirus);
              printf("\nConvertendo de volta para base 4...\n");
              resultadoFinalBaseQuatro = converterBaseQuatro(qtdComuns, resultadoFinalEmDigitos);
              // Concatena todas as strings em uma única string separada por espaços
              resultadoTemp = concatenarStrings(resultadoFinalBaseQuatro, qtdComuns);
              // Atribui o resultado final a novoExperimento.resultado
              strcpy(novoExperimento.resultado, resultadoTemp);
              // Grava dados no arquivo
              fp = fopen("Experimentos.bin", "ab");
              if (fp != NULL) {
                fwrite(&novoExperimento, sizeof(Experimento), 1, fp);
                fclose(fp);
              }
              printf("\n\nResultado Salvo!\n\n");
            }
            //SE TIVER ENCONTRADO, CHAMAR FUNÇÃO PARA CONVERTER EM BASE NITROGENADA E DEPOIS SALVAR EM novoExperimento.resultado
            //SE NÃO TIVER ELEMENTOS EM COMUM, SALVAR "NÃO ENCONTRADO" EM novoExperimento.resultado
            //SALVAR STRUCT EXPERIMENTO NOVOEXPERIMENTO EM ARQUIVO Experimentos.bin
            //printf("Quantidade de vírus lidos: %d\n", numVirus);  //APENAS PARA TESTAR

          /* Imprimir os elementos comuns
          printf("Quantidade de elementos comuns: %d\n", qtdComuns);
          printf("Elementos comuns:\n");
          for (int i = 0; i < qtdComuns; i++) {
              printf("%llu ", resultadoFinalEmDigitos[i]);
          }
          // Imprimir os elementos comuns convertidos para base 4
          printf("\n\n");
          printf("Elementos comuns convertidos:\n");
          for (int i = 0; i < qtdComuns; i++) {
              printf("%s ", resultadoFinalBaseQuatro[i]);
          }*/

          //LIBERAÇÕES DE MEMÓRIA:

          // Liberar memória para virusConvertidoBaseDez
          for (int i = 0; i < numVirus; i++) {
              // Libera a memória alocada para cada vetor de valores convertidos
              free(virusConvertidoBaseDez[i]);
          }

          // Libera a memória alocada para o vetor de ponteiros
          free(virusConvertidoBaseDez);

          // Libera a memória alocada para a struct VirusConvertidos
          free(novoVirus);

          //Libera a memória alocada para o vetor elementosEmCumum
          free(elementosEmComum);

          //Libera a memória alocada para o vetor resultadoFinalEmDigitos
          free(resultadoFinalEmDigitos);

          // Libera a memória alocada
          for (int i = 0; i < qtdComuns; i++) {
              free(resultadoFinalBaseQuatro[i]);
          }
          free(resultadoFinalBaseQuatro);
          free(resultadoTemp);

          }
    } else {
        printf("\nCPF não encontrado!\n");
    }
}
char ***lerVirus(int *numVirus) {
          FILE *pont_arq = fopen("ficticia.txt", "r"); // ficticia.txt é apenas uma base para testar a identificação dos vírus
          if (pont_arq == NULL) {
              printf("ERRO! O arquivo não foi aberto!\n");
              exit(1);
          }

        char linha[MAX_LINE_LENGTH];
        char ***virus = NULL;
        int virusCount = 0;
        int lineCount = 0;

        virus = (char ***)malloc(MAX_VIRUSES * sizeof(char **));
        if (virus == NULL) {
            printf("Erro na alocação de memória para os vírus.\n");
            exit(1);
        }

        for (int i = 0; i < MAX_VIRUSES; i++) {
            virus[i] = NULL;  // Inicializa todos os ponteiros para NULL
        }

        while (fgets(linha, sizeof(linha), pont_arq)) {
            linha[strcspn(linha, "\n")] = '\0'; // Remove o '\n'

            if (linha[0] == '>') { // Identifica início de um novo vírus
                virusCount++;
                lineCount = 0;
                if (virusCount > MAX_VIRUSES) {
                    printf("AVISO! Limite de vírus atingido.\n");
                    break;
                }
                virus[virusCount - 1] = (char **)malloc(MAX_LINES_PER_VIRUS * sizeof(char *));
                if (virus[virusCount - 1] == NULL) {
                    printf("Erro na alocação de memória para as linhas do vírus.\n");
                    exit(1);
                }
                for (int j = 0; j < MAX_LINES_PER_VIRUS; j++) {
                    virus[virusCount - 1][j] = NULL;  // Inicializa todas as linhas para NULL
                }
            } else if (isVirusSequence(linha)) {
                if (virusCount == 0) {
                    printf("Erro: Sequência de vírus encontrada antes de qualquer cabeçalho.\n");
                    exit(1);
                }
                if (lineCount >= MAX_LINES_PER_VIRUS) {
                    printf("AVISO! Limite de linhas para este vírus atingido.\n");
                    break;
                }
                virus[virusCount - 1][lineCount] = (char *)malloc((strlen(linha) + 1) * sizeof(char));
                if (virus[virusCount - 1][lineCount] == NULL) {
                    printf("Erro na alocação de memória para a linha.\n");
                    exit(1);
                }
                strcpy(virus[virusCount - 1][lineCount], linha);
                lineCount++;
            }
        }

        *numVirus = virusCount;
        fclose(pont_arq);
        return virus;
      }
int ***converteParaInt(char ***virusEmChar, int qtdVirus) {
    int ***virusEmInt = (int ***)malloc(qtdVirus * sizeof(int **));
    if (virusEmInt == NULL) {
        printf("Erro na alocação de memória para os vírus em inteiro.\n");
        exit(1);
    }

    for (int i = 0; i < qtdVirus; i++) {
        // Conta o número de linhas para o vírus atual
        int numLinhas = 0;
        while (virusEmChar[i][numLinhas] != NULL) {
            numLinhas++;
        }

        // Aloca memória para as linhas do vírus
        virusEmInt[i] = (int **)malloc((numLinhas + 1) * sizeof(int *)); // +1 para o NULL
        if (virusEmInt[i] == NULL) {
            printf("Erro na alocação de memória para as linhas do vírus em inteiro.\n");
            exit(1);
        }

        for (int j = 0; j < numLinhas; j++) {
            int tamanhoSequencia = strlen(virusEmChar[i][j]);
            virusEmInt[i][j] = (int *)malloc((tamanhoSequencia + 1) * sizeof(int)); // +1 para o '\0'
            if (virusEmInt[i][j] == NULL) {
                printf("Erro na alocação de memória para a sequência do vírus em inteiro.\n");
                exit(1);
            }

            // Converte os caracteres para inteiros
            for (int k = 0; k < tamanhoSequencia; k++) {
                virusEmInt[i][j][k] = virusEmChar[i][j][k] - '0';
            }
            virusEmInt[i][j][tamanhoSequencia] = -1; // Marca o fim da sequência
        }
        virusEmInt[i][numLinhas] = NULL; // Marca o fim das linhas
    }

    return virusEmInt;
}
int isVirusSequence(char *line) {
        if (line[0] == '\0') {
            return 0; // Linha vazia
        }
          for (int i = 0; line[i] != '\0'; i++) {
              if (line[i] < '0' || line[i] > '3') {
                  return 0;  // A linha contém caracteres que não são '0', '1', '2' ou '3'
              }
          }
          return 1;  // A linha contém apenas dígitos válidos
      }
unsigned long long int *compararPrimeiroeSegundoVirus(unsigned long long int *virus1, unsigned long long int *virus2, int *qtdComuns, VirusConvertido *virusConvertido) {
    int capacidadeAtual = 10; // Capacidade inicial mínima
    int contadorElementosIguais = 0;

    // Inicializa o ponteiro para armazenar os elementos comuns
    unsigned long long int *elementosEmComum = (unsigned long long int*)malloc(capacidadeAtual * sizeof(unsigned long long int));
    if (elementosEmComum == NULL) {
        printf("Erro na alocação de memória para elementos em comum.\n");
        exit(1);
    }

    // Vetor auxiliar para verificar duplicados
    unsigned long long int *vetorAuxiliar = (unsigned long long int*)malloc(capacidadeAtual * sizeof(unsigned long long int));
    if (vetorAuxiliar == NULL) {
        printf("Erro na alocação de memória para o vetor auxiliar.\n");
        exit(1);
    }

    for (int i = 0; i < virusConvertido[0].tamanhoDoVirus; i++) {
        if (buscaBinaria(virus2, virusConvertido[1].tamanhoDoVirus, virus1[i])) {
            // Se a capacidade for insuficiente, realoca memória
            if (contadorElementosIguais >= capacidadeAtual) {
                capacidadeAtual *= 2; // Dobra a capacidade
                elementosEmComum = (unsigned long long int*)realloc(elementosEmComum, capacidadeAtual * sizeof(unsigned long long int));
                if (elementosEmComum == NULL) {
                    printf("Erro na realocação de memória para elementos em comum.\n");
                    exit(1);
                }
                vetorAuxiliar = (unsigned long long int*)realloc(vetorAuxiliar, capacidadeAtual * sizeof(unsigned long long int));
                if (vetorAuxiliar == NULL) {
                    printf("Erro na realocação de memória para o vetor auxiliar.\n");
                    exit(1);
                }
            }

            // Verifica se o elemento já foi adicionado e se é diferente de 0
            if (virus1[i] != 0 && !buscaBinaria(vetorAuxiliar, contadorElementosIguais, virus1[i])) {
                // Adiciona o elemento comum e incrementa o contador
                elementosEmComum[contadorElementosIguais] = virus1[i];
                vetorAuxiliar[contadorElementosIguais] = virus1[i];
                contadorElementosIguais++;
            }
        }
    }

    // Ajusta o tamanho final do array de elementos comuns
    elementosEmComum = (unsigned long long int*)realloc(elementosEmComum, contadorElementosIguais * sizeof(unsigned long long int));
    if (elementosEmComum == NULL && contadorElementosIguais > 0) {
        printf("Erro na realocação de memória para elementos em comum.\n");
        exit(1);
    }

    // Define o número total de elementos comuns encontrados
    *qtdComuns = contadorElementosIguais;

    // Libera a memória do vetor auxiliar
    free(vetorAuxiliar);

    return elementosEmComum;
}
int buscaBinaria(unsigned long long int *vetor, int tamanho, unsigned long long int valor) {
      int inicio = 0, fim = tamanho - 1;
      while (inicio <= fim) {
          int meio = (inicio + fim) / 2;
          if (vetor[meio] == valor) {
              return 1; // Valor encontrado
          } else if (vetor[meio] < valor) {
              inicio = meio + 1;
          } else {
              fim = meio - 1;
          }
      }
      return 0; // Valor não encontrado
}
unsigned long long int *emComumNoRestoDosVirus(unsigned long long int *emComumNosDoisPrimeiros, unsigned long long int **virusConvertidoOrdenado, int *qtdEmComum, int qtdVirus, VirusConvertido *virusConvertido) {
    // Aloca memória para armazenar os elementos em comum em todos os vírus
    unsigned long long int *elementosEmComumEmTodosVirus = (unsigned long long int*)malloc(*qtdEmComum * sizeof(unsigned long long int));
    if (elementosEmComumEmTodosVirus == NULL) {
        printf("Erro na alocação de memória para elementos em comum.\n");
        exit(1);
    }

    int qtdEmComumAtualizada = 0;

    // Itera sobre cada elemento em comum dos dois primeiros vírus
    for (int i = 0; i < *qtdEmComum; i++) {
        int encontradoEmTodos = 1; // Assume que o elemento é comum a todos

        // Verifica se o elemento está presente em todos os outros vírus
        for (int j = 2; j < qtdVirus; j++) {
            int tamanhoVirusAtual = virusConvertido[j].tamanhoDoVirus;
            if (!buscaBinaria(virusConvertidoOrdenado[j], tamanhoVirusAtual, emComumNosDoisPrimeiros[i])) {
                encontradoEmTodos = 0;
                break;
            }
        }

        // Se o elemento foi encontrado em todos os vírus, adiciona ao vetor resultado
        if (encontradoEmTodos) {
            elementosEmComumEmTodosVirus[qtdEmComumAtualizada] = emComumNosDoisPrimeiros[i];
            qtdEmComumAtualizada++;
        }
    }

    // Realoca o vetor para o tamanho exato dos elementos em comum encontrados
    if (qtdEmComumAtualizada != *qtdEmComum) {
        elementosEmComumEmTodosVirus = (unsigned long long int*)realloc(elementosEmComumEmTodosVirus, qtdEmComumAtualizada * sizeof(unsigned long long int));
        if (elementosEmComumEmTodosVirus == NULL && qtdEmComumAtualizada > 0) {
            printf("Erro na realocação de memória para elementos em comum.\n");
            exit(1);
        }
    }

    // Atualiza a quantidade de elementos em comum encontrados
    *qtdEmComum = qtdEmComumAtualizada;

    // Se nenhum elemento em comum for encontrado, libera a memória e retorna NULL
    if (*qtdEmComum == 0) {
        free(elementosEmComumEmTodosVirus);
        return NULL;
    }

    return elementosEmComumEmTodosVirus;
}
char** converterBaseQuatro(int qtdEmComum, unsigned long long int* emComumTodosVirus) {
    // Array de strings para armazenar os resultados
    char** resultados = (char**)malloc(qtdEmComum * sizeof(char*));
    if (resultados == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }

    // Mapeamento dos valores base 4 para caracteres
    char mapa[4] = {'A', 'T', 'C', 'G'};

    for (int i = 0; i < qtdEmComum; i++) {
        unsigned long long int numero = emComumTodosVirus[i];

        // Calcula o número de dígitos necessários para armazenar o número na base 4
        int tamanho = 0;
        unsigned long long int temp = numero;
        while (temp > 0) {
            temp /= 4;
            tamanho++;
        }

        // Aloca memória para cada string individual
        resultados[i] = (char*)malloc((tamanho + 1) * sizeof(char)); // +1 para o null terminator
        if (resultados[i] == NULL) {
            printf("Erro na alocação de memória.\n");
            exit(1);
        }
        resultados[i][0] = '\0'; // Inicializa a string como vazia

        // Converte o número para base 4 e constrói a string
        int pos = 0;
        do {
            int resto = numero % 4;
            resultados[i][pos++] = mapa[resto];
            numero /= 4;
        } while (numero > 0);
        resultados[i][pos] = '\0';
        // Reverte a string, pois a conversão é feita do menos significativo para o mais significativo
        int comprimento = strlen(resultados[i]);
        for (int j = 0; j < comprimento / 2; j++) {
            char temp = resultados[i][j];
            resultados[i][j] = resultados[i][comprimento - j - 1];
            resultados[i][comprimento - j - 1] = temp;
        }
        //printf("Número: %llu, Base 4: %s\n", emComumTodosVirus[i], resultados[i]);
    }

    return resultados;
}
unsigned long long int **baseConverter(int ***virusEmInt, int numVirus, int tamanhoEscolhido, VirusConvertido *virusConvertido) {
    unsigned long long int **virusConvertidoBaseDez = (unsigned long long int **)malloc(numVirus * sizeof(unsigned long long int *));
    if (virusConvertidoBaseDez == NULL) {
        printf("Erro na alocação de memória para os vírus convertidos para base 10.\n");
        exit(1);
    }

    for (int i = 0; i < numVirus; i++) {
        // Conta o número de linhas para o vírus atual
        int contadorLinhasPorVirus = 0;
        while (virusEmInt[i][contadorLinhasPorVirus] != NULL) {
            contadorLinhasPorVirus++;
        }

        // Calcula o tamanho total necessário para o vetor convertido
        int tamanhoTotal = contadorLinhasPorVirus * 60;
        if (tamanhoTotal < tamanhoEscolhido) {
            tamanhoTotal = tamanhoEscolhido;
        }

        // Aloca memória para a quantidade de dígitos de cada vírus
        virusConvertidoBaseDez[i] = (unsigned long long int *)malloc(tamanhoTotal * sizeof(unsigned long long int));
        if (virusConvertidoBaseDez[i] == NULL) {
            printf("Erro na alocação de memória para os vírus convertidos para base 10.\n");
            exit(1);
        }

        // Salva o tamanho do vírus na struct
        virusConvertido[i].tamanhoDoVirus = tamanhoTotal;
        int indexConvertido = 0;
        for (int j = 0; j < contadorLinhasPorVirus; j++) {
            for (int k = 0; k < 60; k++) {
                if (indexConvertido < tamanhoTotal) {
                    // Realiza a conversão para base 10
                    virusConvertidoBaseDez[i][indexConvertido] = realizaConversaoBase10(virusEmInt[i][j] + k, tamanhoEscolhido);
                } else {
                    // Preenche as posições restantes com 0
                    virusConvertidoBaseDez[i][indexConvertido] = 0;
                }
                indexConvertido++;
            }
        }
    }

    return virusConvertidoBaseDez;
}
unsigned long long int realizaConversaoBase10(int *sequenciaViral, int tamanhoEscolhido) {
    unsigned long long int virusConvertidoTemp = 0;

    // Realiza a conversão da sequência escolhida para base 10
    for (int i = 0; i < tamanhoEscolhido; i++) {
        virusConvertidoTemp = virusConvertidoTemp * 4 + (unsigned long long int)sequenciaViral[i];
    }

    return virusConvertidoTemp;
}
void quicksort(unsigned long long int *v, int inicio, int fim){
        if (inicio >= fim) return;

        int i = inicio, j = fim;
        unsigned long long int vm = v[(i + j) / 2];

        while (i <= j) {
          while (v[i] < vm) i++;
          while (v[j] > vm) j--;
            if (i <= j) {
              troca(v, i, j);
              i++;
              j--;
            }
        }

        if (inicio < j) quicksort(v, inicio, j);
        if (i < fim) quicksort(v, i, fim);
      }
void troca(unsigned long long int *v, int i, int j) {
        unsigned long long int temp = v[i];
        v[i] = v[j];
        v[j] = temp;
      }
void emitirRelatorios(){
        int opc;
        do{
          printf("------------------------------------------\n");
          printf("          RELATORIOS           \n");
          printf("------------------------------------------\n");
          printf("Digite o numero da opcao escolhida:\n");
          printf("1 - Relatório por pesquisador\n");
          printf("2 - Relatório por ano\n");
          printf("3 - Relatório de resultados positivos\n");
          printf("4 - Sair\n");
          printf("\nOpcao Escolhida: ");
          scanf("%d", &opc);

          if(opc == 1){
            experimentoPorPesquisador();//todos os experimentos de um pesquisador.
          }
          else if(opc == 2){
            experimentoPorAno();//todos os experimentos realizados em um determinado ano.
          }
          else if(opc == 3){
            experimentosPositivos();//todos os experimentos com resultados positivos.
          }
          else if(opc == 4){
            printf("\nRetornando para o menu experimentos...\n\n");
          }
          else{
            printf("Opcao invalida!\n\n");
          }
          }while(opc != 4);
      }
void experimentoPorPesquisador(){
        FILE *ptArq;
        char cpfBuscar[12];
        Experimento experimentosTemp;
        int achou = 0;
        int contador = 1;

        printf("Digite o CPF do Pesquisador: ");
        scanf("\n%[^\n]s", cpfBuscar);

        ptArq = fopen("Experimentos.bin", "rb");
        if (ptArq == NULL) {
          // Se o arquivo não existir, consideramos que o cpf não está registrado
          printf("Ainda nenhum dado registrado!");
          return; // Encerra a função
        }
        //Lê cada experimento e compara o CPF
        while (fread(&experimentosTemp, sizeof(Experimento), 1, ptArq)) {
          if (strcmp(experimentosTemp.cpfPesquisador, cpfBuscar) == 0) {
            if(contador == 1){
              printf("\n\nExperimento(s) do pesquisador de CPF: %s \n\n", cpfBuscar);
            }
            // Imprime os detalhes do experimento
            printf("Experimento %d", contador);
            printf("\nData: %s\nTamanho do alvo: %d\nResultado: %s\n\n", experimentosTemp.dataDeRealizacao, experimentosTemp.tamAlvo, experimentosTemp.resultado);
            achou = 1;
            contador++;
          }
        }
        if(achou == 0){
          printf("\nCPF não encontrado!\n");
        }
        fclose(ptArq);
      }
void experimentoPorAno(){
        FILE *ptArq;
        char anoBuscar[5];
        Experimento experimentosTemp;
        int achou = 0;
        int contador = 1;

        printf("Digite o ano do experimento: ");
        scanf("\n%[^\n]s", anoBuscar);

        ptArq = fopen("Experimentos.bin", "rb");
        if (ptArq == NULL) {
          // Se o arquivo não existir, consideramos que não possui experimentos
          printf("Ainda nenhum dado registrado!\n");
          return; // Encerra a função
        }
        //Lê cada experimento e compara o CPF
        while (fread(&experimentosTemp, sizeof(Experimento), 1, ptArq)) {
          if (strncmp(&experimentosTemp.dataDeRealizacao[6], anoBuscar, 4) == 0) {
            if(contador == 1){
              printf("\n\nExperimento(s) no ano %s \n\n", anoBuscar);
            }
            // Imprime os detalhes do experimento
            printf("Experimento %d", contador);
            printf("\nCPF: %s\nData: %s\nTamanho do alvo: %d\nResultado: %s\n\n", experimentosTemp.cpfPesquisador, experimentosTemp.dataDeRealizacao, experimentosTemp.tamAlvo, experimentosTemp.resultado);
            achou = 1;
            contador++;
          }
        }
        if(achou == 0){
          printf("\nNão foram encontrados experimentos no ano %s\n", anoBuscar);
        }
        fclose(ptArq);
      }
void experimentosPositivos(){
        //experimentos com resultados diferentes de ‘Não Encontrado’
        FILE *ptArq;
        char resultadoTemp[100] = "Não Encontrado";
        Experimento experimentosTemp;
        int achou = 0;
        int contador = 1;


        ptArq = fopen("Experimentos.bin", "rb");
        if (ptArq == NULL) {
            // Se o arquivo não existir, consideramos que não possui experimentos
            printf("Nenhum experimento foi realizado ainda!\n");
            return; // Encerra a função
        }

        // Lê cada experimento e compara o resultado
        while (fread(&experimentosTemp, sizeof(Experimento), 1, ptArq)) {
          //Verifica se o resultado é diferente de "Não Encontrado"
          if (strcmp(experimentosTemp.resultado, resultadoTemp) != 0) {
            if (contador == 1) {
              printf("\n\nExperimento(s) com resultado positivo:\n\n");
            }
            // Imprime os detalhes de cada experimento positivo
            printf("Experimento %d", contador);
            printf("\nCPF: %s\nData: %s\nTamanho do alvo: %d\nResultado: %s\n\n",
              experimentosTemp.cpfPesquisador,
              experimentosTemp.dataDeRealizacao,
              experimentosTemp.tamAlvo,
              experimentosTemp.resultado);
            achou = 1;
            contador++;
          }
        }

        if (achou == 0) {
            printf("\nNão foram encontrados experimentos com resultados positivos\n");
        }
        fclose(ptArq);
      }
int validarData(char *data) {
          int dia, mes, ano;
          if (sscanf(data, "%d-%d-%d", &dia, &mes, &ano) != 3) {
              return 0; // Formato inválido
          }
          if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || ano > 2100) {
              return 0; // Valores inválidos
          }
          return 1;
}
char* concatenarStrings(char** strings, int qtd) {
  // Calcula o tamanho total necessário
  int tamanhoTotal = 0;
  for (int i = 0; i < qtd; i++) {
    tamanhoTotal += strlen(strings[i]);
  }

  // Adiciona o espaço para os delimitadores (espaços) e o terminador nulo
  tamanhoTotal += qtd - 1 + 1; // + (qtd - 1) para os delimitadores e +1 para o terminador nulo

  // Aloca memória para a string final
  char* resultadoFinal = (char*)malloc(tamanhoTotal * sizeof(char));
  if (resultadoFinal == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  // Inicializa a string final como vazia
  resultadoFinal[0] = '\0';

  // Concatena as strings com espaços
  for (int i = 0; i < qtd; i++) {
    strcat(resultadoFinal, strings[i]);
      if (i < qtd - 1) {
        strcat(resultadoFinal, " "); // Adiciona um espaço entre os elementos
      }
  }
  return resultadoFinal;
}
