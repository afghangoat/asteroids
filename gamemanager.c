
#include "gamemanager.h"


void switch_difficulty(GameState* game_state){
    game_state->difficulty=(game_state->difficulty+1)%3;
}
const char* get_difficulty_label(int difficulty){
    switch(difficulty){
        case 0:
            return "alap";
            break;
        case 1:
            return "nehe'z";
            break;
        case 2:
            return "lehetetlen";
            break;
        default:
            return "???";
            break;
    }
}


void setup_game(GameState* gamestate) {
    gamestate->state = 2;
    gamestate->last_frame_time=0;
    gamestate->delta_time=0.0;
    gamestate->mouseposx=-1;
    gamestate->mouseposy=-1;
    gamestate->mousedown=false;
    gamestate->difficulty=0;
}


void update(GameState* gamestate) {
    int delay = FRAME_TIME - (SDL_GetTicks() - gamestate->last_frame_time);
    if (delay > 0 && delay <= FRAME_TIME) {
        SDL_Delay(delay);
    }
    gamestate->delta_time = (SDL_GetTicks() - gamestate->last_frame_time) / 1000.0;
    gamestate->last_frame_time = SDL_GetTicks();
}
