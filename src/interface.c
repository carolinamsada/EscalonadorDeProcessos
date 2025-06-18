/**
 * @file interface.c
 * @brief Interface gráfica do simulador usando GTK.
 *
 * Implementa janelas e interações para o usuário adicionar, remover,
 * reinicializar e simular os processos com diferentes algoritmos.
 * Conecta os botões e elementos visuais com as funções do backend.
 *
 * A interface possui:
 *  - Janela para visualização e controle da lista de processos
 *  - Diálogo para adicionar processos
 *  - Seleção de algoritmo
 *  - Exibição do resultado da simulação
 *
 * @author Carolina Mafra Sada, Amanda Victória Almeida Silva
 */
#include <gtk/gtk.h>                  // Biblioteca GTK para construção da interface gráfica
#include <stdlib.h>                  // Biblioteca padrão para alocação de memória e conversões
#include "processos.h"               // Header com estrutura e funções de manipulação de processos
#include "simulador.h"               // Header com os algoritmos de escalonamento

// Variáveis externas (definidas em outro arquivo)
extern Processo *lista_processos;    // Lista dinâmica de processos 
extern int n_processos;              // Número total de processos atuais

// Protótipos
void abrir_tela_processos();
void abrir_dialogo_adicionar_processo(GtkWidget *parent);
void atualizar_lista(GtkWidget *listbox);
void on_btn_remover_clicked(GtkButton *button, gpointer user_data);
void on_btn_reinicializar_clicked(GtkButton *button, gpointer user_data);
void abrir_tela_selecao_algoritmo(GtkWidget *widget);
void executar_escalonamento(int algoritmo_index, GtkWidget *widget);
void abrir_resultado_simulacao(const char *resultado);


// Abre um diálogo GTK para o usuário inserir os dados de processos
void abrir_dialogo_adicionar_processo(GtkWidget *parent) {
    // Criação de Widgets e grade
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *entry_chegada, *entry_execucao, *entry_prioridade;
    GtkWidget *label_chegada, *label_execucao, *label_prioridade;

    // Criação da janela de diálogo modal
    dialog = gtk_dialog_new_with_buttons("Adicionar Processo",
                                     GTK_WINDOW(gtk_widget_get_toplevel(parent)),
                                     GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                     "_Cancelar", GTK_RESPONSE_CANCEL,
                                     "_Adicionar", GTK_RESPONSE_OK,
                                     NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Configuração da área de conteúdo e grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Criação dos labels e entradas
    label_chegada = gtk_label_new("Tempo de Chegada:");
    entry_chegada = gtk_entry_new();

    label_execucao = gtk_label_new("Tempo de Execução:");
    entry_execucao = gtk_entry_new();

    label_prioridade = gtk_label_new("Prioridade:");
    entry_prioridade = gtk_entry_new();

    // Adicação a grid
    gtk_grid_attach(GTK_GRID(grid), label_chegada, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_chegada, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), label_execucao, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_execucao, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), label_prioridade, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prioridade, 1, 2, 1, 1);

    // Exibe o diálogo
    gtk_widget_show_all(dialog);

    // Ao clicar em "Adicionar" 
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        // Captura os textos
        const gchar *chegada_text = gtk_entry_get_text(GTK_ENTRY(entry_chegada));
        const gchar *execucao_text = gtk_entry_get_text(GTK_ENTRY(entry_execucao));
        const gchar *prioridade_text = gtk_entry_get_text(GTK_ENTRY(entry_prioridade));

        // Verifica se os campos foram preenchidos
        if (g_strcmp0(chegada_text, "") != 0 && g_strcmp0(execucao_text, "") != 0 && g_strcmp0(prioridade_text, "") != 0) {
            // Se sim, cria novo processo, insere e atualiza
            Processo novo;
            novo.pid = n_processos + 1;
            novo.tempo_chegada = atoi(chegada_text);
            novo.tempo_execucao = atoi(execucao_text);
            novo.prioridade = atoi(prioridade_text);

            lista_processos = inserir_processo(lista_processos, &n_processos, novo);

            g_print("Processo %d adicionado.\n", novo.pid);
            abrir_tela_processos(); // Atualiza a lista
        } else {    // Caso contrário
            g_print("Preencha todos os campos!\n");
        }
    }
    // Fecha diálogo
    gtk_widget_destroy(dialog);
}

// Atualiza os itens da lista de processos
void atualizar_lista(GtkWidget *listbox) {
    // Limpa o conteúdo atual
    gtk_list_box_prepend(GTK_LIST_BOX(listbox), gtk_label_new("")); 
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(listbox));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Cria um item por processo
    for (int i = 0; i < n_processos; i++) {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "PID: %d | Chegada: %d | Execução: %d | Prioridade: %d",
                 lista_processos[i].pid,
                 lista_processos[i].tempo_chegada,
                 lista_processos[i].tempo_execucao,
                 lista_processos[i].prioridade);

        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *label = gtk_label_new(buffer);
        GtkWidget *btn_remover = gtk_button_new_with_label("Remover");

        // Associar o PID ao botão para remoção
        g_object_set_data(G_OBJECT(btn_remover), "pid", GINT_TO_POINTER(lista_processos[i].pid));

        g_signal_connect(btn_remover, "clicked", G_CALLBACK(on_btn_remover_clicked), listbox);

        // Adiciona à interface
        gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), btn_remover, FALSE, FALSE, 0);

        gtk_container_add(GTK_CONTAINER(row), hbox);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
    }
    gtk_widget_show_all(listbox);
}

// Callback para o botão remover
void on_btn_remover_clicked(GtkButton *button, gpointer user_data) {
    int pid = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "pid"));
    lista_processos = remover_processo(lista_processos, &n_processos, pid);

    GtkWidget *listbox = GTK_WIDGET(user_data);
    atualizar_lista(listbox);

    g_print("Processo %d removido.\n", pid);
}

// Callback para reinicializar lista
void on_btn_reinicializar_clicked(GtkButton *button, gpointer user_data) {
    reinicializar_lista(&lista_processos, &n_processos);

    GtkWidget *listbox = GTK_WIDGET(user_data);
    atualizar_lista(listbox);

    g_print("Lista reinicializada.\n");
}

// Função principal para abrir a tela de processos
void abrir_tela_processos() {
    // Evitar abrir janelas duplicadas
    static GtkWidget *window = NULL;
    static GtkWidget *listbox = NULL;

    if (window != NULL) {
        gtk_window_present(GTK_WINDOW(window));
        atualizar_lista(listbox);
        return;
    }

    // Cria a janela e os componentes
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Lista de Processos");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // ListBox para os processos e botões
    listbox = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), listbox, TRUE, TRUE, 0);

    // Botão adicionar
    GtkWidget *btn_add = gtk_button_new_with_label("Adicionar Processo");
    gtk_box_pack_start(GTK_BOX(vbox), btn_add, FALSE, FALSE, 0);
    g_signal_connect(btn_add, "clicked", G_CALLBACK(abrir_dialogo_adicionar_processo), window);

    // Botão reinicializar
    GtkWidget *btn_reinicializar = gtk_button_new_with_label("Reinicializar Lista");
    gtk_box_pack_start(GTK_BOX(vbox), btn_reinicializar, FALSE, FALSE, 0);
    g_signal_connect(btn_reinicializar, "clicked", G_CALLBACK(on_btn_reinicializar_clicked), listbox);
    
    // Botão executar escalonamento
    GtkWidget *btn_executar = gtk_button_new_with_label("Selecionar Algoritmo");
    gtk_box_pack_start(GTK_BOX(vbox), btn_executar, FALSE, FALSE, 0);
    g_signal_connect(btn_executar, "clicked", G_CALLBACK(abrir_tela_selecao_algoritmo), window);

    // Mostra tudo e define callback de encerramento
    gtk_widget_show_all(window);
    atualizar_lista(listbox);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

}

// Tela de seleção de algoritmo 
void abrir_tela_selecao_algoritmo(GtkWidget *parent) {
    // Cria o diálogo como combo_box
    GtkWidget *dialog;
    dialog = gtk_dialog_new_with_buttons("Selecione o Algoritmo",
                                         GTK_WINDOW(gtk_widget_get_toplevel(parent)),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "_Cancelar", GTK_RESPONSE_CANCEL,
                                         "_Executar", GTK_RESPONSE_OK,
                                         NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *combo_algoritmo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algoritmo), "FIFO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algoritmo), "SJF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algoritmo), "Round-Robin");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algoritmo), "Prioridade (Preemptivo)");

    gtk_container_add(GTK_CONTAINER(content_area), combo_algoritmo);

    // Executa a seleção
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        int selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_algoritmo));
        if (selected_index >= 0) {
            // Aqui chamamos a função para rodar o algoritmo
            executar_escalonamento(selected_index, parent);
        }
    }

    gtk_widget_destroy(dialog);
}

// Executar algoritmo
void executar_escalonamento(int algoritmo_index, GtkWidget *widget) {
    char output_buffer[4096]; // Ajuste se necessário

    switch (algoritmo_index) {
        case 0: // FIFO
            simular_FIFO(lista_processos, n_processos, output_buffer);
            break;
        case 1: // SJF
            simular_sjf(lista_processos, n_processos, output_buffer);
            break;
        case 2: // Round-Robin
            simular_round_robin(lista_processos, n_processos, output_buffer);
            break;
        case 3: // Prioridade Preemptivo
            simular_prioridade(lista_processos, n_processos, output_buffer);
            break;
        default:
            strcpy(output_buffer, "Algoritmo não implementado.");
    }

    abrir_resultado_simulacao(output_buffer);
}

// Mostrar Resultado
void abrir_resultado_simulacao(const char *resultado) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Resultado da Simulação");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)), resultado, -1);
    gtk_box_pack_start(GTK_BOX(vbox), textview, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}
