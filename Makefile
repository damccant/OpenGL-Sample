ifeq ($(OS), Windows_NT)
	exe = catnmouse.exe
	libs = -lglfw3 -lopengl32 -lgdi32
	rm = del
	cxx = c++ -O3
	cc = gcc -O3
	ld = ld
	strip = strip
else
	exe = catnmouse
	libs = `pkg-config --static --libs glfw3`
	rm = rm -f
	cxx = g++ -O3
	cc = gcc -O3
	ld = ld -z noexecstack
	strip = strip
endif

objects = glad.o vertexShader.o fragmentShader.o shader.o maze.o texture.o path.o object.o game.o

$(target): $(exe)

$(exe): main.cpp $(objects)
	$(cxx) $(cxxflags) -o $(exe) main.cpp $(objects) $(libs)
	$(strip) $(exe)

cross: exe = catnmouse.exe
cross: cxxflags = -static -mwindows
cross: cxx = i686-w64-mingw32-g++ -I/usr/local/include -O3
cross: cc = i686-w64-mingw32-gcc -O3
cross: ld = i686-w64-mingw32-ld
cross: strip = i686-w64-mingw32-strip
cross: $(exe)

static: cxxflags = -static
static: $(exe)

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
	$(ld) -r -b binary -o $@ $<

fragmentShader.o: fragmentShader.frag
	$(ld) -r -b binary -o $@ $<

.PHONY: clean
clean:
	-$(rm) $(objects) $(exe)