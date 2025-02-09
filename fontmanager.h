//? A betűtípus, betűk kirajzolása, betöltése, felszabadítása és kirajzolásáért feleős függvények vannak itt.
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "./extramath.h"
#include "./renderingmanager.h"
#include "debugmalloc.h"

//- A betűfelület struktúrája, egy betűfelület tartalmazza a szöveget, amit majd ki kell rajzolni és egy helyet, ami mutatja,hogy hova kell a szöveget kirajzolni.
typedef struct FontSurface{
    Vector2D pos;
    char data[];
} FontSurface;

//- A betűtípuskezelő tartalmazza a betűtípushoz tartozó textúraatlaszt.
typedef struct FontManager{
    SDL_Texture* cur_font;
} FontManager;

//! Betölt egy paraméterként kapott betűtípus-atlaszt a szintén paraméterként kapott betűtípuskezelőbe.
void load_font_manager(FontManager* font_manager,RenderingContext* context, char path[]);

//! Kirajzol egy betűt egy megadott betűtípussal. Az x és y pozícióját lehet tolni a paraméterekkel.
void draw_letter(RenderingContext* context,FontSurface* written_surface,SDL_Texture* font_texture,int offset_x,int global_offset,int y_adder);

//! Kirajzol egy betűfelületet egy paraméterként megadott betűtípussal.
void draw_letters(RenderingContext* context,FontManager* font_manager,FontSurface* written_surface);
#endif