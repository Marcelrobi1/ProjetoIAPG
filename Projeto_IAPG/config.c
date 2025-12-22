#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int load_config(const char *filename, GameConfig *config) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    // Default values if file missing
    config->spinner_mode = 0;
    return 0; // File not found
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    // Simple key=value parsing
    char *key = strtok(line, "=");
    char *value = strtok(NULL, "\n"); // Remove newline

    if (key && value) {
      if (strcmp(key, "spinner_mode") == 0) {
        config->spinner_mode = atoi(value);
      }
    }
  }

  fclose(file);
  return 1; // Success
}
