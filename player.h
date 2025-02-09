//? A játékos struktúra, függvényei és az input függvényeit tartalmazó forrásfájl.
#ifndef PLAYER_H
#define PLAYER_H
#include "./renderingmanager.h"
#include "./gamemanager.h"
#include "./audiomanager.h"
#include "debugmalloc.h"

//- A játékos struktó+úrája, eltárolja a játékos pozícióját, irányát, gyorsulását, sebességét, a lövés cooldown-t, azt, hogy halott-e, az életeket, a bónusz pontokat, a textúrát, a hanglejátszót és az invincibility frame-eket.
typedef struct Player {
    Vector2D pos;
    double angle;
    double velocity;
    double accel;
    bool moving;
    double cooldown;
    bool shooting;
    bool is_dead;
    double player_radius;
    int lives;
    int bonus_score;
    double iframes;
    SDL_Texture* sprite;
    AudioManager player_sounds;
    
} Player;

//! Inicializálja a játékost, betölti a textúrát, a hanglejátszót, alap értékeket ad a játékosnak.
void init_player(Player* player,RenderingContext* current_context);

//! Felszabadítja a játékos által lefoglalt memóriát és a hanglejátszót.
void cleanup_player(Player* player);

//! Leidézi a játékost és ad neki alap értékeket. A játékost a pálya közepére idézi le.
void spawn_player(Player* player);

//! A játékosnak ad 3 életet és nulláza a bónuszpontjait, hiszen ekkor indul a játék.
void setup_player(Player* player);

//! Ha a játékosnak van bónusz élete akkor újraéleszti a játékost és egy extra életet elvesz, ha nincs akkor a halálképernyőt előhozza.
void try_respawn_player(Player* player,GameState* game_state);

//! Frissíti a játékos gyorsaságát, lövését, gyorsulását, irányát a bemenet alapján.
void update_player(Player* player,GameState* gamestate);

//! Kirajzolja a játékost.
void render_player(Player* player,RenderingContext* context);

//! Az inputok kezelése, egérmozgás, egérlenyomás, betűlenyomások kezelése, eltárolása.
void process_input(Player* player,GameState* game_state);
#endif