#include "chip8.h"
#include "display.h"
#include "keyboard.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    using namespace std::chrono_literals;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
        return 1;
    }

    Chip8 chip8;
    Display display;
    Keyboard keyboard;

    if (!chip8.loadROM(argv[1])) {
        std::cerr << "Failed to load ROM file." << std::endl;
        return 1;
    }

    if (!display.init()) {
        std::cerr << "Failed to initialize display." << std::endl;
        return 1;
    }

    bool running = true;
    SDL_Event event;

    //Main loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            keyboard.handleEvent(event);
        }

        chip8.emulateCycle();

        if (chip8.drawFlag) {
            display.update(chip8.gfx);
            chip8.drawFlag = false;
        }

        keyboard.updateKeys(chip8.key);

        std::this_thread::sleep_for(5ms);
    }

    display.cleanup();
    return 0;
}