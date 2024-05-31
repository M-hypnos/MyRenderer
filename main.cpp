#include <SDL.h>
#include<iostream>
#include "core/RenderBuffer.h"
#include "utils/TColor.h"

size_t WIDTH = 800;
size_t HEIGHT = 600;

void onDraw(SDL_Renderer* renderer, RenderBuffer* renderBuffer) {
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++) {
            TColor c = renderBuffer->getColor(x, y);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("MyRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }


    bool isRunning = true;

    SDL_Event ev;

    RenderBuffer* renderBuffer = new RenderBuffer(WIDTH, HEIGHT);

    while (isRunning) {
        while (SDL_PollEvent(& ev)) {
            if (ev.type == SDL_QUIT) {
                isRunning = false;
            }
            else if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        isRunning = false;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); // 清空渲染器

        onDraw(renderer, renderBuffer);

        SDL_RenderPresent(renderer); // 将绘制的内容显示到窗口上

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}