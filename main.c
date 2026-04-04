#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "smart_copy.h"

#define STD_BUFFER 4096

int copy_with_stdio(const char *src, const char *dest) {
    FILE *f_src = fopen(src, "rb");
    if (!f_src) {
        perror("Error abriendo origen (stdio)");
        return -1;
    }

    FILE *f_dest = fopen(dest, "wb");
    if (!f_dest) {
        perror("Error creando destino (stdio)");
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
        printf("Uso: %s <origen> <destino>\n", argv[0]);
        return 1;
    }

    const char *src = argv[1];
    const char *dest = argv[2];

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
    printf("\nResultados promedio (%d ejecuciones):\n", repetitions);
    printf("System Call Copy: %f segundos\n", time_sys);
    printf("Stdio Copy:       %f segundos\n", time_std);

    return 0;
}