//? A pontszámok kirajzolásáért, beolvasásáért, elmentéséért felelő függvények és struktúrák vannak itt.
#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "./fontmanager.h"
#include "./menumanager.h"
#include "./player.h"
#include "./renderingmanager.h"
#include "./gamemanager.h"
#include "./audiomanager.h"
#include "debugmalloc.h"
#include <ctype.h>

//- Egy pontszám rekord, eltárolja a játékos nevét, pontját és a nehézséget, valamint a következő láncolt lista elemre egy pointert.
typedef struct ScoreEntry{
    int cur_score;
    int entry_difficulty;
    char entry_name[13]; //max név hossz: 12 + \0
    struct ScoreEntry* next_score;
} ScoreEntry;

//- A legmagasabb pontszámkezeőtartalmazza a pontszám rekordok láncolt listáját és a legkevesebb pontszámot olvasásra.
typedef struct HighScoreManager{
    //TOP_SCORES_SIZE
    int lowest;
    struct ScoreEntry* next_score;
} HighScoreManager;

//!Betölti a top pontszámokat a toplista menübe, szövegként egybe.
void load_top_list_screen(TopListScreen* top_list_screen,HighScoreManager* high_score_manager);

//! Elmenti a dicsőséglistát e toplista.txt file-ba.
void save_highscores(HighScoreManager* high_score_manager);
//! Berakja az új pontrekordot a dicsőséglistába a megfelelő helyezéshez.
void insert_to_highscores(HighScoreManager* high_score_manager, ScoreEntry* insertion_value);

//!Betölti a dicsőséglistát a toplista.txt file-ból.
void load_highscores(HighScoreManager* high_score_manager);

//- Ez a marker a szétlőtt aszteroidáért kapott pontot jelzi amíg az élettartama el nem fogy. Tartalmazza a pointert a láncolt listájának következő elemére.
typedef struct ScoreMarker{
    double timeleft;
    FontSurface* fontSurface;
    struct ScoreMarker* next_marker;
} ScoreMarker;
//- A pontmenedzser mutatja, hogy a jelenlegi játékba hány pontot ért el a játékos és ad életeket, ha elég pontot szerzett a játékos
typedef struct ScoreManager{
    Button* scorelabel;
    char* base_string;
    int given_lives;
    int score;
    ScoreMarker* next_marker;
} ScoreManager;

//! Felszabadítja a dicsőséglistát ami be lett töltve és kiszedi a memóriából. A markereket pedig kitörli és felszabadítja.
void cleanup_scores(HighScoreManager* hscoreman,ScoreManager* sman);

//! A halálmenüt és elemeit, gombjait felszabadítja.
void cleanup_death_screen(DeathScreen* death_screen);

//! nullázza a pont számolót. Új játék kezdésekor hívódik meg.
void reset_score_manager(ScoreManager* score_manager);

//! Inicializálja a pontszámlálót alap értékekkel.
void setup_score_manager(ScoreManager* score_manager);

//! Kirajzolja a pontszámlálót és a markereit.
void draw_score_manager(ScoreManager* score_manager, RenderingContext* context, FontManager* font_manager, Player* player);

//! A pontszámlálóhozzáadja magához a játékos által szerzett pontot.
void append_score(ScoreManager* score_manager,Player* player);

//! Hozzáad egy markert a pontszámlálóhoz, egy bizonyos helyre bizonyos értékkel.
void append_score_marker(ScoreManager* score_manager, int a_score, Vector2D pos);

//! A pontszámlálót frissíti, ad extra életet, ha kell és a markereket frissíti, vagy felszabadítja, ha kell.
void update_score_manager(ScoreManager* score_manager,GameState* game_state);

//! Megváltoztatja a halálképernyőre kiírt adatotat az elért pontszám alapján.
void setup_stats(DeathScreen* death_screen,ScoreManager* score_manager,HighScoreManager* high_score_manager);

//!Beolvassa a nevet addíg amíg nem éri el a név hossza a 12 karaktert vagy a játékos nem nyom enter. Ha entert nyom akkor felveszi a toplistába azzal a névvel.
int register_name(char* username);

//!Elmenti a pontszámot.
void save_score(HighScoreManager* hman,char* player_name,ScoreManager* score_manager,GameState* gamestate);
#endif