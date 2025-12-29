@echo off
echo Compiling Domino Muggins for Windows...

:: Check if pdcurses exists
if not exist "lib\pdcurses\curses.h" (
    echo [ERROR] PDCurses headers not found in 'lib\pdcurses' folder.
    echo Please create a folder named 'lib\pdcurses' and place 'curses.h' and 'pdcurses.a' inside.
    pause
    exit /b 1
)

:: Compile
:: Linking order matters. -lpdcurses should be after source files using it.
gcc src\main.c src\game.c src\domino.c src\player.c src\config.c src\ui.c src\storage.c -o domino.exe -I./include -I./lib/pdcurses lib\pdcurses\pdcurses.a

if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed.
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Compilation successful!
echo Run 'domino.exe' to play.
pause
