@echo off
chcp 1251 > log
del log

set MAIN=after_refactoring_4
set CPP_FILES="after_refactoring_4.cpp"
set EXE=example
set CHARSET="-finput-charset=utf-8 -fexec-charset=windows-1251"

if exist %EXE% del %EXE%

g++ "%CHARSET%" "%CPP_FILES%" -o %EXE%

%EXE%