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
#include "./utils/sdl_utility.hpp"

// https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
namespace dont_in_main
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
        SDL_Window *win = SDL_CreateWindow("Lesson 2: Don't Put Everything in Main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
            cleanup(win);
            LOG_E("SDL create renderer failed: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }

        // load texture
        std::string resPath = GetResourcePath();
        SDL_Texture *background = LoadSDLTexture(resPath + "background.bmp", renderer);
        SDL_Texture *image = LoadSDLTexture(resPath + "image.bmp", renderer);
        if (background == nullptr || image == nullptr)
        {
            cleanup(renderer, win);
            SDL_Quit();
            return -1;
        }
        int bw, bh, iw, ih, ix, iy;
        SDL_QueryTexture(background, nullptr, nullptr, &bw, &bh);
        SDL_QueryTexture(image, nullptr, nullptr, &iw, &ih);
        ix = SCREEN_WIDTH / 2 - iw / 2;
        iy = SCREEN_HEIGHT / 2 - ih / 2;

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

            RenderSDLTexture(background, renderer, 0, 0);
            RenderSDLTexture(background, renderer, bw, 0);
            RenderSDLTexture(background, renderer, 0, bh);
            RenderSDLTexture(background, renderer, bw, bh);

            RenderSDLTexture(image, renderer, ix, iy);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        // cleanup
        cleanup(background, renderer, win);
        SDL_Quit();

        return 0;
    }
} // namespace dont_in_main