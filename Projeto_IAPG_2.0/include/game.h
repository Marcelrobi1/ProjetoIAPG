#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "domino.h"
#include "player.h"

#define MAX_PLAYERS 4
#define DECK_SIZE 28

typedef struct {
  int game_id;
  DominoPiece boneyard[DECK_SIZE];
  int boneyard_count;

  DominoPiece board[DECK_SIZE];
  int board_left_end;  // O valor exposto à esquerda
  int board_right_end; // O valor exposto à direita
  int board_count;

  Player players[MAX_PLAYERS];
  int player_count;
  int current_player_index;
  GameConfig config;

  int winner_index; // -1 se ainda não há vencedor
} GameState;

// Function prototypes
void init_game(GameState *game, int num_players, int num_humans);
int can_play_piece(const GameState *game, DominoPiece piece,
                   int *side_to_match);
int play_piece(GameState *game, int player_idx, int piece_idx, int side);
int calculate_score(const GameState *game);
void distribute_pieces(GameState *game);
int draw_piece(GameState *game, int player_idx);

#endif // GAME_H
