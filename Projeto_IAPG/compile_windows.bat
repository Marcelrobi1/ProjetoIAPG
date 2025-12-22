@echo off
echo Compiling Domino Muggins for Windows...

:: Check if pdcurses exists
if not exist "pdcurses\curses.h" (
    echo [ERROR] PDCurses headers not found in 'pdcurses' folder.
    echo Please create a folder named 'pdcurses' and place 'curses.h' and 'pdcurses.a' inside.
    pause
    exit /b 1
)

:: Compile
:: Linking order matters. -lpdcurses should be after source files using it.
gcc main.c game.c domino.c player.c config.c ui.c storage.c -o domino.exe -I./pdcurses pdcurses/pdcurses.a

if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed.
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Compilation successful!
echo Run 'domino.exe' to play.
pause
