windres binary/appdata.rc binary/appdata.o
gcc source/main.c -o binary/treenutc.exe binary/appdata.o
gcc -g source/main.c -o binary/treenutcgdb.exe