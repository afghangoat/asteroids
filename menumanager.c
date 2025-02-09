
#include "menumanager.h"


void check_button(Button* btn,GameState* game_state){
    if(btn->pos.x+btn->size.x+OUTLINE_SIZE*4>game_state->mouseposx&&btn->pos.x-OUTLINE_SIZE*2<game_state->mouseposx && btn->pos.y+btn->size.y+OUTLINE_SIZE*4>game_state->mouseposy&&btn->pos.y-OUTLINE_SIZE*2<game_state->mouseposy){
        if(game_state->mousedown==true){
            btn->isclicked=true;
            game_state->mousedown=false;
            play_sound(&btn->btn_click,"assets/sfx/uiclick.wav");
            switch(btn->func_type){
                case 0:
                    game_state->state=3;
                    break;
                case 1:
                    game_state->state=0;
                    break;
                case 2:
                    game_state->state=5;
                    break;
                case 3:
                    switch_difficulty(game_state);
                    const char* difficulty_label = get_difficulty_label(game_state->difficulty);
                    char nehezseg_label[]="nehe'zseg: ";
                    strcat(nehezseg_label, difficulty_label);
                    strcpy(btn->fontSurface->data, nehezseg_label);
                    break;
                case 6:
                    game_state->state=2;
                    break;
				case 7:
					game_state->state=7;
                default:
                    break;
            }
        }
        btn->inmouse=true;
    } else {
        btn->inmouse=false;
    }
}
void setup_button(Button** btn, Vector2D posi, Vector2D sizei, int func, Vector2D font_start, char datai[]) {
    *btn = (Button*) malloc(sizeof(Button));
    if (*btn == NULL) {
        printf("A gombnak nem sikerült memóriát foglalni!\n");
        return;
    }
    
    (*btn)->pos = posi;
    (*btn)->size = sizei;
    (*btn)->func_type = func;
    (*btn)->isclicked=false;
    (*btn)->inmouse=false;
    init_audio(&(*btn)->btn_click);

    size_t data_len = strlen(datai) + 30+1;
    (*btn)->fontSurface = (FontSurface*) malloc(sizeof(FontSurface) + data_len);
    if ((*btn)->fontSurface == NULL) {
        printf("A gomb betűjének nem sikerült memóriát foglalni\n");
        return;
    }

    (*btn)->fontSurface->pos = font_start;

    strcpy((*btn)->fontSurface->data, datai);
}
void draw_button(Button* btn,RenderingContext* context,FontManager* font_manager){
    if(btn->inmouse==true){
        SDL_Rect t1;
        t1.x = btn->pos.x-OUTLINE_SIZE;
        t1.y = btn->pos.y-OUTLINE_SIZE;
        t1.w = btn->size.x+OUTLINE_SIZE*2;
        t1.h = btn->size.y+OUTLINE_SIZE*2;
        SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(context->renderer, &t1);
    }
    draw_letters(context,font_manager,btn->fontSurface);
}
void main_menu_init(MainMenu* main_menu, RenderingContext* context) {
    main_menu->main_title = load_sprite(context, "assets/title.bmp");
    main_menu->title_dst = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    if (main_menu->title_dst == NULL) {
        printf("A címnek nem sikerült memóriát foglalni\n");
        return;
    }

    main_menu->title_dst->x = 200;
    main_menu->title_dst->y = 100;
    main_menu->title_dst->w = 256;
    main_menu->title_dst->h = 37;
	
	main_menu->main_icon = load_sprite(context, "assets/asteroid_icon.bmp");
	main_menu->icon_angle=0.0;

    setup_button(&main_menu->start_btn, init_vec2(300, 200), init_vec2(192, 16), 0, init_vec2(300, 200), "start");
    setup_button(&main_menu->quit_btn, init_vec2(300, 300), init_vec2(192, 16), 1, init_vec2(300, 300), "quit");
    setup_button(&main_menu->top_list_btn, init_vec2(300, 400), init_vec2(192, 16), 2, init_vec2(300, 400), "top 10");
    setup_button(&main_menu->difficulty_btn, init_vec2(300, 500), init_vec2(320, 16), 3, init_vec2(300, 500), "nehe'zseg: alap");
	
	init_audio(&main_menu->music_player);
}
void play_music(MainMenu* main_menu){
	play_sound(&main_menu->music_player,"assets/music/stellar_miner.wav");
}
void check_main_menu_inputs(MainMenu* main_menu,GameState* game_state){
    check_button(main_menu->start_btn,game_state);
    check_button(main_menu->quit_btn,game_state);
    check_button(main_menu->top_list_btn,game_state);
    check_button(main_menu->difficulty_btn,game_state);
}
void draw_main_menu(MainMenu* main_menu,RenderingContext* context,FontManager* font_manager){

    SDL_RenderCopy(context->renderer, main_menu->main_title, NULL, main_menu->title_dst); //Kirajzolja a fő címet
    draw_button(main_menu->start_btn,context,font_manager);
    draw_button(main_menu->quit_btn,context,font_manager);
    draw_button(main_menu->top_list_btn,context,font_manager);
    draw_button(main_menu->difficulty_btn,context,font_manager);
	
	SDL_Rect a_icon_dst = { 450, 200, 200, 200 };
    SDL_RenderCopyEx(context->renderer,main_menu->main_icon,NULL,&a_icon_dst,main_menu->icon_angle,NULL,SDL_FLIP_NONE);
	
	main_menu->icon_angle-=0.05;
}
void cleanup_button(Button* btn) {
    if (btn != NULL) {
        if (btn->fontSurface != NULL) {
            free(btn->fontSurface);
            btn->fontSurface = NULL;
        }

        cleanup_audio(&btn->btn_click);
		free(btn);
		btn=NULL;
    }
}
void cleanup_menus(MainMenu* mainmenu,TopListScreen* top_screen){
	cleanup_button(mainmenu->start_btn);
    cleanup_button(mainmenu->quit_btn);
    cleanup_button(mainmenu->top_list_btn);
    cleanup_button(mainmenu->difficulty_btn);
	cleanup_audio(&mainmenu->music_player);

    if (mainmenu->title_dst != NULL) {
        free(mainmenu->title_dst);
        mainmenu->title_dst = NULL;
    }
	
    if (top_screen != NULL) {
        free(top_screen->self_label);
        top_screen->self_label=NULL;
        free(top_screen->top_string);
        top_screen->top_string=NULL;
	    cleanup_button(top_screen->back_btn);
    }
}
void init_top_list_screen(TopListScreen* top_list_screen, RenderingContext* context){

    top_list_screen->top_string=NULL;

    setup_button(&top_list_screen->back_btn, init_vec2(32, 400), init_vec2(300, 20), 6, init_vec2(32, 400), "vissza");
    top_list_screen->self_label = (FontSurface*)malloc(sizeof(FontSurface) + 50);

    if (top_list_screen->self_label == NULL) {
        printf("A top lista kiírásnak nem sikerült memóriát foglalni\n");
        return;
    }
    top_list_screen->self_label->pos = init_vec2(40, 40);

    char* val="A toplista:";
    strcpy(top_list_screen->self_label->data, val);
}

void draw_top_list_screen(TopListScreen* top_list_screen,GameState* game_state,RenderingContext* context,FontManager* font_manager){
    draw_letters(context,font_manager,top_list_screen->self_label);
    draw_letters(context,font_manager,top_list_screen->top_string);

    check_button(top_list_screen->back_btn,game_state);
    draw_button(top_list_screen->back_btn,context,font_manager);
}

void init_death_screen(DeathScreen* death_screen,RenderingContext* context){
	
	death_screen->freed=false;
	death_screen->self_label=NULL;
	death_screen->name_label=NULL;
	
    death_screen->done=false;
    death_screen->toplist_label=load_sprite(context,"assets/name_label.bmp");
    setup_button(&death_screen->back_btn, init_vec2(32, 200), init_vec2(300, 20), 7, init_vec2(32, 200), "mente's e's vissza");
}
void update_death_button(DeathScreen* death_screen,GameState* game_state,RenderingContext* context,FontManager* font_manager){
    draw_letters(context,font_manager,death_screen->self_label);
    draw_letters(context,font_manager,death_screen->name_label);
    if(death_screen->ontoplist==true){
        SDL_Rect toplabel_texturesize = { 32, 100, 362, 30 };
        SDL_RenderCopy(context->renderer, death_screen->toplist_label, NULL, &toplabel_texturesize);
        //SDL_RenderCopyEx(context->renderer,death_screen->toplist_label,NULL,&toplabel_texturesize,0,NULL,SDL_FLIP_NONE);

    }
    check_button(death_screen->back_btn,game_state);
    draw_button(death_screen->back_btn,context,font_manager);
}
