VERSION = -std=c++20
OPTIMIERUNG = -O2 
LIB = -lSDL2


debug:
	g++ -g -o out/emulator.exe src/*.cpp $(LIB)

build:
	g++ $(VERSION) $(OPTIMIERUNG) -o out/emulator.exe src/*.cpp $(LIB)

run:
	g++ $(VERSION) $(OPTIMIERUNG) -o out/emulator.exe src/*.cpp $(LIB)
	./out/emulator.exe out/roms/PONG
