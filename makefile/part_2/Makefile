mp3:main.o mp3.o
	gcc -o mp3 main.o mp3.o

main.o:
	gcc -c main.c -o main.o

mp3.o:
	gcc -c mp3.c -o mp3.o

.PHONY:clean
clean:
	rm mp3
