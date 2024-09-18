#include <iostream>
#include <SDL.h>
//#include <vld.h>
#include "core/STexture.h"
#include "core/SFrameBuffer.h"
#include "core/SPipeline.h"
#include "entity/Scene.h"

int WIDTH = 800;
int HEIGHT = 600;

void onDraw(SDL_Renderer* renderer, SFrameBuffer* frameBuffer) {
    SColor c;
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++) {
            frameBuffer->readBuffer(x, y, c);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(renderer, x, HEIGHT - y - 1);
        }
    }
}


int main(int argc, char* argv[])
{
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

    SFrameBuffer* frameBuffer = new SFrameBuffer(WIDTH, HEIGHT);
    SPipeline::getInstance().bindFrameBuffer(frameBuffer);

    Scene scene((float)WIDTH / (float)HEIGHT);
    //scene.addModelObject("res/nanosuit/nanosuit.obj");
    //scene.addModelObject("res/Crate/Crate1.obj");
    //scene.addModelObject("res/spot/spot_triangulated.obj");
    scene.addModelObject("res/sakura/sakura.obj");

    SDL_Event ev;

    bool mousePressed = false;

    int fps = 0;
    int frameCount = 0;
    Uint32 start = SDL_GetTicks();

    while (isRunning) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                isRunning = false;
            }
            else if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                case SDLK_j:
                    SPipeline::getInstance().setRendererMode(RendererMode::Line);
                    break;
                case SDLK_k:
                    SPipeline::getInstance().setRendererMode(RendererMode::Triangle);
                    break;
                case SDLK_u:
                    SPipeline::getInstance().switchBackCulling();
                    break;
                case SDLK_i:
                    SPipeline::getInstance().switchClipTriangle();
                    break;
                case SDLK_w:
                    scene.moveCamera(MoveDir::Forward);
                    break;
                case SDLK_s:
                    scene.moveCamera(MoveDir::Back);
                    break;
                case SDLK_a:
                    scene.moveCamera(MoveDir::Left);
                    break;
                case SDLK_d:
                    scene.moveCamera(MoveDir::Right);
                    break;
                case SDLK_UP:
                    scene.moveCamera(MoveDir::Top);
                    break;
                case SDLK_DOWN:
                    scene.moveCamera(MoveDir::Bottom);
                    break;
                case SDLK_SPACE:
                    scene.resetCamera();
                    break;
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                mousePressed = true;
                int px = ev.button.x;
                int py = ev.button.y;
                scene.updateFront(px, py, true);
            }
            else if (ev.type == SDL_MOUSEBUTTONUP) {
                mousePressed = false;
            }
            else if (ev.type == SDL_MOUSEMOTION) {
                int px = ev.motion.x;
                int py = ev.motion.y;
                if(mousePressed) {
                    scene.updateFront(px, py, false);
                }
            }
            else if (ev.type == SDL_MOUSEWHEEL) {
                scene.updateFov(ev.wheel.preciseY);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        frameBuffer->clear();

        scene.onRender();

        onDraw(renderer, frameBuffer);

        frameCount++;
        if (SDL_GetTicks() - start > 1000) {
            printf("%d\n", frameCount);
            frameCount = 0;
            start = SDL_GetTicks();
        }

        // 交换缓冲区
        SDL_RenderPresent(renderer);
    }

    delete frameBuffer;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

