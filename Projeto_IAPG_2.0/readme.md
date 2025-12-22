# Domino Muggins (Five-Up) Implementation

## Introdução
Esta aplicação implementa o jogo do Dominó na variante Muggins (semelhante ao Five-Up) em linguagem C, utilizando a biblioteca `ncurses` para a interface gráfica no terminal.

## Compilação e Execução
### Requisitos
- Compilador C (GCC, MinGW, etc.)
- Biblioteca `ncurses` (ou `pdcurses` em Windows)

### Como Compilar
```bash
gcc main.c game.c domino.c player.c config.c ui.c storage.c -o domino -lncurses
```
Nota: Em Windows, pode ser necessário especificar caminhos para include e lib do pdcurses.

### Instalação Windows (Detalhada)
1. Baixe o **PDCurses** (versão para MinGW/GCC se estiver usando esses compiladores).
   - Sugestão: Procure por "PDCurses binaries for MinGW" ou compile a partir do código fonte.
   - Alternativa comum: Baixar `pdcurses.a` e `curses.h`.
2. Crie uma pasta chamada `pdcurses` dentro da pasta do projeto (`c:/Projeto_IAPG/pdcurses`).
3. Coloque os ficheiros `curses.h` e `pdcurses.a` (ou `libpdcurses.a`) dentro dessa pasta.
4. Execute o script `compile_windows.bat`.

### Como Jogar
Execute o programa:
```bash
./domino
```
Use o menu para iniciar um novo jogo.
- **Teclas**: Siga as instruções no rodapé. Para jogar uma peça, digite: `<Indice da Peça> <Lado (L/R)>`.
- **Exemplo**: `1 L` joga a primeira peça da sua mão na extremidade esquerda.

## Funcionalidades Implementadas (Req Mapping)

| Req ID | Descrição | Estado | Notas |
|--------|-----------|--------|-------|
| 1 | Estrutura `Jogador` | [x] Implementado | Ver `player.h` |
| 2 | Estrutura `Jogo` | [x] Implementado | Ver `game.h`. Inclui baralho, tabuleiro (ends), etc. |
| 3 | Menus Ncurses | [x] Implementado | Ver `ui.c`. Menu principal e visualização. |
| 4 | Inicialização (Shuffle, Deal) | [x] Implementado | `init_game`, `distribute_pieces` em `game.c`. |
| 5 | Jogadas (Validar, Atualizar) | [x] Implementado | `play_piece`, `can_play_piece`. Include draw logic. |
| 6 | Log das jogadas | [/] Parcial | O estado do jogo é atualizado, mas o log detalhado passo-a-passo em memória não é persistido além do estado atual, exceto no report final. |
| 7 | Jogador Artificial (AI) | [x] Implementado | AI joga primeira peça válida ou pesca. |
| 8 | Variações (Config) | [/] Parcial | `config_domino.txt` é lido, mas a lógica de Spinner suporta apenas o modo padrão lineal (config 0) por enquanto. |
| 9 | Leitura Configuração | [x] Implementado | `config.c`. |
| 10.1/2 | Histórico Binário | [x] Implementado | `save_history_binary` em `storage.c`. |
| 10.3/4 | Histórico Texto | [x] Implementado | `save_history_text` em `storage.c`. |

## Observações
- A visualização do tabuleiro é simplificada, mostrando a lista de peças jogadas.
- O cálculo de pontuação segue a regra de múltiplos de 5 nas extremidades.
- Em caso de bloqueio, o jogador compra do baralho (boneyard).
