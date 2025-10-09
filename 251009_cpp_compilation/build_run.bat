@echo off
chcp 1251 > log
def log

set MAIN=main
set EXE=example
set CHARSET="-finput-charset=utf-8 -fexec-charset=windows-1251"

g++ "%CHARSET%" %MAIN%.cpp -o %EXE%

example.exe