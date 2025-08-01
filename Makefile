# Runs the project
run: build
	./build/*.exe

# Builds the sources
build: generate
	ninja -C build

# Generates the build-system's scripts
generate:
	cmake -S . -B build/