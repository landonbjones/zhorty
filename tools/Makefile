CC=gcc
CFLAGS=-Iinclude/ -std=c99 -DHASH_FUNCTION=HASH_SFH -O3
DEPS = include/shorty.h 
#MYSQL_FLAGS = $(shell mysql_config --cflags --libs)
SRC_DIR = src/
OBJ_DIR = obj/
 
all: shorty shorty_gen shorty_stats shorty_bench

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

shorty: $(OBJ_DIR)shorty.o $(OBJ_DIR)shorty_functions.o  
	$(CC) -o shorty $(OBJ_DIR)shorty.o $(OBJ_DIR)shorty_functions.o $(CFLAGS)

shorty_gen: $(OBJ_DIR)shorty_gen.o $(OBJ_DIR)shorty_functions.o 
	$(CC) -o shorty_gen $(OBJ_DIR)shorty_gen.o $(OBJ_DIR)shorty_functions.o  $(CFLAGS)

shorty_stats: $(OBJ_DIR)shorty_stats.o $(OBJ_DIR)shorty_functions.o 
	$(CC) -o shorty_stats $(OBJ_DIR)shorty_stats.o $(OBJ_DIR)shorty_functions.o  $(CFLAGS)

shorty_bench: $(OBJ_DIR)shorty_bench.o $(OBJ_DIR)shorty_functions.o $(OBJ_DIR)bench_games.o $(OBJ_DIR)hard_bench_games.o
	$(CC) -o shorty_bench $(OBJ_DIR)shorty_bench.o $(OBJ_DIR)shorty_functions.o $(OBJ_DIR)bench_games.o $(OBJ_DIR)hard_bench_games.o $(CFLAGS)

.PHONY: clean

.PHONY: build

clean:
	rm -rf $(OBJ_DIR)*o shorty shorty_gen shorty_stats shorty_bench
	