#include "keyboard.h"

void Keyboard::handleEvent(const SDL_Event& event) {
    //SDL Key Event abgreifen
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        //schauen ob eine taste gedrückt oder losgelassen wurde
        bool isPressed = event.type == SDL_KEYDOWN;
        //prüfen und setzen welche taste gesetzt wurde 
        switch (event.key.keysym.sym) {
            case SDLK_x: currentKeys[0] = isPressed; break;
            case SDLK_1: currentKeys[1] = isPressed; break;
            case SDLK_2: currentKeys[2] = isPressed; break;
            case SDLK_3: currentKeys[3] = isPressed; break;
            case SDLK_q: currentKeys[4] = isPressed; break;
            case SDLK_w: currentKeys[5] = isPressed; break;
            case SDLK_e: currentKeys[6] = isPressed; break;
            case SDLK_a: currentKeys[7] = isPressed; break;
            case SDLK_s: currentKeys[8] = isPressed; break;
            case SDLK_d: currentKeys[9] = isPressed; break;
            case SDLK_z: currentKeys[10] = isPressed; break;
            case SDLK_c: currentKeys[11] = isPressed; break;
            case SDLK_4: currentKeys[12] = isPressed; break;
            case SDLK_r: currentKeys[13] = isPressed; break;
            case SDLK_f: currentKeys[14] = isPressed; break;
            case SDLK_v: currentKeys[15] = isPressed; break;
        }
    }
}

void Keyboard::updateKeys(std::array<uint8_t, 16>& key) {
    for (int i = 0; i < 16; ++i) {
        key[i] = currentKeys[i];
    }
}