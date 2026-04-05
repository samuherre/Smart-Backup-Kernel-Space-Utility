#include "smart_copy.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define BAR_WIDTH 40

static void print_progress(long bytes_copied, long total) {
    float percent = (float)bytes_copied / total;
    int filled = (int)(percent * BAR_WIDTH);

    printf("\r" COLOR_CYAN "[");
    for (int i = 0; i < BAR_WIDTH; i++) {
        if (i < filled)        printf("█");
        else if (i == filled)  printf("▓");
        else                   printf("░");
    }
    printf("] " COLOR_GREEN "%3d%%" COLOR_RESET, (int)(percent * 100));
    fflush(stdout);
}

int sys_smart_copy(const char *src_path, const char *dest_path) {
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    // -------- LOG --------
    FILE *log = fopen("backup.log", "a");
    if (log != NULL) {
        time_t now = time(NULL);
        fprintf(log, "[%s] Iniciando copia: %s -> %s\n", ctime(&now), src_path, dest_path);
    }

    // -------- ABRIR ORIGEN --------
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        fprintf(stderr, COLOR_RED "Error abriendo '%s': %s\n" COLOR_RESET, src_path, strerror(errno));

        if (errno == EACCES) {
            fprintf(stderr, COLOR_YELLOW "Permiso denegado\n" COLOR_RESET);
        }

        if (log != NULL) {
            fprintf(log, "Error al abrir archivo origen\n\n");
            fclose(log);
        }

        return COPY_ERROR;
    }

    // -------- TAMAÑO DEL ARCHIVO --------
    struct stat file_info;
    long total_bytes = 0;
    if (fstat(src_fd, &file_info) == 0) {
        total_bytes = file_info.st_size;
    }

    // -------- CREAR DESTINO --------
    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        fprintf(stderr, COLOR_RED "Error creando '%s': %s\n" COLOR_RESET, dest_path, strerror(errno));

        if (errno == EACCES) {
            fprintf(stderr, COLOR_YELLOW "Permiso denegado\n" COLOR_RESET);
        }

        close(src_fd);

        if (log != NULL) {
            fprintf(log, "Error al crear archivo destino\n\n");
            fclose(log);
        }

        return COPY_ERROR;
    }

    // -------- COPIA --------
    long bytes_copied = 0;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {

        bytes_written = write(dest_fd, buffer, bytes_read);

        if (bytes_written < 0) {
            fprintf(stderr, COLOR_RED "Error escribiendo: %s\n" COLOR_RESET, strerror(errno));

            if (errno == ENOSPC) {
                fprintf(stderr, COLOR_YELLOW "No hay espacio en disco\n" COLOR_RESET);
            }

            close(src_fd);
            close(dest_fd);

            if (log != NULL) {
                fprintf(log, "Error durante escritura\n\n");
                fclose(log);
            }

            return COPY_ERROR;
        }

        // -------- PROGRESO --------
        if (total_bytes > 0) {
            bytes_copied += bytes_written;
            print_progress(bytes_copied, total_bytes);
        }
    }

    if (bytes_read < 0) {
        fprintf(stderr, COLOR_RED "Error leyendo: %s\n" COLOR_RESET, strerror(errno));

        if (log != NULL) {
            fprintf(log, "Error durante lectura\n\n");
            fclose(log);
        }

        close(src_fd);
        close(dest_fd);
        return COPY_ERROR;
    }

    // -------- CIERRE --------
    close(src_fd);
    close(dest_fd);

    printf("\n");

    // -------- LOG FINAL --------
    if (log != NULL) {
        fprintf(log, "Copia finalizada correctamente\n\n");
        fclose(log);
    }

    return COPY_SUCCESS;
}