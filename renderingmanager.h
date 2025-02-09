//? A kirajzolásért felelős függvények és struktúrák:
#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include <SDL2/SDL.h>
#include "./extramath.h"
#include <stdio.h>

//- A rajzoló kontextus struktúrája, tartalmazza az egér pozícióját csak olvasásra, A játék ablakát és az SDL renderer-t.
typedef struct RenderingContext{
    SDL_Window* ablak;
    SDL_Renderer* renderer;
    Vector2D mouse_pos;
} RenderingContext;

//!Betölt egy textúrát a kapott paraméteren lévő helyről (.bmp) és visszaadja azt.
SDL_Texture* load_sprite(RenderingContext* context,char path[]);

//! Inicializálja az ablakot. Hiba esetén hamisat ad vissza, másképp igazat.
bool ablak_init(RenderingContext* rendering_context);

//! Inicializálja az SDL renderert. Hiba esetén hamisat ad vissza, másképp igazat.
bool renderer_init(RenderingContext* rendering_context);

//! Felszabadítja a rajzoló kontextust és elemeit.
void cleanup_context(RenderingContext* context);
#endif