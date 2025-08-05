# Runs the project
r: b
	./build/*.exe

# Builds the sources
b: g
	ninja -C build

# Generates the build-system's scripts
g:
	cmake -S . -B build/

eg:
	emcmake cmake -B wabuild/

eb: eg
	cmake --build wabuild 

er: eb
	emrun wabuild/snake.html