# Smart Backup Kernel-Space Utility

## Descripción
Este proyecto implementa un sistema de backup en C que compara dos enfoques:

- System Calls (simulación de kernel-space)
- Librería estándar (stdio)

## Estructura

- smart_copy.h → Definición de la función
- backup_engine.c → Lógica de copia con system calls
- main.c → Comparación de rendimiento
- backup.log → Registro de operaciones
- reporte.pdf → Análisis del proyecto

## Compilación

```bash
gcc main.c backup_engine.c -o backup
```

## Ejecución

```bash
./backup archivo_origen archivo_destino
```

## Pruebas

Se recomienda probar con diferentes tamaños:

```bash
head -c 1024 /dev/urandom > test_1KB.bin
head -c 1048576 /dev/urandom > test_1MB.bin
```

## Conceptos Clave

- System Calls
- Buffering (4KB)
- Manejo de errores con errno
- Comparación de rendimiento

## Autores

- Samuel Herrera Galvis
- Simón Castro Valencia