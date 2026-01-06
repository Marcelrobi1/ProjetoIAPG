#include "../include/storage.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funções para salvar e carregar estados de jogo e histórico.
// Suporta salvamento binário e textual do histórico, e saves nomeados.

// save_history_binary: Salva o estado do jogo no histórico binário (anexa).
// Passos:
// 1. Abre arquivo em modo append binário.
// 2. Escreve a estrutura GameState.
// 3. Fecha arquivo.
// 4. Retorna 1 se sucesso, 0 se erro.
int save_history_binary(const GameState *game, const char *filename) {
  FILE *f = fopen(filename, "ab"); // Anexar ao histórico
  if (!f)
    return 0;
  fwrite(game, sizeof(GameState), 1, f);
  fclose(f);
  return 1;
}

// load_history_binary: Carrega o último jogo do histórico binário.
// Passos:
// 1. Abre arquivo em modo leitura binária.
// 2. Posiciona no final menos o tamanho de GameState.
// 3. Lê a estrutura.
// 4. Fecha arquivo.
// 5. Retorna 1 se sucesso, 0 se erro.
int load_history_binary(GameState *game, const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return 0;
  fseek(f, -((long)sizeof(GameState)), SEEK_END);
  size_t read = fread(game, sizeof(GameState), 1, f);
  fclose(f);
  return read == 1;
}

// save_history_text: Salva resumo textual do jogo no histórico (anexa).
// Passos:
// 1. Abre arquivo em modo append texto.
// 2. Imprime ID, vencedor, pontuações.
// 3. Fecha arquivo.
// 4. Retorna 1 se sucesso, 0 se erro.
int save_history_text(const GameState *game, const char *filename) {
  FILE *f = fopen(filename, "a");
  if (!f)
    return 0;
  fprintf(f, "ID do Jogo: %d\n", game->game_id);
  fprintf(f, "Indice Vencedor: %d\n", game->winner_index);
  if (game->winner_index != -1)
    fprintf(f, "Vencedor: %s\n", game->players[game->winner_index].name);
  else
    fprintf(f, "Vencedor: Nenhum (Incompleto)\n");
  for (int i = 0; i < game->player_count; i++) {
    fprintf(f, "%s Pontos: %d\n", game->players[i].name,
            game->players[i].score);
  }
  fprintf(f, "--------------------------\n");
  fclose(f);
  return 1;
}

// --- Implementação de Saves Nomeados ---

int save_game_state(const GameState *game, const char *filename) {
  FILE *f = fopen(filename, "wb"); // Sobrescrever para arquivo específico
  if (!f)
    return 0;
  fwrite(game, sizeof(GameState), 1, f);
  fclose(f);
  return 1;
}

// load_game_state: Carrega o estado do jogo de arquivo binário nomeado.
// Passos:
// 1. Abre arquivo em modo leitura binária.
// 2. Lê a estrutura GameState.
// 3. Fecha arquivo.
// 4. Retorna 1 se sucesso, 0 se erro.
int load_game_state(GameState *game, const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return 0;
  size_t read = fread(game, sizeof(GameState), 1, f);
  fclose(f);
  return read == 1;
}

// list_save_files: Lista arquivos de save (.sav) no diretório atual.
// Passos:
// 1. Abre diretório atual.
// 2. Itera sobre arquivos, filtra .sav.
// 3. Aloca array de strings para nomes.
// 4. Retorna contagem e array.
int list_save_files(char ***filenames) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (!d)
    return 0;

  int count = 0;
  int capacity = 10;
  *filenames = malloc(capacity * sizeof(char *));
  if (*filenames == NULL) {
    closedir(d);
    return 0;
  }

  while ((dir = readdir(d)) != NULL) {
    if (strstr(dir->d_name, ".sav")) {
      // Apenas se terminar em .sav (verificação básica)
      char *dot = strrchr(dir->d_name, '.');
      if (dot && strcmp(dot, ".sav") == 0) {
        if (count >= capacity) {
          capacity *= 2;
          char **temp = realloc(*filenames, capacity * sizeof(char *));
          if (!temp) {
            // limpar
            free_save_files(*filenames, count);
            closedir(d);
            return 0;
          }
          *filenames = temp;
        }
        (*filenames)[count] = strdup(dir->d_name);
        count++;
      }
    }
  }
  closedir(d);
  return count;
}

// free_save_files: Libera memória alocada para lista de saves.
// Passos:
// 1. Para cada string, libera.
// 2. Libera o array.
void free_save_files(char **filenames, int count) {
  if (!filenames)
    return;
  for (int i = 0; i < count; i++) {
    free(filenames[i]);
  }
  free(filenames);
}
