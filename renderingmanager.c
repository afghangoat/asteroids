
#include "renderingmanager.h"


SDL_Texture* load_sprite(RenderingContext* context,char path[]){
    SDL_Surface* surf=SDL_LoadBMP(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(context->renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}
bool ablak_init(RenderingContext* rendering_context) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Az SDL nem tudott inicializálódni. Hiba kód: %s\n", SDL_GetError());
        return false;
    }
    rendering_context->ablak = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, JATEKTER_HOSSZ, JATEKTER_MAGASSAG, SDL_WINDOW_SHOWN);
    if (!rendering_context->ablak) {
        printf("A program nem tudott ablakot létrehozni. Hiba kód: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    return true;
}

bool renderer_init(RenderingContext* rendering_context) {
    rendering_context->renderer = SDL_CreateRenderer(rendering_context->ablak, -1, SDL_RENDERER_ACCELERATED);
    if (!rendering_context->renderer) {
        printf("Az SDL Renderer létrehozása sikertelen volt! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(rendering_context->ablak);
        SDL_Quit();
        return false;
    }
    return true;
}

void cleanup_context(RenderingContext* context) {
    SDL_DestroyRenderer(context->renderer);
    SDL_DestroyWindow(context->ablak);
    SDL_Quit();
}
