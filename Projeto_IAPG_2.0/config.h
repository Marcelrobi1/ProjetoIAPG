#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
  int spinner_mode; // 0 = none, 1 = first double, 2 = all doubles
                    // Potential future configs:
                    // int starting_pieces;
                    // int win_score;
} GameConfig;

// Function prototypes
int load_config(const char *filename, GameConfig *config);

#endif // CONFIG_H
