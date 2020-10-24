#if !defined(SDL_UTILITY_H)
#define SDL_UTILITY_H

#ifdef __cplusplus
#include <string>

extern "C"
{
#endif
#include <SDL2/SDL.h>
#ifdef __cplusplus
}
#endif

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

const int SDL_DRAW_DELAY = 16;

/*
 * Recurse through the list of arguments to clean up, cleaning up
 * the first one in the list each iteration.
 */
template <typename T, typename... Args>
void cleanup(T *t, Args &&... args)
{
    //Cleanup the first item in the list
    cleanup(t);
    //Recurse to clean up the remaining arguments
    cleanup(std::forward<Args>(args)...);
}
/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single item
 * one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion
 * We also make it safe to pass nullptrs to handle situations where we
 * don't want to bother finding out which values failed to load (and thus are null)
 * but rather just want to clean everything up and let cleanup sort it out
 */
template <>
inline void cleanup<SDL_Window>(SDL_Window *win)
{
    if (!win)
    {
        return;
    }
    SDL_DestroyWindow(win);
}
template <>
inline void cleanup<SDL_Renderer>(SDL_Renderer *ren)
{
    if (!ren)
    {
        return;
    }
    SDL_DestroyRenderer(ren);
}
template <>
inline void cleanup<SDL_Texture>(SDL_Texture *tex)
{
    if (!tex)
    {
        return;
    }
    SDL_DestroyTexture(tex);
}
template <>
inline void cleanup<SDL_Surface>(SDL_Surface *surf)
{
    if (!surf)
    {
        return;
    }
    SDL_FreeSurface(surf);
}

void LogSDLError(const std::string &msg);

SDL_Texture *LoadSDLTexture(const std::string &file, SDL_Renderer *ren);

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

#endif // SDL_UTILITY_H
