//? A menük, gombok, struktúráit, függvényeit tartalmazó header file.
#ifndef MENUMANAGER_H
#define MENUMANAGER_H
#include <stdbool.h>
#include "./fontmanager.h"
#include "./renderingmanager.h"
#include "./extramath.h"
#include "./gamemanager.h"
#include "./audiomanager.h"
#include "debugmalloc.h"

//- A gomb struktúrája, tartalmazza a funkcióját, a betűfelületet a gomb szövegével, egy nagyságot, egy pozíciót, egy attribútumot, hogy kattintottak-e rá, egy attribútumot, hogy rajta van-e az egért, és egy hanglejátszót a kattintás esetére.
typedef struct Button{
    Vector2D pos;
    Vector2D size;
    int func_type;
    bool isclicked;
    bool inmouse;
    FontSurface* fontSurface;
    AudioManager btn_click;
} Button;

//- A főmenü struktúrája, tartalmazza a főmenübe lévő textűrákat, egy hanglejátszót a zenének a főmenüben, és pointereket a start,kilépés,toplista,nehézség gombokra.
typedef struct MainMenu{
    Button* start_btn;
    Button* quit_btn;
    Button* top_list_btn;
    Button* difficulty_btn;
	double icon_angle;
    SDL_Texture* main_title;
	SDL_Texture* main_icon;
    SDL_Rect* title_dst;
	AudioManager music_player;
} MainMenu;

//! Elkezdi lejátszani a zenét a főmenübe.
void play_music(MainMenu* main_menu);

//! Megnézi, hogy egy kapott gombra kattintottak-e vagy rajta van-e a kurzor.
void check_button(Button* btn,GameState* game_state);

//! inicialiál egy gombot és visszaadja a gomb címét kettős indirekcióval.
void setup_button(Button** btn, Vector2D posi, Vector2D sizei, int func, Vector2D font_start, char datai[]);

//! Felszabadítja a gomb felületét, hanglejátszóját.
void cleanup_button(Button* btn);

//! Kirajzolja a kapott gombot egy kapott betűtípussal.
void draw_button(Button* btn,RenderingContext* context,FontManager* font_manager);

//! Inicializálja a főmenüt, betölti a hanglejátszót, gombokat, zenét, textúrákat.
void main_menu_init(MainMenu* main_menu, RenderingContext* context);

//! Megnézi, hogy kattintottak-e a főmenü gombjaira.
void check_main_menu_inputs(MainMenu* main_menu,GameState* game_state);

//! Kirajzolja a főmenü gombjait, grafikus elemeit.
void draw_main_menu(MainMenu* main_menu,RenderingContext* context,FontManager* font_manager);

//- A toplista vizualizálásához szükséges struktúra. Tartalmaz egy vissza gombot, ami a főmenübe visz, egy felső szöveget, és egy szöveget, ami kiírja a TOP X helyezettet, pontszámait és a nehézséget ahol elérték azt.
typedef struct TopListScreen{
    FontSurface* self_label;
    FontSurface* top_string;
    Button* back_btn;
} TopListScreen;

//! Felszabadítja a toplista menűt és a főmenüt.
void cleanup_menus(MainMenu* mainmenu,TopListScreen* top_screen);

//! Inicializálja a top lista vizualizációs menüjét.
void init_top_list_screen(TopListScreen* top_list_screen, RenderingContext* context);

//! Kirajzolja a toplistát
void draw_top_list_screen(TopListScreen* top_list_screen,GameState* game_state,RenderingContext* context,FontManager* font_manager);

//- A halálképernyő struktúrája, kiírja, hogy a játékos hány pontot ért el, és hogy bekerült-e a toplistána. Ezen felül itt választja ki a játékos a nevét, hogy ha bekerült a toplisztába.
typedef struct DeathScreen{
    FontSurface* name_label; //Ez rajzolja ki a nevet amivel a játékos akar szerepelni
    FontSurface* self_label;
    Button* back_btn;
    SDL_Texture* toplist_label;
	bool freed;
    bool ontoplist;
    bool done;
    double timer;
} DeathScreen;

//! A halálképernyőt inicializákja.
void init_death_screen(DeathScreen* death_screen,RenderingContext* context);

//! Halálképernyő kirajzolásáért felelős függvény, függő a játékállapotba szerepelt értékektől és a paraméterként megadott betűtípust fogja használni szöveg rajzolásakor.
void update_death_button(DeathScreen* death_screen,GameState* game_state,RenderingContext* context,FontManager* font_manager);
#endif