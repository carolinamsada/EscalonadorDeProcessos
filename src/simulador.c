/**
 * @file simulador.c
 * @brief Implementação dos algoritmos de escalonamento de processos.
 *
 * Contém a lógica dos algoritmos FIFO, SJF, Round-Robin e Prioridade.
 * Cada função simula o comportamento de escalonamento com base na lista
 * de processos atual e retorna os resultados em forma de texto.
 *
 * @author Carolina Mafra Sada, Amanda Victória Almeida SIlva
 */

#include <stdio.h>      // Biblioteca padrão para entrada/saída 
#include <stdlib.h>     // Biblioteca para alocação dinâmica 
#include <string.h>     // Biblioteca para manipulação de strings 
#include "simulador.h"  // Header com as declarações das funções deste arquivo
#include "processos.h"  // Header com a definição da struct Processo

// Simula o escalonamento FIFO (primeiro a chegar, primeiro a ser executado, FIFO)
void simular_FIFO(Processo *lista_processos, int n, char *output_buffer) {
    int tempo_atual = 0;                // Marca o tempo atual do sistema
    char temp[256];                     // Buffer temporário para formatar a saída
    
    // Cabeçalho da simulação
    strcpy(output_buffer, "Simulação FIFO:\n");
    strcat(output_buffer, "PID\tInício\tFim\tTurnaround\tEspera\n");

    // Ordena os processos por tempo de chegada
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (lista_processos[j].tempo_chegada > lista_processos[j + 1].tempo_chegada) {
                Processo temp = lista_processos[j];
                lista_processos[j] = lista_processos[j + 1];
                lista_processos[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        // Se o processo chegou depois do tempo atual, o sistema espera
        if (lista_processos[i].tempo_chegada > tempo_atual) {
            tempo_atual = lista_processos[i].tempo_chegada;
        }

        int inicio = tempo_atual;                                // Tempo em que o processo começa
        int fim = inicio + lista_processos[i].tempo_execucao;    // Tempo em que o processo termina
        int turnaround = fim - lista_processos[i].tempo_chegada; // Tempo total que o processo levou (chegada até término)
        int espera = inicio - lista_processos[i].tempo_chegada;  // Quanto tempo o processo esperou na fila

        // Formata e armazena a linha do processo no buffer de saída
        snprintf(temp, sizeof(temp), "%-6d\t%-7d\t%-7d\t%-11d\t%-6d\n",
                 lista_processos[i].pid,
                 inicio,
                 fim,
                 turnaround,
                 espera);

        strcat(output_buffer, temp);      // Adiciona a linha ao resultado final

        tempo_atual = fim;                // Atualiza o tempo atual para o fim do processo
    }
}

// Simula o escalonamento FIFO (primeiro a chegar, primeiro a ser executado)
void simular_sjf(Processo *lista_processos, int n, char *output_buffer) {
    int tempo_atual = 0;                        // Tempo atual da simulação
    int concluídos = 0;                         // Contador de processos finalizados
    int *finalizado = calloc(n, sizeof(int));  // Vetor que marca quais processos já foram executados
    char temp[256];                            // Buffer temporário para formatar a saída

    // Cabeçalho da simulação
    strcpy(output_buffer, "Simulação SJF:\n");
    strcat(output_buffer, "PID\tInício\tFim\tTurnaround\tEspera\n");

    while (concluídos < n) {
        int menor_tempo = __INT_MAX__;  // Tempo de execução mais curto
        int indice = -1;                // Índice do processo escolhido

        for (int i = 0; i < n; i++) {
            // Seleciona o menor tempo entre os processos que já chegaram e não foram finalizados
            if (!finalizado[i] && lista_processos[i].tempo_chegada <= tempo_atual) {
                if (lista_processos[i].tempo_execucao < menor_tempo) {
                    menor_tempo = lista_processos[i].tempo_execucao; // Atualiza o menor tempo encontrado
                    indice = i;                                      // Salva o índice do processo com menor tempo
                }
            }
        }

        // Se nenhum processo está pronto, incrementa o tempo
        if (indice == -1) {
            tempo_atual++;  // Avança o tempo da simulação
            continue;       // Volta ao início do while
        }

        int inicio = tempo_atual;                                       // Tempo que o processo começa a ser executado
        int fim = inicio + lista_processos[indice].tempo_execucao;      // Tempo que o processo termina
        int turnaround = fim - lista_processos[indice].tempo_chegada;   // Tempo total entre chegada e fim
        int espera = inicio - lista_processos[indice].tempo_chegada;    // Tempo que o processo ficou esperando na fila

        snprintf(temp, sizeof(temp), "%-6d\t%-7d\t%-7d\t%-11d\t%-6d\n",
                 lista_processos[indice].pid,
                 inicio,
                 fim,
                 turnaround,
                 espera);

        strcat(output_buffer, temp);  // Adiciona essa linha formatada ao buffer de saída

        tempo_atual = fim;            // Atualiza o tempo da simulação para o fim do processo executado
        finalizado[indice] = 1;       // Marca o processo como concluído
        concluídos++;                 // Atualiza o contador de processos finalizados
    }

    free(finalizado);  // Libera memória
}

// Simula o escalonamento round-robin com quantum fixo de 2
void simular_round_robin(Processo *lista_processos, int n, char *output_buffer) {
    int quantum = 2;  // Tempo fixo de fatia para cada processo
    int tempo_atual = 0;  // Relógio da simulação, em unidades de tempo
    // Array para armazenar o tempo restante de execução de cada processo
    int *tempo_restante = malloc(n * sizeof(int)); 
    char temp[256];  // Buffer temporário para montar strings de saída

    // Inicializa o tempo restante com o tempo total de execução de cada processo
    for (int i = 0; i < n; i++) {
        tempo_restante[i] = lista_processos[i].tempo_execucao;
    }

    int processos_restantes = n;  // Quantidade de processos ainda não concluídos

    // Inicializa a saída com cabeçalho da simulação
    strcpy(output_buffer, "Simulação Round-Robin (Q=2):\n");
    strcat(output_buffer, "Execução passo a passo:\n");

    // Arrays para controlar o tempo de início de execução e processos finalizados
    int *inicio_execucao = calloc(n, sizeof(int));  // Inicializa com zero
    int *terminado = calloc(n, sizeof(int));        // Inicializa com zero

    // Buffer para o resumo final das métricas (turnaround, espera)
    char resumo_final[2048] = "";
    strcat(resumo_final, "Resumo Final:\n");
    strcat(resumo_final, "PID\tInício\tFim\tTurnaround\tEspera\n");

    // Loop principal enquanto houver processos a executar
    while (processos_restantes > 0) {
        int executou_ao_menos_um = 0;  // Flag para verificar se algum processo rodou nesta volta

        for (int i = 0; i < n; i++) {
            // Verifica se o processo está pronto para rodar (tempo restante > 0 e já chegou)
            if (tempo_restante[i] > 0 && lista_processos[i].tempo_chegada <= tempo_atual) {
                executou_ao_menos_um = 1;

                // Registra o tempo de início da primeira execução do processo
                // Problema: se começar no tempo 0, condição pode falhar (ver resposta técnica)
                if (!inicio_execucao[i]) {
                    inicio_execucao[i] = (tempo_atual < lista_processos[i].tempo_chegada) ?
                                          lista_processos[i].tempo_chegada : tempo_atual;
                }

                // Define quanto tempo o processo vai rodar: quantum ou tempo restante se menor
                int tempo_exec = (tempo_restante[i] < quantum) ? tempo_restante[i] : quantum;

                // Ajusta o tempo inicial para o tempo de chegada caso seja maior que tempo atual
                int tempo_inicio = (tempo_atual < lista_processos[i].tempo_chegada) ?
                                    lista_processos[i].tempo_chegada : tempo_atual;

                // Atualiza o tempo atual somando o tempo de execução desta fatia
                tempo_atual = tempo_inicio + tempo_exec;

                // Decrementa o tempo restante do processo
                tempo_restante[i] -= tempo_exec;

                // Registra no log a execução deste intervalo
                snprintf(temp, sizeof(temp),
                         "Tempo %d-%d: PID %d (restante: %d)\n",
                         tempo_inicio, tempo_atual,
                         lista_processos[i].pid,
                         tempo_restante[i]);
                strcat(output_buffer, temp);

                // Caso o processo tenha terminado nesta execução
                if (tempo_restante[i] == 0 && !terminado[i]) {
                    int fim = tempo_atual;  // Tempo final do processo
                    int turnaround = fim - lista_processos[i].tempo_chegada;  // Tempo total no sistema
                    int espera = turnaround - lista_processos[i].tempo_execucao;  // Tempo de espera na fila

                    // Registra as métricas finais no resumo
                    snprintf(temp, sizeof(temp), "%-6d\t%-7d\t%-7d\t%-11d\t%-6d\n",
                             lista_processos[i].pid,
                             inicio_execucao[i],
                             fim,
                             turnaround,
                             espera);
                    strcat(resumo_final, temp);

                    terminado[i] = 1;          // Marca processo como terminado
                    processos_restantes--;      // Decrementa contador de processos restantes
                }
            }
        }

        // Se nenhum processo foi executado neste ciclo, incrementa o tempo para simular espera
        if (!executou_ao_menos_um) {
            tempo_atual++;
        }
    }

    // Adiciona o resumo final à saída principal
    strcat(output_buffer, "\n");
    strcat(output_buffer, resumo_final);

    // Libera memória alocada dinamicamente
    free(tempo_restante);
    free(inicio_execucao);
    free(terminado);
}

void simular_prioridade(Processo *lista_processos, int n, char *output_buffer) {
    int tempo_atual = 0;  // Relógio da simulação em unidades de tempo
    int *tempo_restante = malloc(n * sizeof(int));  // Tempo restante para cada processo
    int *finalizado = calloc(n, sizeof(int));       // Flag para processos concluídos
    int processos_restantes = n;                     // Contador de processos não finalizados
    char temp[256];                                  // Buffer temporário para saída

    // Inicializa tempo restante com tempo total de execução dos processos
    for (int i = 0; i < n; i++) {
        tempo_restante[i] = lista_processos[i].tempo_execucao;
    }

    // Cabeçalho da saída
    strcpy(output_buffer, "Simulação Prioridade Preemptiva:\n");
    strcat(output_buffer, "Execução passo a passo:\n");

    // Buffer para resumo final de métricas
    char resumo_final[2048] = "";
    strcat(resumo_final, "\nResumo Final:\n");
    strcat(resumo_final, "PID\tInício\tFim\tTurnaround\tEspera\n");

    int *inicio_execucao = calloc(n, sizeof(int));  // Registra o tempo da primeira execução de cada processo

    // Loop principal até todos os processos terminarem
    while (processos_restantes > 0) {
        int menor_prioridade = __INT_MAX__;  // Inicializa com maior inteiro possível
        int indice = -1;                     // Índice do processo escolhido para executar

        // Busca processo pronto com menor prioridade
        for (int i = 0; i < n; i++) {
            if (!finalizado[i] && lista_processos[i].tempo_chegada <= tempo_atual) {
                if (lista_processos[i].prioridade < menor_prioridade) {
                    menor_prioridade = lista_processos[i].prioridade;
                    indice = i;
                }
            }
        }

        // Nenhum processo disponível, avança tempo
        if (indice == -1) {
            tempo_atual++;
            continue;
        }

        // Marca início da execução se for a primeira vez
        if (!inicio_execucao[indice]) {
            inicio_execucao[indice] = tempo_atual;
        }

        // Log da execução da unidade de tempo do processo escolhido
        snprintf(temp, sizeof(temp), "Tempo %d: PID %d (restante: %d)\n",
                 tempo_atual, lista_processos[indice].pid, tempo_restante[indice] - 1);
        strcat(output_buffer, temp);

        // Executa um passo da unidade de tempo
        tempo_restante[indice]--;
        tempo_atual++;

        // Processo terminou?
        if (tempo_restante[indice] == 0) {
            int fim = tempo_atual;
            int turnaround = fim - lista_processos[indice].tempo_chegada;
            int espera = turnaround - lista_processos[indice].tempo_execucao;

            // Registra métricas finais no resumo
            snprintf(temp, sizeof(temp), "%-6d\t%-7d\t%-7d\t%-11d\t%-6d\n",
                     lista_processos[indice].pid,
                     inicio_execucao[indice],
                     fim,
                     turnaround,
                     espera);

            strcat(resumo_final, temp);
            finalizado[indice] = 1;
            processos_restantes--;
        }
    }

    // Anexa resumo final ao output principal
    strcat(output_buffer, resumo_final);
 
    // Libera memória alocada dinamicamente
    free(tempo_restante);
    free(finalizado);
    free(inicio_execucao);
}

