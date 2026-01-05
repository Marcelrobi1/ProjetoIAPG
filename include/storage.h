#ifndef STORAGE_H
#define STORAGE_H

#include "game.h"

int save_history_binary(const GameState *game, const char *filename);
int load_history_binary(GameState *game, const char *filename);
int save_history_text(const GameState *game, const char *filename);

// New functions for Named Saves
int save_game_state(const GameState *game, const char *filename);
int load_game_state(GameState *game, const char *filename);
int list_save_files(char ***filenames);
void free_save_files(char **filenames, int count);

#endif // STORAGE_H
