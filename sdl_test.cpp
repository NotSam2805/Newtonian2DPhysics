#include <iostream>
#include "SDL2/include/SDL.h"

const int WIDTH = 800; const int HEIGHT = 600;

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (NULL == window){
        std::cout << "Cannot create window" << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    if (NULL == renderer){
        std::cout << "Cannot create renderer" << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE); // Set colour to dark grey
    SDL_RenderClear(renderer);

    /* draw a filled rectangle in the middle of the canvas. */
    SDL_Rect rect;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = rect.y = 100;
    rect.w = 440;
    rect.h = 280;
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer); // Show rendered objects

    SDL_Event windowEvent;

    while (true){
        if (SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type){
                break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return EXIT_SUCCESS;
}