#include "smart_copy.h"
#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close
#include <stdio.h>
#include <errno.h>
#include <string.h>

int sys_smart_copy(const char *src_path, const char *dest_path) {
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    
    char buffer[BUFFER_SIZE];

    // Abrir archivo origen
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        perror("Error abriendo archivo origen");
        return COPY_ERROR;
    }

    // Crear archivo destino
    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error creando archivo destino");
        close(src_fd);
        return COPY_ERROR;
    }

    // Leer y escribir usando buffer
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        
        bytes_written = write(dest_fd, buffer, bytes_read);

        if (bytes_written < 0) {
            perror("Error escribiendo archivo");
            close(src_fd);
            close(dest_fd);
            return COPY_ERROR;
        }
    }

    if (bytes_read < 0) {
        perror("Error leyendo archivo");
    }

    // Cerrar archivos
    close(src_fd);
    close(dest_fd);

    return COPY_SUCCESS;
}