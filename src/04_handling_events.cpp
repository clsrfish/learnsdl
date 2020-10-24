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
#include "./utils/log.hpp"
#include "./res_path.hpp"
#include "./utils/sdl_utility.hpp"

namespace handling_events
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

        LOG_I("Resource path is %s", GetResourcePath().c_str());
        // create window
        SDL_Window *win = SDL_CreateWindow("Lesson 4: Handling Events", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
        SDL_Texture *background = LoadSDLTexture(resPath + "background.png", renderer);
        SDL_Texture *image = LoadSDLTexture(resPath + "image.png", renderer);
        if (background == nullptr || image == nullptr)
        {
            cleanup(renderer, win);
            SDL_Quit();
            return -1;
        }
        int iw, ih, ix, iy;
        SDL_QueryTexture(image, nullptr, nullptr, &iw, &ih);
        ix = SCREEN_WIDTH / 2 - iw / 2;
        iy = SCREEN_HEIGHT / 2 - ih / 2;

        int xTiles = SCREEN_WIDTH / TILE_SIZE;
        int yTiles = SCREEN_HEIGHT / TILE_SIZE;

        // rendering
        bool quit = false;
        SDL_Event event;
        while (!quit)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                {
                    LOG_I("Quitting");
                    quit = true;
                }
            }
            SDL_RenderClear(renderer);

            for (int i = 0; i < xTiles * yTiles; i++)
            {
                int x = i % xTiles;
                int y = i / xTiles;
                RenderSDLTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }

            RenderSDLTexture(image, renderer, ix, iy);

            SDL_RenderPresent(renderer);
            SDL_Delay(SDL_DRAW_DELAY);
        }
        // cleanup
        cleanup(background, renderer, win);
        IMG_Quit();
        SDL_Quit();

        return 0;
    }
} // namespace handling_events