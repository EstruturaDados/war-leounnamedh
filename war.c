// ============================================================================
//      PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (NÍVEL AVENTUREIRO)
// ============================================================================
//
// OBJETIVO:
// - Usar alocação dinâmica para o mapa.
// - Implementar uma fase de ataque interativa.
// - Simular batalhas com dados aleatórios.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias
#include <stdio.h>   // Para funções de entrada/saída
#include <string.h>  // Para manipulação de strings
#include <stdlib.h>  // Para alocação de memória (calloc, free) e números aleatórios (rand, srand)
#include <time.h>    // Para inicializar a semente de números aleatórios (time)

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define MAX_NOME 51
#define MAX_COR 21

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numeroTropas;
} Territorio;

// --- Protótipos das Funções ---
Territorio* alocarMapa(); // MODIFICADO: Agora retorna um ponteiro
void cadastrarTerritorios(Territorio *mapa); // MODIFICADO: Aceita um ponteiro
void exibirMapa(const Territorio *mapa); // MODIFICADO: Aceita um ponteiro
void faseDeAtaque(Territorio *mapa); // NOVO
void simularBatalha(Territorio *mapa, int idAtacante, int idDefensor); // NOVO
void limparBufferEntrada();

// --- Função Principal (main) ---
int main() {
    // Inicializa a semente para geração de números aleatórios uma única vez.
    srand(time(NULL));

    // 1. Alocação Dinâmica de Memória
    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: Falha ao alocar memória para o mapa!\n");
        return 1; // Termina o programa com código de erro
    }

    printf("Bem-vindo ao Desafio WAR Estruturado - Nível Aventureiro!\n");
    cadastrarTerritorios(mapa);

    // 2. Laço Principal do Jogo (Game Loop)
    int opcao;
    do {
        exibirMapa(mapa);
        printf("\nEscolha sua ação:\n");
        printf("1. Atacar\n");
        printf("0. Sair do Jogo\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 0:
                printf("\nEncerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    // 3. Liberação da Memória Alocada
    // Essencial para evitar vazamento de memória (memory leak).
    free(mapa);

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Aloca memória para o mapa do mundo usando calloc.
 * @return Um ponteiro para a memória alocada ou NULL em caso de falha.
 */
Territorio* alocarMapa() {
    // calloc aloca e inicializa todos os bytes com zero.
    return (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

/**
 * @brief Solicita ao usuário que insira os dados de cada território.
 * @param mapa Ponteiro para o vetor de territórios que será preenchido.
 */
void cadastrarTerritorios(Territorio *mapa) {
    printf("Vamos cadastrar os %d territórios iniciais.\n", NUM_TERRITORIOS);
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Cadastrando Território %d ---\n", i + 1);
        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].corExercito, MAX_COR, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';

        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].numeroTropas);
        limparBufferEntrada();
    }
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

    // Validações básicas
    if (idAtacante < 1 || idAtacante > NUM_TERRITORIOS || idDefensor < 1 || idDefensor > NUM_TERRITORIOS) {
        printf("\nErro: Número de território inválido!\n");
        return;
    }
    if (idAtacante == idDefensor) {
        printf("\nErro: Você não pode atacar o mesmo território!\n");
        return;
    }

    // Converte para índice do vetor (0 a 4)
    int idxAtacante = idAtacante - 1;
    int idxDefensor = idDefensor - 1;
    
    // Validações de Lógica de Jogo
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
    printf("\n>>> Batalha: %s (%d tropas) ataca %s (%d tropas) <<<\n",
           mapa[idAtacante].nome, mapa[idAtacante].numeroTropas,
           mapa[idDefensor].nome, mapa[idDefensor].numeroTropas);
           
    // Simula a rolagem de um dado de 6 faces
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("--- Rolagem de Dados ---\n");
    printf("Ataque rolou: %d\n", dadoAtaque);
    printf("Defesa rolou: %d\n", dadoDefesa);
    
    // Lógica da batalha: empate favorece o ataque
    if (dadoAtaque >= dadoDefesa) {
        printf("\nVITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        mapa[idDefensor].numeroTropas--;

        // Verifica se o território foi conquistado
        if (mapa[idDefensor].numeroTropas == 0) {
            printf("!!! TERRITÓRIO %s CONQUISTADO POR %s !!!\n",
                   mapa[idDefensor].nome, mapa[idAtacante].corExercito);
            
            // Transfere a propriedade
            strcpy(mapa[idDefensor].corExercito, mapa[idAtacante].corExercito);
            
            // Move uma tropa para o novo território
            mapa[idDefensor].numeroTropas = 1;
            mapa[idAtacante].numeroTropas--;
        }
    } else {
        printf("\nVITÓRIA DA DEFESA! O ataque falhou.\n");
    }
}


/**
 * @brief Exibe o estado atual de todos os territórios em formato de tabela.
 * @param mapa Ponteiro para o vetor de territórios (const para não ser alterado).
 */
void exibirMapa(const Territorio *mapa) {
    printf("\n\n===================================================\n");
    printf("              ESTADO ATUAL DO MAPA\n");
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