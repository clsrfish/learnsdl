#include <SDL2_image/SDL_image.h>
#include "./sdl_utility.hpp"
#include "./log.hpp"

void LogSDLError(const std::string &msg)
{
    LOG_E(msg, SDL_GetError());
}

SDL_Texture *LoadSDLTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *tex = IMG_LoadTexture(ren, file.c_str());
    if (tex == nullptr)
    {
        LogSDLError("Could not load texture");
    }
    return tex;
}

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr)
    {
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else
    {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    RenderSDLTexture(tex, ren, dst, clip);
}

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    RenderSDLTexture(tex, ren, dst);
}

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip)
{
    SDL_RenderCopy(ren, tex, clip, &dst);
}