#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
  int spinner_mode; // 0 = nenhum, 1 = primeiro duplo, 2 = todos os duplos
} GameConfig;

// Function prototypes
int load_config(const char *filename, GameConfig *config);

#endif // CONFIG_H
