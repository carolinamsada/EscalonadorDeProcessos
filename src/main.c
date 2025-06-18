/**
 * @file main.c
 * @brief Ponto de entrada da aplicação.
 *
 * Inicializa a biblioteca GTK e abre a janela principal da interface gráfica.
 * Controla o ciclo de vida da aplicação GTK.
 *
 * @author Carolina Mafra Sada, Amanda Victória Almeida Silva
 */

#include <gtk/gtk.h>
#include "processos.h"

// Declarações externas da interface
void abrir_tela_processos();

// Lista global de processos e contador
Processo *lista_processos = NULL;
int n_processos = 0;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    abrir_tela_processos();

    gtk_main();

    // Ao fechar o app, libera a lista
    reinicializar_lista(&lista_processos, &n_processos);

    return 0;
}
