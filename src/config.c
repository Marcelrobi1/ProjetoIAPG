#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para carregar configurações do jogo de arquivo.

// load_config: Carrega configurações de "config_domino.txt".
// Passos:
// 1. Abre arquivo para leitura.
// 2. Se não existir, usa valores padrão.
// 3. Parseia linhas chave=valor.
// 4. Define spinner_mode.
// 5. Fecha arquivo.
int load_config(const char *filename, GameConfig *config) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    // Valores padrão se arquivo faltar
    config->spinner_mode = 0;
    return 0; // Arquivo não encontrado
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    // Análise simples chave=valor
    char *key = strtok(line, "=");
    char *value = strtok(NULL, "\n"); // Remover nova linha

    if (key && value) {
      if (strcmp(key, "spinner_mode") == 0) {
        config->spinner_mode = atoi(value);
      }
    }
  }

  fclose(file);
  return 1; // Sucesso
}
