CC= gcc
V= valgrind
GDB= gdb
R= rm
CFLAGS= -std=c99 -g -O0
GFLAGS= -tui
ARCHIVOS= archivo.txt
WFLAGS= -Wall -Wconversion -Werror -lm -Wbad-function-cast
VFLAGS= --leak-check=full --track-origins=yes --show-reachable=yes

usuarios: procesar_usuarios.c count_min_sketch.c  strutil.c 
		$(CC) procesar_usuarios.c count_min_sketch.c  strutil.c  $(CFLAGS) $(WFLAGS) -lm -o usuarios

tweets: procesar_tweets.c count_min_sketch.c  strutil.c cola.c infix.c
		$(CC) procesar_tweets.c count_min_sketch.c  strutil.c cola.c infix.c  $(CFLAGS) $(WFLAGS) -lm -o tweets
valu: usuarios
		$(V) $(VFLAGS) ./usuarios < arch.txt

valt: tweets
		$(V) $(VFLAGS) ./usuarios $(ARCHIVOS) 
