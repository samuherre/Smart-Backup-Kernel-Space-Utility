all:
	gcc main.c backup_engine.c -o backup

run:
	./backup archivo.txt copia.txt

clean:
	rm -f backup copy_stdio.txt copia.txt backup.log