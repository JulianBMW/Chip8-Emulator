#include "display.h"
#include <iostream>
#include <array>
#include <cstdint>

Display::Display() : window(nullptr), renderer(nullptr), texture(nullptr) {}

bool Display::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 64, 32);
    if (!texture) {
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Display::update(const std::array<uint8_t, 64 * 32>& gfx) {
    //konvertier pixels
    std::array<uint32_t, 64 * 32> pixels;
    for (size_t i = 0; i < gfx.size(); ++i) {
        pixels[i] = (gfx[i] ? 0x66FF66FF : 0x000000FF);
        //0xFFFFFFFF = Weiß  und 0x000000FF = Schwarz
        //Apple 2 oldshool color = 0x66FF66FF
    }

    //update display
    SDL_UpdateTexture(texture, nullptr, pixels.data(), 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Display::cleanup() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}