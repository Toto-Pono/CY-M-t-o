exe: ABR.o main.o
	gcc ABR.o main.o -o exe -Wall
ABR.o: ABR.c ABR.h
	gcc -c ABR.c  -o ABR.o -Wall
main.o: main.c ABR.h
	gcc -c main.c -o main.o -Wall
clean:
	rm -f *.o
	rm exe
