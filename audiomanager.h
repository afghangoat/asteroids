//? A hanglejátszáshoz fontos adatstruktúra, függvények, betöltések vannak itt.

#ifndef AUDIOMAN_H
#define AUDIOMAN_H
#include "debugmalloc.h"
#include <stdio.h>
#include <SDL2/SDL.h>

//- Az audió kezelő, eltárolja az audió lejátszó azonosítót.
typedef struct AudioManager{
    SDL_AudioDeviceID audio_device;
} AudioManager;

//!Inicializálja a paraméterként kapott hanglejátszót. Negatív visszatérési érték jelzi a hibát.
int init_audio(AudioManager* aman);

//!Lejátszik egy hangot a paraméterként kapott hanglejátszóval. a file_path az a .wav hang file útvonala.
int play_sound(AudioManager* aman,const char* file_path);

//!Felszabadítja a paraméterként kapott hanglejátszót.
void cleanup_audio(AudioManager* aman);

#endif