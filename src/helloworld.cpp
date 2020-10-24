//
//  helloworld.cpp
//
//  Created by Clsrfish on 24/10/2020
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include "./utils/log.hpp"
#include "./res_path.hpp"

// https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
namespace helloworld
{
    int main()
    {
        LOG_I(__FILENAME__);

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            LOG_E("SDL_Init error: %s", SDL_GetError());
            return -1;
        }
        LOG_I("Resource path is %s", GetResourcePath().c_str());
        // create window
        SDL_Window *win = SDL_CreateWindow(__FILENAME__, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
        if (win == nullptr)
        {
            LOG_E("Failed to create window: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }

        // create renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr)
        {
            SDL_DestroyWindow(win);
            LOG_E("SDL create renderer failed: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }

        // load bitmap
        std::string imagePath = GetResourcePath() + "hello.bmp";
        SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
        if (bmp == nullptr)
        {
            LOG_E("Failed to load bmp: %s", SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(win);
            SDL_Quit();
            return -1;
        }
        // upload data to renderer
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
        SDL_FreeSurface(bmp);
        if (tex == nullptr)
        {
            LOG_E("Failed to upload bmp: %s", SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(win);
            SDL_Quit();
            return -1;
        }

        // rendering
        bool quit = false;
        SDL_Event event;
        while (!quit)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    LOG_I("Quitting");
                    quit = true;
                }
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, tex, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
        }
        // cleanup

        SDL_DestroyTexture(tex);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        SDL_Quit();

        return 0;
    }
} // namespace helloworld