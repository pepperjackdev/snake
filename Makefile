# Runs the project
r: b
	./build/*.exe

# Builds the sources
b: g
	ninja -C build

# Generates the build-system's scripts
g:
	cmake -S . -B build/

# Runs the project on a web browser
er: eb
	emrun wabuild/snake.html

# Builds the sources with emcc
eb: eg
	cmake --build wabuild 

# Generates the build-system's scripts for emscripten
eg:
	emcmake cmake -B wabuild/

