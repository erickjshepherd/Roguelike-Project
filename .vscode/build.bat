@echo off
setlocal enabledelayedexpansion

set "ROOT=%~1"
if "%ROOT%"=="" set "ROOT=%cd%"

set "CXX="
for %%C in (g++.exe x86_64-w64-mingw32-g++.exe clang++.exe) do (
  where %%C >nul 2>nul
  if not errorlevel 1 (
    set "CXX=%%C"
    goto found_compiler
  )
)

:found_compiler
if "%CXX%"=="" (
  echo No GCC, MinGW-w64, or Clang C++ compiler found on PATH.
  echo Install MinGW-w64 or MSYS2 and restart VS Code.
  exit /b 1
)

echo Using compiler: %CXX%

if not exist "%ROOT%\vs_build" mkdir "%ROOT%\vs_build"

%CXX% -std=c++17 -g -O0 -Wall -Wextra -DWIN32 -D_DEBUG -D_CONSOLE -DSDL_MAIN_HANDLED -mconsole ^
  -I"%ROOT%\source" ^
  -I"%ROOT%\SDL2\include" ^
  -I"%ROOT%\SDL2_image\include" ^
  -I"%ROOT%\SDL2_ttf\include" ^
  "%ROOT%\source\*.cpp" ^
  -L"%ROOT%\SDL2\lib\x64" ^
  -L"%ROOT%\SDL2_image\lib\x64" ^
  -L"%ROOT%\SDL2_ttf\lib\x64" ^
  -lSDL2 -lSDL2_image -lSDL2_ttf -lmingw32 ^
  -o "%ROOT%\vs_build\Object_Roguelike.exe"

copy /y "%ROOT%\SDL2\lib\x64\SDL2.dll" "%ROOT%\vs_build\SDL2.dll" >nul
copy /y "%ROOT%\SDL2_image\lib\x64\SDL2_image.dll" "%ROOT%\vs_build\SDL2_image.dll" >nul
copy /y "%ROOT%\SDL2_ttf\lib\x64\SDL2_ttf.dll" "%ROOT%\vs_build\SDL2_ttf.dll" >nul
copy /y "%ROOT%\SDL2_image\lib\x64\libpng16-16.dll" "%ROOT%\vs_build\libpng16-16.dll" >nul
copy /y "%ROOT%\SDL2_image\lib\x64\zlib1.dll" "%ROOT%\vs_build\zlib1.dll" >nul
copy /y "%ROOT%\SDL2_ttf\lib\x64\libfreetype-6.dll" "%ROOT%\vs_build\libfreetype-6.dll" >nul

exit /b %errorlevel%
