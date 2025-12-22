#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void init_game(GameState *game, int num_players, int num_humans) {
  game->game_id = (int)time(NULL); // Simple ID
  game->board_count = 0;
  game->board_left_end = -1;
  game->board_right_end = -1;
  game->player_count = num_players;
  game->current_player_index = 0; // Starts with player 0, technically should be
                                  // highest double but can simplify first.
  game->winner_index = -1;
  game->boneyard_count = 28; // Full deck initially

  // Initialize deck
  init_deck(game->boneyard);
  shuffle_deck(game->boneyard, 28);

  // Initialize players
  for (int i = 0; i < num_players; i++) {
    sprintf(game->players[i].name, "Player %d", i + 1);
    game->players[i].hand_count = 0;
    game->players[i].score = 0;
    game->players[i].is_human = (i < num_humans);
    // The user requirement says: "escolher a quantidade de jogadores".
    // "não quero jogar contra uma inteligência generativa mas sim com outra
    // pessoa". So if 2 players selected, both human. If we want AI, we'd need a
    // separate menu. For now, let's assume all players in 'num_players' are
    // human if intended for hotseat. BUT, we might want to support "1 Player vs
    // AI". Let's add a parameter `int num_humans`. Simplified: All AI except
    // maybe player 1 for now, or all human locally? Requirement says "human vs
    // AI", so let's default p1 human, others AI.
  }

  distribute_pieces(game);
}

void distribute_pieces(GameState *game) {
  // Distribute 5 pieces to each player
  int card_idx = 0;
  for (int p = 0; p < game->player_count; p++) {
    for (int i = 0; i < 5; i++) {
      game->players[p].hand[i] = game->boneyard[card_idx++];
      game->players[p].hand_count++;
    }
  }

  // Shift boneyard
  // Quick hack: move remaining to front or just update pointer/count?
  // Since boneyard is array, best to slide down.
  int remaining = 28 - card_idx;
  for (int i = 0; i < remaining; i++) {
    game->boneyard[i] = game->boneyard[card_idx + i];
  }
  game->boneyard_count = remaining;
}

int can_play_piece(const GameState *game, DominoPiece piece,
                   int *side_to_match) {
  // If board is empty, any piece is valid
  if (game->board_count == 0) {
    *side_to_match = 0; // Any
    return 1;
  }

  // Check against left end
  // Piece has s1, s2. Board has L, R.
  // Logic:
  // If piece s1 == L -> match left (connect s1 to L, new L is s2)
  // If piece s2 == L -> match left (connect s2 to L, new L is s1)
  // If piece s1 == R -> match right (connect s1 to R, new R is s2)
  // If piece s2 == R -> match right (connect s2 to R, new R is s1)

  // Note: This function checks if *at least one* move is possible.
  // If multiple are possible, the UI needs to ask user.
  // Here we return bitmask or first valid?
  // Let's return 1 if valid. side_to_match can be output param indicating
  // possibilities: 1 = left, 2 = right, 3 = both.

  int valid = 0;
  if (piece.side1 == game->board_left_end ||
      piece.side2 == game->board_left_end)
    valid |= 1;
  if (piece.side1 == game->board_right_end ||
      piece.side2 == game->board_right_end)
    valid |= 2;

  if (valid)
    *side_to_match = valid;
  return valid > 0;
}

int play_piece(GameState *game, int player_idx, int piece_idx, int side) {
  // side: 1 = left, 2 = right
  Player *p = &game->players[player_idx];
  DominoPiece piece = p->hand[piece_idx];

  // Validate again logic could be here, but assuming valid called before.

  // Add to board
  // Simplified board storage: just keeping track of ends for logic.
  // Ideally we store the whole chain for display.
  // For now, let's just update ends.
  // To properly store chain, we need to know layout.
  // The GameState has `board` array. We should add to it.
  // But since we add to *ends*, a simple array append only works for one end.
  // Using a large array middle-out is better for array impl.
  // But `board` is size 28. If we just append to `board`, we lose order if we
  // don't track connectivity. RE-DESIGN: `board` in GameState should probably
  // be ordered list of pieces played. But inserting at Left means shifting.
  // Inserting at Right means append.

  if (game->board_count == 0) {
    game->board[0] = piece;
    game->board_count = 1;
    game->board_left_end = piece.side1;
    game->board_right_end = piece.side2;
    // User could rotate first piece? usually doesn't matter unless Spinner.
  } else {
    if (side == 1) { // Left
      // Must match board_left_end
      // Check orientation
      int connected_val = -1;
      int new_end = -1;

      if (piece.side2 == game->board_left_end) {
        // [s1|s2] -> matches s2 to Left. Visual: [s1|s2] [Left..]. Correct.
        connected_val = piece.side2;
        new_end = piece.side1;
      } else if (piece.side1 == game->board_left_end) {
        // [s1|s2] -> matches s1 to Left. Visual: [s1|s2] [Left..].
        // We want s2 to be the connecting side for Left play (Right side of
        // piece touches board). So we swap.
        int temp = piece.side1;
        piece.side1 = piece.side2;
        piece.side2 = temp;

        connected_val = piece.side2; // Now s2 is the old s1
        new_end = piece.side1;
      } else {
        return 0; // Invalid
      }

      // Insert at 0, shift others
      for (int i = game->board_count; i > 0; i--) {
        game->board[i] = game->board[i - 1];
      }
      game->board[0] = piece;
      game->board_count++;
      game->board_left_end = new_end;

    } else if (side == 2) { // Right
      // Must match board_right_end
      int new_end = -1;
      if (piece.side1 == game->board_right_end) {
        // [...Right] [s1|s2]. Correct.
        new_end = piece.side2;
      } else if (piece.side2 == game->board_right_end) {
        // [...Right] [s1|s2]. Matches s2.
        // We want s1 to connect. Swap.
        int temp = piece.side1;
        piece.side1 = piece.side2;
        piece.side2 = temp;

        new_end = piece.side2; // new exposed is old s1
      } else {
        return 0; // Invalid
      }

      game->board[game->board_count] = piece;
      game->board_count++;
      game->board_right_end = new_end;
    }
  }

  // Remove from hand
  for (int i = piece_idx; i < p->hand_count - 1; i++) {
    p->hand[i] = p->hand[i + 1];
  }
  p->hand_count--;

  // Score
  int points = calculate_score(game);
  if (points > 0) {
    p->score += points;
  }

  // Check win
  if (p->hand_count == 0) {
    game->winner_index = player_idx;
    // Add opponents' pips to score? (Muggins rule sometimes does this)
    // Definição do problema says: "Ganha o jogador que primeiro chegar aos 61
    // pontos". Also "A peça que um jogador joga... senão tem que ir pescar".
    // Usually, ending round awards points. Requirement doesn't explicitly
    // detail end-round scoring besides "Ganha... 61 pontos". But Five-Up
    // usually awards points during play.
  }

  return 1;
}

int calculate_score(const GameState *game) {
  if (game->board_count == 0)
    return 0;

  // Sum of open ends.
  // Basic interaction: Left End + Right End.
  // If double, is it crosswise?
  // Standard Muggins: First double is spinner (4 ends).
  // Config: 0=none (2 ends), 1=first (4 ends), 2=all (many).
  // Let's implement Config 0 first (2 ends) as default.

  // IMPORTANT: When a double is played spinner-style, it counts as sum of its
  // pips? Usually double 5 counts as 10. But if it's inline (non-spinner), does
  // it count as 10 or just one end? In Muggins, a double placed inline counts
  // as total pips (5-5 = 10). A single piece 3-5 at end counts as 5 (the open
  // end).

  // Assumption: We track `board_left_end` and `board_right_end` as the VALUES
  // exposed. If `board_left_end` is from a double 5-5, does our `play_piece`
  // logic store 5 or 10? `board_left_end` stores 5. So calculation needs to
  // check if that piece is a double.

  // To do this accurately, we need to know if ends are doubles.
  // `board[0]` is left end (if we shifted correctly), `board[count-1]` is right
  // end.

  int left_val = game->board_left_end;
  DominoPiece left_piece = game->board[0];
  if (left_piece.is_double)
    left_val += left_val; // Double counts full value?
  // Wait. Standard rules: "The open ends of the layout are added up".
  // A double 6 (6-6) transversely usually counts as 12.
  // A non-double 6-something counts as 6 (the exposed end).

  // BUT, we need to be careful. In play_piece, we set `board_left_end` to the
  // exposed number. If I have 5-5 on the left end, `board_left_end` is 5. So
  // the score contribution is 5 + 5 = 10.

  // Checking piece at index 0 for Left, index count-1 for Right.
  // Correction: `board` array order might be confusing if we just shifted.
  // Yes, index 0 is left-most, index count-1 is right-most based on my logic in
  // play_piece.

  int score = 0;

  // Left
  if (game->board[0].is_double)
    score += (game->board_left_end * 2);
  else
    score += game->board_left_end;

  // Right (if only 1 piece, left=right physically, but logically handled?)
  if (game->board_count == 1) {
    // Only one piece. It's both ends.
    // e.g. 5-5. Left exposed 5, Right exposed 5. sum = 10?
    // Or just piece total.
    // If 3-5. Left 3, Right 5. sum = 8.
    score = game->board[0].side1 + game->board[0].side2;
  } else {
    if (game->board[game->board_count - 1].is_double)
      score += (game->board_right_end * 2);
    else
      score += game->board_right_end;
  }

  if (score % 5 == 0)
    return score;
  return 0;
}

int draw_piece(GameState *game, int player_idx) {
  if (game->boneyard_count <= 0)
    return 0; // Empty

  Player *p = &game->players[player_idx];
  if (p->hand_count >= MAX_HAND_SIZE)
    return 0; // Hand full?

  // Take from front
  p->hand[p->hand_count] = game->boneyard[0];
  p->hand_count++;

  // Shift boneyard
  for (int i = 0; i < game->boneyard_count - 1; i++) {
    game->boneyard[i] = game->boneyard[i + 1];
  }
  game->boneyard_count--;

  return 1;
}
