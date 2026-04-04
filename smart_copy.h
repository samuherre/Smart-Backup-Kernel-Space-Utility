#ifndef SMART_COPY_H
#define SMART_COPY_H

#include <stddef.h>

// Tamaño del buffer (4KB - tamaño de página)
#define BUFFER_SIZE 4096

// Flags (por si quieres extender funcionalidad después)
#define COPY_SUCCESS 0
#define COPY_ERROR   -1

// Firma de la función tipo "system call"
int sys_smart_copy(const char *src_path, const char *dest_path);

#endif