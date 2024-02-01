exe = sample
libs = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl
objects = glad.o vertexShader.o fragmentShader.o shader.o
rm = rm -f
cxx = g++ -O3
cc = gcc -O3
ld = ld

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
	$(cxx) -c shader.cpp

vertexShader.o: vertexShader.vert
	$(ld) -z noexecstack -r -b binary -o $@ $<

fragmentShader.o: fragmentShader.frag
	$(ld) -z noexecstack -r -b binary -o $@ $<

.PHONY: clean
clean:
	-$(rm) $(objects) $(exe)