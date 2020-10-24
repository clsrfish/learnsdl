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

namespace sprite_sheet
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
        SDL_Window *win = SDL_CreateWindow("Lesson 5: Clipping Sprite Sheets", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
        SDL_Texture *image = LoadSDLTexture(resPath + "sprite.png", renderer);
        if (background == nullptr || image == nullptr)
        {
            cleanup(renderer, win);
            SDL_Quit();
            return -1;
        }

        int iw = 100, ih = 100;
        int x = SCREEN_WIDTH / 2 - iw / 2;
        int y = SCREEN_HEIGHT / 2 - ih / 2;

        //Setup the clips for our image
        SDL_Rect clips[4];
        for (int i = 0; i < 4; ++i)
        {
            clips[i].x = i / 2 * iw;
            clips[i].y = i % 2 * ih;
            clips[i].w = iw;
            clips[i].h = ih;
        }
        //Specify a default clip to start with
        int useClip = 0;

        int xTiles = SCREEN_WIDTH / TILE_SIZE;
        int yTiles = SCREEN_HEIGHT / TILE_SIZE;

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
                    case SDLK_1:
                        useClip = 0;
                        break;
                    case SDLK_2:
                        useClip = 1;
                        break;
                    case SDLK_3:
                        useClip = 2;
                        break;
                    case SDLK_4:
                        useClip = 3;
                        break;
                    default:
                        break;
                    }
                }
            }
            SDL_RenderClear(renderer);

            for (int i = 0; i < xTiles * yTiles; i++)
            {
                int x = i % xTiles;
                int y = i / xTiles;
                RenderSDLTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }

            RenderSDLTexture(image, renderer, x, y, &clips[useClip]);

            SDL_RenderPresent(renderer);
            SDL_Delay(SDL_DRAW_DELAY);
        }
        // cleanup
        cleanup(background, renderer, win);
        IMG_Quit();
        SDL_Quit();

        return 0;
    }
} // namespace sprite_sheet