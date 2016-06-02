#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
    SDL_Window* window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    SDL_Delay(1000);

    SDL_DestroyWindow(window);

    return 0;
}