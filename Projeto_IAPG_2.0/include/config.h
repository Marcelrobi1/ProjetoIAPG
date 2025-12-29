#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
  int spinner_mode; // 0 = nenhum, 1 = primeiro duplo, 2 = todos os duplos
                    // Configurações futuras potenciais:
                    // int starting_pieces;
                    // int win_score;
} GameConfig;

// Function prototypes
int load_config(const char *filename, GameConfig *config);

#endif // CONFIG_H
