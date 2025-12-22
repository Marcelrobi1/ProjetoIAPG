#ifndef STORAGE_H
#define STORAGE_H

#include "game.h"

int save_history_binary(const GameState *game, const char *filename);
int load_history_binary(GameState *game, const char *filename);
int save_history_text(const GameState *game, const char *filename);

#endif // STORAGE_H
