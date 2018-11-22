objetos=main.o bolhas_dinamica.o

test_d: $(objetos)
	gcc $(objetos) -L/usr/lib/i386-linux-gnu -lm -lSDL2 -lSDL2_gfx -o bolhas
	./bolhas -l 15 -a 15 -b 15 -d
#Compila e executa em modo de debug
test_n: $(objetos)
	gcc $(objetos) -L/usr/lib/i386-linux-gnu -lm -lSDL2 -lSDL2_gfx -o bolhas
	./bolhas -l 15 -a 15 -b 15
#Compila e executa em modo normal
bolhas: $(objetos)
	gcc $(objetos) -L/usr/lib/i386-linux-gnu -lm -lSDL2 -lSDL2_gfx -o bolhas
	rm $(objetos)
#Compila
main.o: main.c
	gcc -c  main.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/lib/i386-linux-gnu -lSDL2 -lSDL2_gfx
#Compila o objeto da main
bolhas_dinamica.o: bolhas_dinamica.c
	gcc -c  bolhas_dinamica.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/lib/i386-linux-gnu -lSDL2 -lSDL2_gfx
#Compila o objeto da bolhas_dinamica
clean:
	rm $(objetos) bolhas resultados.txt
#Elimina tudo exceto o código-fonte
clean_objetos:
	rm $(objetos)
#Elimina apenas os objetos
