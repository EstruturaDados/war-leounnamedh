// ============================================================================
//      PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (NÍVEL MESTRE)
// ============================================================================
//
// OBJETIVO:
// - Dividir o código em funções bem definidas (modularização total).
// - Implementar um sistema de missões aleatórias.
// - Verificar o cumprimento da missão para determinar a vitória.
// - Aplicar boas práticas (uso de const, organização, etc.).
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias
#include <stdio.h>      // Para funções de entrada/saída
#include <string.h>     // Para manipulação de strings
#include <stdlib.h>     // Para alocação de memória e números aleatórios
#include <time.h>       // Para inicializar a semente de números aleatórios
#include <stdbool.h>    // Para usar o tipo de dado 'bool' (true/false)

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define MAX_NOME 51
#define MAX_COR 21
#define JOGADOR_COR "Azul" // Define a cor do jogador para fácil referência

// --- Estruturas de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numeroTropas;
} Territorio;

// Enum para os tipos de missão
typedef enum {
    DESTRUIR_EXERCITO,
    CONQUISTAR_TERRITORIOS
} TipoMissao;

// Estrutura para a missão do jogador
typedef struct {
    TipoMissao tipo;
    char alvoCor[MAX_COR]; // Usado para DESTRUIR_EXERCITO
    int alvoQuantidade;    // Usado para CONQUISTAR_TERRITORIOS
} Missao;

// Estrutura principal para agrupar o estado do jogo
typedef struct {
    Territorio* mapa;
    Missao missaoAtual;
    bool missaoCompleta;
} Jogo;

// --- Protótipos das Funções ---

// Funções de Gerenciamento do Jogo
void inicializarJogo(Jogo *jogo);
void executarLoopPrincipal(Jogo *jogo);
void encerrarJogo(Jogo *jogo);

// Funções de Interface e Menu
void exibirMenu();
void exibirMapa(const Territorio *mapa);
void limparBufferEntrada();

// Funções de Lógica do Jogo
void faseDeAtaque(Territorio *mapa);
void simularBatalha(Territorio *mapa, int idAtacante, int idDefensor);

// Funções do Sistema de Missão
void atribuirMissaoAleatoria(Missao *missao);
void exibirMissao(const Missao *missao);
bool verificarCumprimentoMissao(const Territorio *mapa, const Missao *missao);

// --- Função Principal (main) ---
int main() {
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    Jogo jogo; // Cria a estrutura principal do jogo

    inicializarJogo(&jogo); // Prepara tudo: aloca mapa, define territórios e atribui missão

    if (jogo.mapa != NULL) {
        executarLoopPrincipal(&jogo); // Inicia o loop principal do jogo
        encerrarJogo(&jogo);          // Libera a memória alocada
    }

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Aloca o mapa, define os territórios iniciais e atribui uma missão.
 * @param jogo Ponteiro para a estrutura principal do jogo.
 */
void inicializarJogo(Jogo *jogo) {
    printf("Inicializando o Desafio WAR - Nível Mestre...\n");
    
    // 1. Aloca memória para o mapa
    jogo->mapa = (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (jogo->mapa == NULL) {
        printf("Erro: Falha ao alocar memória para o mapa!\n");
        return; // Retorna para main, onde o programa será encerrado
    }

    // 2. Define os territórios iniciais (Inicialização Automática)
    strcpy(jogo->mapa[0].nome, "Brasil");
    strcpy(jogo->mapa[0].corExercito, "Azul");
    jogo->mapa[0].numeroTropas = 5;

    strcpy(jogo->mapa[1].nome, "Argentina");
    strcpy(jogo->mapa[1].corExercito, "Verde");
    jogo->mapa[1].numeroTropas = 3;

    strcpy(jogo->mapa[2].nome, "Peru");
    strcpy(jogo->mapa[2].corExercito, "Verde");
    jogo->mapa[2].numeroTropas = 2;

    strcpy(jogo->mapa[3].nome, "Chile");
    strcpy(jogo->mapa[3].corExercito, "Azul");
    jogo->mapa[3].numeroTropas = 4;

    strcpy(jogo->mapa[4].nome, "Colombia");
    strcpy(jogo->mapa[4].corExercito, "Vermelho");
    jogo->mapa[4].numeroTropas = 3;
    
    // 3. Atribui uma missão aleatória
    atribuirMissaoAleatoria(&jogo->missaoAtual);
    jogo->missaoCompleta = false;
    
    printf("\nBem-vindo ao campo de batalha!\n");
    exibirMissao(&jogo->missaoAtual);
}

/**
 * @brief Controla o fluxo principal do jogo, exibindo o menu e processando as ações.
 * @param jogo Ponteiro para a estrutura principal do jogo.
 */
void executarLoopPrincipal(Jogo *jogo) {
    int opcao;
    do {
        exibirMapa(jogo->mapa);
        exibirMenu();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(jogo->mapa);
                // Após cada ataque, verifica se a missão foi cumprida
                jogo->missaoCompleta = verificarCumprimentoMissao(jogo->mapa, &jogo->missaoAtual);
                break;
            case 2:
                printf("\n--- VERIFICANDO MISSÃO ---\n");
                exibirMissao(&jogo->missaoAtual);
                if (verificarCumprimentoMissao(jogo->mapa, &jogo->missaoAtual)) {
                    printf("Status: MISSÃO COMPLETA!\n");
                } else {
                    printf("Status: Em andamento.\n");
                }
                break;
            case 0:
                printf("\nEncerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0 && !jogo->missaoCompleta);

    // Se o loop terminou por a missão ter sido completada
    if (jogo->missaoCompleta) {
        printf("\n\n==============================================\n");
        printf("   !!! PARABÉNS, VOCÊ VENCEU O JOGO !!!\n");
        printf("   Sua missão foi cumprida com sucesso!\n");
        printf("==============================================\n");
        exibirMapa(jogo->mapa); // Mostra o estado final do mapa
    }
}

/**
 * @brief Libera a memória alocada para o mapa.
 * @param jogo Ponteiro para a estrutura principal do jogo.
 */
void encerrarJogo(Jogo *jogo) {
    free(jogo->mapa);
    jogo->mapa = NULL; // Boa prática para evitar 'dangling pointers'
    printf("Memória do mapa liberada com sucesso.\n");
}

/**
 * @brief Exibe o menu principal de opções para o jogador.
 */
void exibirMenu() {
    printf("\nEscolha sua ação:\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair do Jogo\n");
    printf("Opção: ");
}

/**
 * @brief Gerencia a interface para a ação de ataque.
 * @param mapa Ponteiro para o mapa do mundo.
 */
void faseDeAtaque(Territorio *mapa) {
    int idAtacante, idDefensor;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o território ATACANTE (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &idAtacante);
    printf("Escolha o território DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &idDefensor);
    limparBufferEntrada();

    if (idAtacante < 1 || idAtacante > NUM_TERRITORIOS || idDefensor < 1 || idDefensor > NUM_TERRITORIOS) {
        printf("\nErro: Número de território inválido!\n");
        return;
    }
    if (idAtacante == idDefensor) {
        printf("\nErro: Você não pode atacar o mesmo território!\n");
        return;
    }

    int idxAtacante = idAtacante - 1;
    int idxDefensor = idDefensor - 1;
    
    // Valida se o atacante pertence ao jogador
    if (strcmp(mapa[idxAtacante].corExercito, JOGADOR_COR) != 0) {
        printf("\nErro: Você só pode atacar com exércitos da sua cor (%s)!\n", JOGADOR_COR);
        return;
    }
    if (strcmp(mapa[idxAtacante].corExercito, mapa[idxDefensor].corExercito) == 0) {
        printf("\nErro: Você não pode atacar um território que já é seu!\n");
        return;
    }
    if (mapa[idxAtacante].numeroTropas < 2) {
        printf("\nErro: Você precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    simularBatalha(mapa, idxAtacante, idxDefensor);
}

/**
 * @brief Executa a lógica de uma batalha entre dois territórios.
 * @param mapa Ponteiro para o mapa do mundo.
 * @param idAtacante Índice do território atacante.
 * @param idDefensor Índice do território defensor.
 */
void simularBatalha(Territorio *mapa, int idAtacante, int idDefensor) {
    printf("\n>>> Batalha: %s (%s) ataca %s (%s) <<<\n",
           mapa[idAtacante].nome, mapa[idAtacante].corExercito,
           mapa[idDefensor].nome, mapa[idDefensor].corExercito);
           
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("--- Rolagem de Dados ---\n");
    printf("Ataque rolou: %d\n", dadoAtaque);
    printf("Defesa rolou: %d\n", dadoDefesa);
    
    if (dadoAtaque > dadoDefesa) { // Modificado: Ataque vence apenas se o dado for maior
        printf("\nVITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        mapa[idDefensor].numeroTropas--;

        if (mapa[idDefensor].numeroTropas == 0) {
            printf("!!! TERRITÓRIO %s CONQUISTADO POR %s !!!\n",
                   mapa[idDefensor].nome, mapa[idAtacante].corExercito);
            
            strcpy(mapa[idDefensor].corExercito, mapa[idAtacante].corExercito);
            mapa[idDefensor].numeroTropas = 1;
            mapa[idAtacante].numeroTropas--;
        }
    } else {
        printf("\nVITÓRIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        mapa[idAtacante].numeroTropas--;
    }
}

/**
 * @brief Define uma das missões possíveis para o jogador de forma aleatória.
 * @param missao Ponteiro para a estrutura da missão que será preenchida.
 */
void atribuirMissaoAleatoria(Missao *missao) {
    int missaoId = rand() % 2; // Sorteia 0 ou 1

    if (missaoId == 0) {
        missao->tipo = CONQUISTAR_TERRITORIOS;
        missao->alvoQuantidade = 3;
        strcpy(missao->alvoCor, ""); // Não usado para esta missão
    } else {
        missao->tipo = DESTRUIR_EXERCITO;
        strcpy(missao->alvoCor, "Verde"); // Alvo da missão
        missao->alvoQuantidade = 0; // Não usado
    }
}

/**
 * @brief Imprime na tela a descrição da missão atual do jogador.
 * @param missao Ponteiro para a missão (const, pois não será alterada).
 */
void exibirMissao(const Missao *missao) {
    printf("\n--- SUA MISSÃO SECRETA ---\n");
    switch(missao->tipo) {
        case CONQUISTAR_TERRITORIOS:
            printf("Objetivo: Conquistar e manter %d territórios no mapa.\n", missao->alvoQuantidade);
            break;
        case DESTRUIR_EXERCITO:
            printf("Objetivo: Eliminar completamente o exército de cor %s do mapa.\n", missao->alvoCor);
            break;
    }
}

/**
 * @brief Verifica se a condição de vitória da missão atual foi atingida.
 * @param mapa Ponteiro para o mapa do mundo (const).
 * @param missao Ponteiro para a missão atual (const).
 * @return true se a missão foi cumprida, false caso contrário.
 */
bool verificarCumprimentoMissao(const Territorio *mapa, const Missao *missao) {
    switch (missao->tipo) {
        case CONQUISTAR_TERRITORIOS: {
            int territoriosDoJogador = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].corExercito, JOGADOR_COR) == 0) {
                    territoriosDoJogador++;
                }
            }
            return territoriosDoJogador >= missao->alvoQuantidade;
        }
        case DESTRUIR_EXERCITO: {
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                // Se encontrar qualquer território com a cor alvo, a missão ainda não foi cumprida
                if (strcmp(mapa[i].corExercito, missao->alvoCor) == 0) {
                    return false;
                }
            }
            // Se o loop terminar, significa que nenhum território da cor alvo foi encontrado
            return true;
        }
    }
    return false; // Caso padrão
}


/**
 * @brief Exibe o estado atual de todos os territórios em formato de tabela.
 * @param mapa Ponteiro para o vetor de territórios (const para não ser alterado).
 */
void exibirMapa(const Territorio *mapa) {
    printf("\n\n===================================================\n");
    printf("                   ESTADO ATUAL DO MAPA\n");
    printf("===================================================\n");
    printf("ID | %-20s | %-15s | %s\n", "Território", "Cor do Exército", "Tropas");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-2d | %-20s | %-15s | %d\n", i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].numeroTropas);
    }
    printf("===================================================\n");
}

/**
 * @brief Função utilitária para limpar o buffer de entrada (stdin).
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}