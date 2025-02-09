#include <SDL2/SDL.h>
#include <stdio.h>
#include "./asteroidmanager.h"
#include "./extramath.h"
#include "./player.h"
#include "./scoremanager.h"
#include "./renderingmanager.h"
#include "./gamemanager.h"
#include "./bulletmanager.h"
#include "./audiomanager.h"

//? A fő forrásfájl, ebbe van a game loop, a felszabadítás és a fő inicializáló függvények.
/*
Saját modulok:

konstansok.h
extramath.h/c
bulletmanager.h/c
fontmanager.h/c
player.h/c
menumanager.h/c
gamemanager.h/c
asteroidmanager.h/c
scoremanager.h/c
audiomanager.h/c
renderingmanager.h/c

Fordítása:
build.bat
*/

/*TODO (Mi hiányzik):
+ kommentek
*/

//! A renderert és az ablakot inicializálja a függvény;
int init_context(RenderingContext* context){
	if (ablak_init(context) == false) {
        return 1;
    }
    if (renderer_init(context) == false) {
        return 1;
    }
	return 0;
}

//! A kattintható és/vagy szöveges elemeket inicializálja a függvény
void init_gui(RenderingContext* context,FontManager* font_manager, ScoreManager* score_manager, HighScoreManager* h_score_man, TopListScreen* top_list_gui, DeathScreen* death_screen, MainMenu* main_menu){
	load_font_manager(font_manager,context,"assets/font_atlas.bmp");
	
	setup_score_manager(score_manager);
	load_highscores(h_score_man);
	
	init_top_list_screen(top_list_gui,context);
	
	init_death_screen(death_screen,context);
	
	main_menu_init(main_menu,context);
}

//! A fő függvény. Ebbe van meghívva az összes inicializálás, a game loop és a felszabadítás a végén.
int main(int argc, char* argv[]) {
	
	//Renderer és ablak inicialiálása
    RenderingContext current_context;
    if (init_context(&current_context)==1){
		printf("Hiba a játék inicializálása közben!");
		return 0;
	}

    FontManager font_manager;
    ScoreManager score_manager;
    HighScoreManager high_score_manager;
    TopListScreen top_screen;
    DeathScreen death_screen;
    MainMenu main_menu;
	//A menük inicializálása:
	init_gui(&current_context,&font_manager,&score_manager,&high_score_manager,&top_screen,&death_screen,&main_menu);
    
	//A játék elemeinek inicializálása:
	GameState game_state;
    setup_game(&game_state);
    BulletManager bullet_manager;
    setup_bullet_manager(&bullet_manager,&current_context);
    AsteroidSpawner asteroid_man;
    setup_asteroid_spawner(&asteroid_man,&current_context,game_state.difficulty);
    Player player;
    init_player(&player,&current_context);
	int saved_done=0;
	
    SDL_Delay(1000);
	play_music(&main_menu);
	
    while (game_state.state != QUIT) {

        process_input(&player,&game_state);
        update(&game_state);
		
        SDL_SetRenderDrawColor(current_context.renderer, 0, 0, 0, 255);
        SDL_RenderClear(current_context.renderer);
		
		render_background(&asteroid_man,&current_context);
        switch (game_state.state){
            case INGAME:
                try_spawn_asteroid(&asteroid_man,&game_state);
                update_player(&player,&game_state);
                update_bullet_hits(&bullet_manager,&asteroid_man,&score_manager,&player);
                update_bullets(&bullet_manager,&game_state);
                update_asteroids(&asteroid_man,&game_state);
				update_explosions(&asteroid_man,&game_state);
                if(player.iframes>0.0){
                    player.iframes-=game_state.delta_time;
                } else {
                    check_player_hit(&player,&asteroid_man);
                }
				
                update_score_manager(&score_manager,&game_state);
                if(player.shooting==true){
                    play_sound(&player.player_sounds,"assets/sfx/shoot.wav");
                    shoot_bullet(&bullet_manager,&player);
                }

                if(player.is_dead==false){
                    render_player(&player,&current_context);
                } else {
                    try_respawn_player(&player,&game_state);
                    if(game_state.state==4){
						clear_asteroids(&asteroid_man);
                        setup_stats(&death_screen,&score_manager,&high_score_manager);
						
                    }
                    
                }
                render_bullets(&bullet_manager,&current_context);
                render_asteroids(&asteroid_man,&current_context);
				render_explosions(&asteroid_man,&current_context);
                draw_score_manager(&score_manager,&current_context,&font_manager,&player);
                break;
            case INMENU:
                check_main_menu_inputs(&main_menu,&game_state);
                draw_main_menu(&main_menu,&current_context,&font_manager);
				if (game_state.state!=INMENU){
					//Nehézség frissítése
					change_diff(&asteroid_man,game_state.difficulty);
				}
                break;
            case RESET_SCORE:
                reset_score_manager(&score_manager);
                setup_player(&player);
                game_state.state=1;
                break;
            case DISPLAY_DEATH:
                update_death_button(&death_screen,&game_state,&current_context,&font_manager);
                
                if(death_screen.done==false){
                    saved_done=register_name(death_screen.name_label->data);
					if(saved_done==1){
                        game_state.state=7;
                    }
                }
                break;
            case LOAD_TOPLIST:
                game_state.state=6;
                load_top_list_screen(&top_screen,&high_score_manager);
                break;
            case DISPLAY_TOPLIST:
                draw_top_list_screen(&top_screen,&game_state,&current_context,&font_manager);
				
                break;
			case SAVE_STATS:
				if(death_screen.ontoplist==true){
                    death_screen.done=true;
                    save_score(&high_score_manager,death_screen.name_label->data,&score_manager,&game_state);
					cleanup_death_screen(&death_screen);
					saved_done=0;
                }
				game_state.state=2;
				break;
            default:
                break;
        }
        SDL_RenderPresent(current_context.renderer);
    }

	cleanup_death_screen(&death_screen);
	cleanup_bullets(&bullet_manager);
    cleanup_scores(&high_score_manager,&score_manager);
    cleanup_menus(&main_menu,&top_screen);
    cleanup_asteroid_manager(&asteroid_man);
    cleanup_player(&player);
    cleanup_context(&current_context);

    return 0;
}

/*
Asteroids **

A játékos egy űrhajót irányít, és a felé repülő aszteroidákat kell szétlőnie. Természetesen a nagyobb aszteroidák nem csak simán eltűnnek, hanem kisebb darabokra esnek szét; közben vigyázni kell, nehogy az űrhajó bármelyiknek nekiütközzön.

A programban legyenek nehézségi szintek (pl. az aszteroidák gyakorisága alapján), és legyen dicsőséglista a legeredményesebb játékosok pontjaival!
*/