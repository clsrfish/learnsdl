//
//  04_handling_events.cpp
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
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "./utils/log.hpp"
#include "./res_path.hpp"
#include "./utils/sdl_utility.hpp"

// https://www.willusher.io/sdl2%20tutorials/2013/12/18/lesson-6-true-type-fonts-with-sdl_ttf
namespace render_text
{
    int main()
    {
        LOG_I(__FILENAME__);

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            LOG_E("SDL_Init error: %s", SDL_GetError());
            return -1;
        }
        // init SDL_image optional
        if (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG != IMG_INIT_PNG)
        {
            LOG_E("IMG_Init error: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }
        // init SDL_ttf
        if (TTF_Init() != 0)
        {
            LOG_E("TTF_Init error: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }
        LOG_I("Resource path is %s", GetResourcePath().c_str());
        // create window
        SDL_Window *win = SDL_CreateWindow("Lesson 6: True Type Fonts with SDL_ttf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
        SDL_Color color = {255, 255, 255, 255};
        SDL_Texture *image = RenderText("TTF fonts are cool!", resPath + "sample.ttf", color, 64, renderer);
        if (image == nullptr)
        {
            cleanup(renderer, win);
            SDL_Quit();
            return -1;
        }
        int iW, iH;
        SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
        int x = SCREEN_WIDTH / 2 - iW / 2;
        int y = SCREEN_HEIGHT / 2 - iH / 2;

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
                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                    }
                }
            }
            SDL_RenderClear(renderer);

            RenderSDLTexture(image, renderer, x, y);

            SDL_RenderPresent(renderer);
            SDL_Delay(SDL_DRAW_DELAY);
        }
        // cleanup
        cleanup(image, renderer, win);
        IMG_Quit();
        SDL_Quit();

        return 0;
    }
} // namespace render_text