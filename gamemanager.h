//? A játékállapot struktúráját, függvényeit, az állást tároló struktúrát tartalmazó header file.
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "./extramath.h"
#include <SDL2/SDL.h>

//- A játékállapot helyzet megértésére ad lehetőséget
typedef enum State {
	QUIT =0,
	INGAME = 1,
	INMENU = 2,
	RESET_SCORE = 3,
	DISPLAY_DEATH = 4,
	LOAD_TOPLIST=5,
	DISPLAY_TOPLIST=6,
	SAVE_STATS=7
} State;

//- A játékállást tárolja el, tárolja a delta időt 2 frame között, az egér x és y pozícióját, hogy kattintott-e a játékos, a nehézséget és a játékállapotot.
typedef struct GameState{
    State state;
    int last_frame_time;
    double delta_time;
    int mouseposx;
    int mouseposy;
    bool mousedown;
    int difficulty;
} GameState;

//! Megváltoztatja a nehézséget a paraméterként kapott játékállapotban.
void switch_difficulty(GameState* game_state);

//! Visszaad egy szöveget, ami jellemzi a nehézséget.
const char* get_difficulty_label(int difficulty);

//! Inicializálja a játékállást alapértékekkel.
void setup_game(GameState* gamestate);

//! Frissíti a játékállást. Újra kiszámolja a delta időt.
void update(GameState* gamestate);
#endif