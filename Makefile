sample: sample.cpp glad.o
	g++ sample.cpp -o sample -lglfw -lGL -lX11 -lpthread -lXrandr -ldl glad.o

glad.o: glad.c
	gcc -c glad.c
