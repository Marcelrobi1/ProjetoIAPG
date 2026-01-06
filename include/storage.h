#ifndef STORAGE_H
#define STORAGE_H

#include "game.h"

// Protótipos de funções para armazenamento.
// save_history_binary: Salva histórico binário.
// load_history_binary: Carrega último do histórico binário.
// save_history_text: Salva histórico textual.
// save_game_state: Salva jogo nomeado.
// load_game_state: Carrega jogo nomeado.
// list_save_files: Lista saves.
// free_save_files: Libera lista de saves.

int save_history_binary(const GameState *game, const char *filename);
int load_history_binary(GameState *game, const char *filename);
int save_history_text(const GameState *game, const char *filename);
int save_game_state(const GameState *game, const char *filename);
int load_game_state(GameState *game, const char *filename);
int list_save_files(char ***filenames);
void free_save_files(char **filenames, int count);

#endif // STORAGE_H
