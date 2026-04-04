#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "smart_copy.h"

#define STD_BUFFER 4096

// Copia usando stdio (fread / fwrite)
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

    // Archivos de salida diferentes
    const char *dest_sys = "copy_syscall.txt";
    const char *dest_std = "copy_stdio.txt";

    clock_t start, end;
    double time_sys, time_std;

    // ----------- SYSTEM CALL -----------
    start = clock();
    sys_smart_copy(src, dest_sys);
    end = clock();

    time_sys = ((double)(end - start)) / CLOCKS_PER_SEC;

    // ----------- STDIO -----------
    start = clock();
    copy_with_stdio(src, dest_std);
    end = clock();

    time_std = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Resultados
    printf("\nResultados:\n");
    printf("System Call Copy: %f segundos\n", time_sys);
    printf("Stdio Copy:       %f segundos\n", time_std);

    return 0;
}