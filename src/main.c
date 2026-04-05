#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include "smart_copy.h"

#define STD_BUFFER 4096

int copy_with_stdio(const char *src, const char *dest) {
    FILE *f_src = fopen(src, "rb");
    if (!f_src) {
        perror(COLOR_RED "Error abriendo origen (stdio)" COLOR_RESET);
        return -1;
    }

    FILE *f_dest = fopen(dest, "wb");
    if (!f_dest) {
        perror(COLOR_RED "Error creando destino (stdio)" COLOR_RESET);
        fclose(f_src);
        return -1;
    }

    char buffer[STD_BUFFER];
    size_t bytes;

    while ((bytes = fread(buffer, 1, STD_BUFFER, f_src)) > 0) {
        fwrite(buffer, 1, bytes, f_dest);
    }

    fclose(f_src);
    fclose(f_dest);

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf(COLOR_YELLOW "Uso: %s <origen> <destino>\n" COLOR_RESET, argv[0]);
        return 1;
    }

    const char *src  = argv[1];
    const char *dest = argv[2];

    // -------- DETECTAR SI ES CARPETA O ARCHIVO --------
    struct stat src_info;
    if (stat(src, &src_info) < 0) {
        fprintf(stderr, COLOR_RED "No se puede acceder a '%s'\n" COLOR_RESET, src);
        return 1;
    }

    if (S_ISDIR(src_info.st_mode)) {
        // -------- MODO CARPETA --------
        return backup_dir(src, dest);
    }

    // -------- MODO ARCHIVO (comportamiento original) --------
    const char *dest_std = "copy_stdio.txt";

    clock_t start, end;
    double time_sys = 0;
    double time_std = 0;

    int repetitions = 3;

    // -------- SYSTEM CALL TEST --------
    for (int i = 0; i < repetitions; i++) {
        start = clock();
        sys_smart_copy(src, dest);
        end = clock();
        time_sys += ((double)(end - start)) / CLOCKS_PER_SEC;
    }

    time_sys /= repetitions;

    // -------- STDIO TEST --------
    for (int i = 0; i < repetitions; i++) {
        start = clock();
        copy_with_stdio(src, dest_std);
        end = clock();
        time_std += ((double)(end - start)) / CLOCKS_PER_SEC;
    }

    time_std /= repetitions;

    // -------- RESULTADOS --------
    printf(COLOR_CYAN "\nResultados promedio (%d ejecuciones):\n" COLOR_RESET, repetitions);
    printf(COLOR_GREEN "System Call Copy: %f segundos\n" COLOR_RESET, time_sys);
    printf(COLOR_GREEN "Stdio Copy:       %f segundos\n" COLOR_RESET, time_std);

    return 0;
}