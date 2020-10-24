#include "./sdl_utility.hpp"
#include "./log.hpp"

void LogSDLError(const std::string &msg)
{
    LOG_E(msg, SDL_GetError());
}

SDL_Texture *LoadSDLTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *tex = nullptr;
    SDL_Surface *bmp = SDL_LoadBMP(file.c_str());
    if (bmp != nullptr)
    {
        tex = SDL_CreateTextureFromSurface(ren, bmp);
        SDL_FreeSurface(bmp);
        if (tex == nullptr)
        {
            LogSDLError("Could not load texture");
        }
    }
    else
    {
        LogSDLError("LoadBMP");
    }
    return tex;
}

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, nullptr, &dst);
}