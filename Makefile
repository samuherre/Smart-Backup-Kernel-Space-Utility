# Compilador
CC = gcc

# Archivos fuente
SRC = src/main.c src/backup_engine.c

# Ejecutable
OUT = bin/backup

# Compilación
all:
	$(CC) $(SRC) -o $(OUT)

# Ejecutar ejemplo
run:
	./bin/backup tests/test_1KB.bin outputs/copia1.txt

# Limpiar archivos generados
clean:
	rm -f $(OUT)
	rm -f outputs/*
	rm -f logs/*