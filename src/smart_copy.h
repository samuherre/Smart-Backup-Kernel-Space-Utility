#ifndef SMART_COPY_H
#define SMART_COPY_H

#include <stddef.h>

// Tamaño del buffer (4KB - tamaño de página)
#define BUFFER_SIZE 4096

// Flags (por si quieres extender funcionalidad después)
#define COPY_SUCCESS 0
#define COPY_ERROR   -1

// -------- COLORES ANSI --------
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_CYAN    "\033[1;36m"

// Firma de la función tipo "system call"
int sys_smart_copy(const char *src_path, const char *dest_path);

#endif