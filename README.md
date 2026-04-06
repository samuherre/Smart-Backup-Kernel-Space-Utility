# Smart Backup Kernel-Space Utility

## Descripción

Este proyecto implementa un sistema de backup en lenguaje C que compara dos enfoques de copia de archivos:

- **System Calls** (simulación de kernel-space)
- **Librería estándar (stdio)**

El objetivo es analizar el rendimiento de ambos métodos y comprender el impacto del buffering y las llamadas al sistema en diferentes tamaños de archivos.

---

## Estructura del Proyecto

Smart-Backup/
│
├── src/ # Código fuente
│ ├── main.c
│ ├── backup_engine.c
│ ├── smart_copy.h
│
├── bin/ # Ejecutable
│ └── backup
│
├── tests/ # Archivos de prueba
│
├── outputs/ # Copias generadas
│
├── logs/ # Archivo de logs
│ └── backup.log
│
├── docs/ # Documentación
│ └── reporte.pdf
│
├── Makefile
├── README.md
├── .gitignore

---

## Requisitos

- gcc
- make (opcional)
- Sistema Linux o WSL

---

## Compilación

### Opción 1: Usando Makefile
```bash
make
```

### Opción 2: Manual
```bash
gcc src/main.c src/backup_engine.c -o bin/backup
```

## Ejecución

### Opción 1: Usando Makefile
```bash
make run
```

### Opción 2: Ejecutar desde la raíz del proyecto:

```bash
./bin/backup <archivo_origen> <archivo_destino>
```

### Ejemplo

```bash
./bin/backup tests/test_1KB.bin outputs/copia1.txt
```

## Pruebas de Rendimiento

Para generar archivos de prueba:

```bash
# 1KB
head -c 1024 /dev/urandom > tests/test_1KB.bin

# 1MB
head -c 1048576 /dev/urandom > tests/test_1MB.bin

# (Opcional) 100MB
head -c 100000000 /dev/urandom > tests/test_100MB.bin
```

El programa mide automáticamente el tiempo de ejecución para:

- System Calls
- stdio (fread / fwrite)

## Resultados

Los resultados muestran:

- Archivos pequeños → stdio es más eficiente (buffering en espacio de usuario)
- Archivos grandes → system calls pueden ser más eficientes
- Uso de buffer de 4KB mejora el rendimiento general

## Conceptos Clave

- System Calls (open, read, write, close)
- Buffering (4KB - tamaño de página)
- Manejo de errores con errno
- Comparación Kernel vs User Space
- Context Switch

## Logs

El sistema genera un archivo de log en:

logs/backup.log

Donde se registran:

- Inicio de copia
- Errores
- Finalización del proceso

## Notas

- Los archivos generados (outputs/, logs/, *.bin) no se incluyen en el repositorio.
- Ejecutar siempre desde la raíz del proyecto para mantener rutas correctas.

## Autores
- Samuel Herrera Galvis
- Simón Castro Valencia

## Estado del Proyecto

✔ Implementación completa
✔ Comparación de rendimiento
✔ Manejo de errores
✔ Logs del sistema
✔ Estructura profesional

Proyecto desarrollado para la materia Sistemas Operativos.