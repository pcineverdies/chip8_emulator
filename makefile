X11_FLAGS =  -L/usr/X11/lib -lX11 -lstdc++
BUILD_FOLDER = build
SOURCE_FOLDER = src
OUT_NAME = chip8_emulator

emulator: main.o memory.o chip8.o keyboard.o
	g++ -o $(OUT_NAME) $(BUILD_FOLDER)/*.o $(X11_FLAGS)

main.o: directories
	g++ -c $(SOURCE_FOLDER)/main.cpp -o $(BUILD_FOLDER)/main.o

memory.o: directories
	g++ -c $(SOURCE_FOLDER)/memory.cpp -o $(BUILD_FOLDER)/memory.o

chip8.o: directories
	g++ -c $(SOURCE_FOLDER)/chip8.cpp -o $(BUILD_FOLDER)/chip8.o

keyboard.o: directories
	g++ -c $(SOURCE_FOLDER)/keyboard.cpp $(X11_FLAGS) -o $(BUILD_FOLDER)/keyboard.o

directories:
	mkdir ${BUILD_FOLDER}
