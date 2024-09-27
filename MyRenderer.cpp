#include <iostream>
#include <SDL.h>
#include <vld.h>
#include "core/Texture.h"
#include "core/FrameBuffer.h"
#include "core/Pipeline.h"
#include "entity/Scene.h"
#include <SDL_ttf.h>

int WIDTH = 800;
int HEIGHT = 600;

void onDraw(SDL_Renderer* renderer, FrameBuffer* frameBuffer) {
    Color c;
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

    if (TTF_Init() == -1) {
        return 1;
    }
    bool isRunning = true;

    FrameBuffer* frameBuffer = new FrameBuffer(WIDTH, HEIGHT);
    Pipeline::getInstance().bindFrameBuffer(frameBuffer);

    Scene scene((float)WIDTH / (float)HEIGHT, renderer);
    //scene.addModelObject("res/Knicker/Air_Gun-Wavefront OBJ.obj", 5);
    scene.addModelObject("res/nanosuit/nanosuit.obj", 1, 2);
    //scene.addModelObject("res/cyborg/cyborg.obj", 2, 1);
    //scene.addModelObject("res/Crate/Crate1.obj", 2);
    //scene.addModelObject("res/25obj/KSR-29 sniper rifle new_obj.obj");
    //scene.addModelObject("res/E-45-Aircraft/E 45 Aircraft_obj.obj", 1, 1);
    //scene.addModelObject("res/sakura/Sakura.obj");
    //scene.addModelObject("res/sakura/Sakura2.obj");
    //scene.addModelObject("res/rock/rock.obj");
    //scene.addQuat("res/spot/spot_texture.png");
    /*{
        scene.addModelObject("res/african_head/african_head.obj", 2);
        scene.addModelObject("res/african_head/african_head_eye_inner.obj", 2);
        scene.addModelObject("res/african_head/african_head_eye_outer.obj", 2);
    }*/

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
                    Pipeline::getInstance().setRendererMode(RendererMode::Line);
                    break;
                case SDLK_k:
                    Pipeline::getInstance().setRendererMode(RendererMode::Triangle);
                    break;
                case SDLK_l:
                    Pipeline::getInstance().setRendererMode(RendererMode::Point);
                    break;
                case SDLK_u:
                    Pipeline::getInstance().switchBackCulling();
                    break;
                case SDLK_i:
                    Pipeline::getInstance().switchClipTriangle();
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
                case SDLK_LEFT:
                    scene.leftLight();
                    break;
                case SDLK_RIGHT:
                    scene.rightLight();
                    break;
                case SDLK_SPACE:
                    scene.resetCamera();
                    break;
                case SDLK_KP_1:
                    scene.setObjectRotate(Vec3f(0, 1, 0), -5);
                    break;
                case SDLK_KP_3:
                    scene.setObjectRotate(Vec3f(0, 1, 0), 5);
                    break;
                case SDLK_KP_5:
                    scene.setObjectRotate(Vec3f(1, 0, 0), 5);
                    break;
                case SDLK_KP_2:
                    scene.setObjectRotate(Vec3f(1, 0, 0), -5);
                    break;
                case SDLK_KP_4:
                    scene.setObjectRotate(Vec3f(0, 0, 1), 5);
                    break;
                case SDLK_KP_6:
                    scene.setObjectRotate(Vec3f(0, 0, 1), -5);
                    break;
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                mousePressed = true;
                int px = ev.button.x;
                int py = ev.button.y;
                scene.updateFront(px, py, true);
                scene.checkClickBtn(px, py, true);
            }
            else if (ev.type == SDL_MOUSEBUTTONUP) {
                mousePressed = false;
                scene.checkClickBtn(0, 0, false);
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
        scene.onRenderUI();

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
    TTF_Quit();

    return 0;
}

