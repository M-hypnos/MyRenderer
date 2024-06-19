#include <SDL.h>
#include<iostream>
#include "core/FrameBuffer.h"
#include "core/TColor.h"
#include "scene/Scene.h"
#include "core/Pipeline.h"
int WIDTH = 800;
int HEIGHT = 600;

void onDraw(SDL_Renderer* renderer, FrameBuffer* frameBuffer) {
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++) {
            TColor c = frameBuffer->getColor(x, y);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(renderer, x, HEIGHT - y - 1);
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


    std::vector<FrameBuffer*> frameBuffer;
    FrameBuffer* buffer1 = new FrameBuffer(WIDTH, HEIGHT);
    FrameBuffer* buffer2 = new FrameBuffer(WIDTH, HEIGHT);
    frameBuffer.emplace_back(buffer1);
    frameBuffer.emplace_back(buffer2);
    int curBuff = 0;

    Scene scene;
    scene.init();

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
                    case SDLK_w:
                        scene.moveCamera(MoveDir::Top);
                        break;
                    case SDLK_s:
                        scene.moveCamera(MoveDir::Bottom);
                        break;
                    case SDLK_a:
                        scene.moveCamera(MoveDir::Left);
                        break;
                    case SDLK_d:
                        scene.moveCamera(MoveDir::Right);
                        break;
                    case SDLK_z:
                        scene.moveCamera(MoveDir::Back);
                        break;
                    case SDLK_x:
                        scene.moveCamera(MoveDir::Forward);
                        break;
                    case SDLK_UP:
                        scene.moveCameraFront(Vec3f(0, 2, 0));
                        break;
                    case SDLK_DOWN:
                        scene.moveCameraFront(Vec3f(0, -2, 0));
                        break;
                    case SDLK_LEFT:
                        scene.moveCameraFront(Vec3f(-2, 0, 0));
                        break;
                    case SDLK_RIGHT:
                        scene.moveCameraFront(Vec3f(2, 0, 0));
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); // 清空渲染器

        onDraw(renderer, frameBuffer[curBuff]);

        curBuff++;
        curBuff = curBuff % 2;

        PipeLine::getInstance().bindFrameBuffer(frameBuffer[curBuff]);

        scene.onRender();

        SDL_RenderPresent(renderer); // 将绘制的内容显示到窗口上

    }

    for (auto buffer : frameBuffer) {
        delete buffer;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}