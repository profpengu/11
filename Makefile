ifeq ($(TARGET), windows)
	FLAGS = -g -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wpedantic -Werror -std=c99
else
	FLAGS = -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wpedantic -Werror -std=c99
endif

default: towergame

util.o: util.c util.h
	gcc -c util.c -o util.o $(FLAGS)

balloon.o: balloon.c balloon.h util.h
	gcc -c balloon.c -o balloon.o $(FLAGS)

monkey.o: monkey.c monkey.h util.h
	gcc -c monkey.c -o monkey.o $(FLAGS)

main.o: main.c balloon.o monkey.o util.o
	gcc -c main.c -o main.o $(FLAGS)

towergame: main.o
	gcc main.o util.o balloon.o monkey.o -o main $(FLAGS)
