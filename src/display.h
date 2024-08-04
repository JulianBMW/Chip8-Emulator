#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>
#include <array>
#include <SDL2/SDL.h>

class Display {
public:
    Display();
    bool init();
    void update(const std::array<uint8_t, 64 * 32>& gfx);
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif 