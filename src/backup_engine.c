#include "smart_copy.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
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
    FILE *log = fopen("logs/backup.log", "a");
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

// -------- BACKUP DE CARPETA --------
int backup_dir(const char *src_dir, const char *dest_dir) {
    DIR *dir = opendir(src_dir);
    if (!dir) {
        fprintf(stderr, COLOR_RED "Error abriendo carpeta '%s': %s\n" COLOR_RESET, src_dir, strerror(errno));
        return COPY_ERROR;
    }

    // Crear carpeta destino si no existe
    if (mkdir(dest_dir, 0755) < 0 && errno != EEXIST) {
        fprintf(stderr, COLOR_RED "Error creando carpeta '%s': %s\n" COLOR_RESET, dest_dir, strerror(errno));
        closedir(dir);
        return COPY_ERROR;
    }

    printf(COLOR_CYAN "Carpeta: %s -> %s\n" COLOR_RESET, src_dir, dest_dir);

    struct dirent *entry;
    struct stat entry_info;
    char src_path[1024];
    char dest_path[1024];

    while ((entry = readdir(dir)) != NULL) {

        // Saltar . y ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(src_path,  sizeof(src_path),  "%s/%s", src_dir,  entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

        if (stat(src_path, &entry_info) < 0) {
            fprintf(stderr, COLOR_YELLOW "No se pudo leer '%s', saltando...\n" COLOR_RESET, src_path);
            continue;
        }

        if (S_ISDIR(entry_info.st_mode)) {
            // Recursión para subcarpetas
            backup_dir(src_path, dest_path);
        } else if (S_ISREG(entry_info.st_mode)) {
            // Archivo normal
            printf(COLOR_CYAN "  Archivo: %s\n" COLOR_RESET, entry->d_name);
            sys_smart_copy(src_path, dest_path);
        }
    }

    closedir(dir);
    return COPY_SUCCESS;
}