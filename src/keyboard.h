#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>
#include <SDL2/SDL.h>

class Keyboard {
public:
    void handleEvent(const SDL_Event& event);
    void updateKeys(std::array<uint8_t, 16>& key);

private:
    std::array<uint8_t, 16> currentKeys;
    std::array<uint8_t, 16> previousKeys;
};

#endif 