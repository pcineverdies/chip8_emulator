X11_FLAGS =  -L/usr/X11/lib -lX11 -lstdc++
SDL2_FLAGS = -lSDL2
BUILD_FOLDER = build
SOURCE_FOLDER = src
OUT_NAME = chip8_emulator

emulator: main.o memory.o chip8.o keyboard.o display.o
	g++ -o $(BUILD_FOLDER)/$(OUT_NAME) $(BUILD_FOLDER)/*.o $(X11_FLAGS) $(SDL2_FLAGS)

main.o:
	g++ -c $(SOURCE_FOLDER)/main.cpp -o $(BUILD_FOLDER)/main.o

display.o:
	g++ -c $(SOURCE_FOLDER)/display.cpp $(X11_FLAGS) -o $(BUILD_FOLDER)/display.o $(SDL2_FLAGS)

memory.o:
	g++ -c $(SOURCE_FOLDER)/memory.cpp -o $(BUILD_FOLDER)/memory.o

chip8.o:
	g++ -c $(SOURCE_FOLDER)/chip8.cpp -o $(BUILD_FOLDER)/chip8.o

keyboard.o:
	g++ -c $(SOURCE_FOLDER)/keyboard.cpp $(X11_FLAGS) -o $(BUILD_FOLDER)/keyboard.o

directories:
	mkdir -p ${BUILD_FOLDER}
