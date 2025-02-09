
#include "player.h"

void init_player(Player* player,RenderingContext* current_context){
    init_audio(&player->player_sounds);
    player->sprite=load_sprite(current_context,"assets/urhajo.bmp");
}
void cleanup_player(Player* player){
    cleanup_audio(&player->player_sounds);
}
void spawn_player(Player* player){
    player->is_dead=false;
    player->pos.x = JATEKTER_HOSSZ / 2.0;
    player->pos.y = JATEKTER_MAGASSAG / 2.0;
    player->shooting=false;
    player->moving = false;
    player->cooldown=0.0;
    player->iframes=PLAYER_IFRAMES;
}
void setup_player(Player* player) {
    spawn_player(player);
    player->player_radius=16.0;
    player->lives=3;
    player->bonus_score=0;
}
void try_respawn_player(Player* player,GameState* game_state){
    play_sound(&player->player_sounds,"assets/sfx/ship_spawn.wav");
    player->lives--;
    if(player->lives>0){
        spawn_player(player);
    } else {
        game_state->state=4; //dead
    }
}

//Ez megnézi, hogy az űrhajó tud-e hiperugrani
void check_hyperspace(Player* player){
	if(player->pos.x<-32){
		player->pos.x+=JATEKTER_HOSSZ+31;
	} else if(player->pos.x>JATEKTER_HOSSZ+32){
		player->pos.x-=JATEKTER_HOSSZ+31;
	}
	
	if(player->pos.y<-32){
		player->pos.y+=JATEKTER_MAGASSAG+31;
	} else if(player->pos.y>JATEKTER_MAGASSAG+32){
		player->pos.y-=JATEKTER_MAGASSAG+31;
	}
}
void update_player(Player* player,GameState* gamestate) {
    if (player->moving == false) {
        player->accel *= 0.8;
        player->velocity *= 0.8;
    }
    player->velocity += player->accel;
    player->velocity = clampd(player->velocity, -205.0, 302.0);
    player->pos.x += cos(player->angle*RAD) * player->velocity*gamestate->delta_time;
    player->pos.y += sin(player->angle*RAD) * player->velocity*gamestate->delta_time;
    if(player->cooldown>0.0){
        player->cooldown-=gamestate->delta_time;
    }
	check_hyperspace(player);
	
}

void render_player(Player* player,RenderingContext* context) {
    
    SDL_Rect player_texturesize = { (int)player->pos.x, (int)player->pos.y, 64, 32 };
    SDL_RenderCopyEx(context->renderer,player->sprite,NULL,&player_texturesize,player->angle,NULL,SDL_FLIP_NONE);
    //SDL_SetRenderDrawColor(context->renderer, 0, 255, 255, 255);
    //SDL_RenderFillRect(context->renderer, &t1);
}
void process_input(Player* player,GameState* game_state) {
    SDL_Event event;
    SDL_PollEvent(&event);
    player->moving = false;
    player->shooting=false;
    if (event.type == SDL_QUIT) {
        game_state->state = 0;
        return;
    }
    if(event.type==SDL_MOUSEMOTION){
        SDL_GetMouseState(&game_state->mouseposx,&game_state->mouseposy);
        return;
    }
    if(event.type==SDL_MOUSEBUTTONDOWN){
        game_state->mousedown=true;
        return;
    }
    if(event.type==SDL_MOUSEBUTTONUP){
        game_state->mousedown=false;
    }
    const Uint8* keypresses = SDL_GetKeyboardState(NULL);
    if(keypresses[SDL_SCANCODE_ESCAPE]){
        game_state->state = 0;
    }
    if(keypresses[SDL_SCANCODE_W]){
        if (player->accel < 200.0){
            player->accel += 70.0;
        }
        player->moving = true;
    }
    if(keypresses[SDL_SCANCODE_S]){
        if (player->accel > -100.0){
            player->accel -= 20.0;
        }
        player->moving = true;
    }
    if(keypresses[SDL_SCANCODE_A]){
        player->angle -= 6.0;
    }
    if(keypresses[SDL_SCANCODE_D]){
        player->angle += 6.0;
    }
    if(keypresses[SDL_SCANCODE_SPACE]){
        if(player->cooldown<=0.0){
            player->shooting=true;

        }
    }
}
