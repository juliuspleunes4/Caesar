@echo off
cd /d C:\Users\Gebruiker\Desktop\projects\Caesar
C:\msys64\mingw64\bin\c++.exe -Iinclude -Wall -Wextra -std=gnu++17 -fsyntax-only src\codegen\codegen.cpp > compile_output.txt 2>&1
type compile_output.txt
