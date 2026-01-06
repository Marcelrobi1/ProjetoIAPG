@echo off
echo Compiling Domino Muggins for Windows...

:: Check if pdcurses exists
if not exist "lib\pdcurses\PDCurses-3.9\curses.h" (
    echo [ERROR] PDCurses headers not found in 'lib\pdcurses\PDCurses-3.9' folder.
    echo Please ensure PDCurses is extracted to 'lib\pdcurses\PDCurses-3.9' with wincon\pdcurses.a.
    pause
    exit /b 1
)

:: Compile
:: Linking order matters. -lpdcurses should be after source files using it.
gcc src\main.c src\game.c src\domino.c src\player.c src\config.c src\ui.c src\storage.c -o domino.exe -I./include lib\pdcurses\PDCurses-3.9\wincon\pdcurses.a

if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed.
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Compilation successful!
echo Run 'domino.exe' to play.
pause
