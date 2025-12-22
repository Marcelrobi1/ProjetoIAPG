#include "storage.h"
#include <stdio.h>

// Requirement: Escrita em modo binário do histórico dos jogos
// (historico_jogos.dat). Requirement: Leitura em modo binário do histórico dos
// jogos. Requirement: Escrita em modo de texto do histórico dos jogos
// (historico_jogos.txt).

int save_history_binary(const GameState *game, const char *filename) {
  FILE *f =
      fopen(filename,
            "ab"); // Append to history? Or overwrite? "histórico" implies list.
  // We will append this game state.
  if (!f)
    return 0;

  fwrite(game, sizeof(GameState), 1, f);
  fclose(f);
  return 1;
}

int load_history_binary(GameState *game, const char *filename) {
  // Load last game? Or read all?
  // Requirement says "Leitura ... do histórico".
  // For simplicity, let's just allow reading the last one or all.
  // Implementation: print count or read last.
  FILE *f = fopen(filename, "rb");
  if (!f)
    return 0;

  // Seek to end - size
  fseek(f, -((long)sizeof(GameState)), SEEK_END);
  size_t read = fread(game, sizeof(GameState), 1, f);

  fclose(f);
  return read == 1;
}

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
