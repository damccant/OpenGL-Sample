exe = sample
libs = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl
rm = rm -f
cxx = g++ -O3
cc = gcc -O3
ld = ld
objects = glad.o vertexShader.o fragmentShader.o shader.o maze.o texture.o path.o object.o game.o

$(target): $(exe)

$(exe): sample.cpp $(objects)
	$(cxx) sample.cpp $(objects) -o $@ $(libs)

windows: exe = sample.exe
windows: rm = del
windows: cc = i686-w64-mingw32-gcc --static
windows: ld = i686-w64-mingw32-ld
windows: $(exe)



glad.o: glad.c
	$(cc) -c $< -o $@

shader.o: shader.cpp shader.h
	$(cxx) -c shader.cpp -o $@

texture.o: texture.cpp texture.h
	$(cxx) -c texture.cpp -o $@

maze.o: maze.cpp maze.h
	$(cxx) -c maze.cpp -o $@

object.o: object.cpp object.h
	$(cxx) -c object.cpp -o $@

game.o: game.cpp game.h
	$(cxx) -c game.cpp -o $@

path.o: path.cpp path.h
	$(cxx) -c path.cpp -o $@

vertexShader.o: vertexShader.vert
	$(ld) -z noexecstack -r -b binary -o $@ $<

fragmentShader.o: fragmentShader.frag
	$(ld) -z noexecstack -r -b binary -o $@ $<

.PHONY: clean
clean:
	-$(rm) $(objects) $(exe)