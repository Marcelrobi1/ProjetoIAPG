#include "../include/config.h"
#include "../include/game.h"
#include "../include/storage.h"
#include "../include/ui.h"
#include "../lib/pdcurses/PDCurses-3.9/curses.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep

// Função principal do programa.
// Passos:
// 1. Carrega a configuração do jogo a partir do arquivo "config_domino.txt".
// 2. Inicializa a interface do usuário (UI) usando PDCurses.
// 3. Entra em um loop principal onde exibe o menu principal e processa as escolhas do usuário:
//    - Novo jogo: Permite configurar e iniciar um novo jogo.
//    - Carregar jogo: Lista e carrega jogos salvos.
//    - Regras: Exibe as regras do jogo.
//    - Sair: Encerra o programa.
// 4. Para um novo jogo:
//    - Seleciona o modo (vs IA ou vs Jogador).
//    - Define o número de jogadores.
//    - Inicializa o estado do jogo.
//    - Solicita nomes para jogadores humanos.
//    - Entra no loop do jogo, alternando turnos entre jogadores humanos e IA.
// 5. No loop do jogo:
//    - Desenha o estado atual do jogo.
//    - Para jogadores humanos: Verifica se podem jogar, permite entrada de jogadas ou pescar/pegar peças.
//    - Para IA: Simula jogadas automaticamente.
//    - Verifica condições de vitória após cada jogada.
//    - Salva o histórico do jogo ao final.
// 6. Fecha a UI ao sair.
int main() {
  GameConfig config;
  load_config("config_domino.txt", &config); // Carrega configurações do arquivo

  init_ui(); // Inicializa a interface curses

  int running = 1;
  while (running) {
    int choice = show_main_menu();
    GameState game;
    int ready_to_play = 0;

    if (choice == 4) {
      running = 0;
    } else if (choice == 3) {
      show_rules();
    } else if (choice == 2) {
      // Carregar Jogo
      char **files = NULL;
      int count = list_save_files(&files);
      int selected = show_load_menu(files, count);
      if (selected != -1) {
        if (load_game_state(&game, files[selected])) {
          display_message("Jogo Carregado com Sucesso!");
          napms(1000);
          ready_to_play = 1;
        } else {
          display_message("Erro ao carregar jogo.");
          wait_for_key();
        }
      }
      free_save_files(files, count);
    } else if (choice == 1) {
      // Novo Jogo
      int mode = show_mode_menu(); // 1=Jogador vs IA, 2=Jogador vs Jogador
      int num_p = get_player_count();
      int humans = (mode == 1) ? 1 : num_p;

      init_game(&game, num_p, humans);

      // Obter nomes para jogadores humanos
      for (int i = 0; i < num_p; i++) {
        if (game.players[i].is_human) {
          get_player_name(game.players[i].name, i + 1);
        }
      }
      ready_to_play = 1;
    }

    // Ciclo Principal do Jogo
    if (ready_to_play) {
      int game_running = 1;
      while (game_running) {
        draw_game_state(&game);

        int p_idx = game.current_player_index;
        Player *p = &game.players[p_idx];

        // Turno do Humano
        if (p->is_human) {
          // Verificar se existem jogadas possíveis
          int can_move = 0;
          int side_dummy;
          for (int i = 0; i < p->hand_count; i++) {
            if (can_play_piece(&game, p->hand[i], &side_dummy)) {
              can_move = 1;
              break;
            }
          }

          if (!can_move) {
            display_message(
                "Sem jogadas validas. Pressione tecla para pescar...");
            wait_for_key();
            int drew = draw_piece(&game, p_idx);
            if (drew) {
              display_message("Pecou uma peca.");
              napms(500);
              continue; // Re-avaliar após pescar
            } else {
              display_message("Baralho vazio. Passou a vez.");
              wait_for_key();
            }
          } else {
            // Loop normal de entrada do utilizador
            int p_idx_input, side;
            int valid_input = get_user_input_move(&p_idx_input, &side);

            if (valid_input == 3) { // Quit
              char response[10];
              get_input_string("Deseja salvar? (s/n): ", response, 5);
              if (response[0] == 's' || response[0] == 'S') {
                char name[50];
                get_input_string("Nome do Save: ", name, 40);
                strcat(name, ".sav");
                if (save_game_state(&game, name)) {
                  display_message("Jogo Salvo!");
                } else {
                  display_message("Erro ao salvar!");
                }
                napms(1000);
              }

              game_running = 0;
              continue;
            } else if (valid_input == 2) {
              display_message("Voce tem jogadas validas! Nao pode passar.");
              wait_for_key();
              continue;
            } else if (valid_input == 1) {
              if (p_idx_input >= 0 && p_idx_input < p->hand_count) {
                int success = play_piece(&game, p_idx, p_idx_input, side);
                if (!success) {
                  display_message("Jogada Invalida!");
                  wait_for_key();
                  continue;
                } else {
                  display_message("Jogada aceite.");
                  napms(500);
                }
              } else {
                display_message("Indice de Peca Invalido.");
                wait_for_key();
                continue;
              }
            }
          }
        } else {
          // Turno da IA
          napms(1000);
          // Ciclo de Pesca da IA
          int moved = 0;
          while (!moved) {
            // Verificar movimentos disponíveis
            int moves_available = 0;
            for (int i = 0; i < p->hand_count; i++) {
              int side_match = 0;
              if (can_play_piece(&game, p->hand[i], &side_match)) {
                int side = (side_match & 1) ? 1 : 2;
                play_piece(&game, p_idx, i, side);
                moved = 1;
                moves_available = 1;
                break;
              }
            }

            if (!moves_available) {
              // Try Draw
              if (draw_piece(&game, p_idx)) {
                display_message("IA pescou uma peca.");
                napms(500);
                draw_game_state(&game);
              } else {
                display_message("IA Passou.");
                moved = 1; // Forçar saída poque não consegue jogar nem pescar
              }
            }
          }
        }

        // Verificar Vencedor
        if (game.winner_index != -1) {
          draw_game_state(&game); // Mostrar final
          // Salvar histórico
          save_history_binary(&game, "historico_jogos.dat");
          save_history_text(&game, "historico_jogos.txt");

          char final_msg[100];
          sprintf(final_msg, "%s Venceu! (Jogo Salvo)",
                  game.players[game.winner_index].name);
          display_message(final_msg);

          wait_for_key();
          game_running = 0;
        }

        // Próximo turno
        if (game_running) {
          game.current_player_index =
              (game.current_player_index + 1) % game.player_count;
        }
      }
    }
  }

  close_ui();
  return 0;
}
