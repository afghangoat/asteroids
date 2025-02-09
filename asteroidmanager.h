//? Az aszteroidák, robbanások és a főbb játékelemek kezelése itt történik. A érintkezések vizsgálata itt történík meg, a pontok hozzáadása, az aszteroidák idézése, eltüntetése, szétrobbantása itt történik meg.
#ifndef ASTMANAGER_H
#define ASTMANAGER_H

#include <stdbool.h>
#include "./extramath.h"
#include "./player.h"
#include "./scoremanager.h"
#include "./renderingmanager.h"
#include "./gamemanager.h"
#include "./bulletmanager.h"
#include "./audiomanager.h"
#include "debugmalloc.h"

//- a robbanás struct, tartalmazza a pozíciót és azt, hogy meddig él még, valamint egy pointert a következő lista elemre.
typedef struct AsteroidExplosion{
	Vector2D pos;
	double lifetime;
	struct AsteroidExplosion* next_explosion;
} AsteroidExplosion;

//- A robbanás irányítzó szerkezete, tartalmazza a textúráját a robbanásnak és a láncolt robbanás lista fejét.
typedef struct Explosionmanager{
	SDL_Texture* explosion_atlas;
	struct AsteroidExplosion* next_explosion;
} Explosionmanager;

//- Az asteroida szerkezete, van pozíciója, iránya, forgása, rádiusza és típusa, 3 típus van, kicsi, nagy és nagyon nagy aszteroida. Tartalmaz egy pointert a következő aszteroida elemre.
typedef struct Asteroid{
    SDL_Texture* self_texture;
    Vector2D vel;
    Vector2D pos;
    int type;
    bool hit;
    double angle;
    double radius;
    int value;
    struct Asteroid* nextAsteroid;
} Asteroid;

//- Az aszteroida idéző idézi az aszteroidákat, tárolja a robbanásokat, a robbanáskezelőt és az aszteroidák láncolt listáját.
typedef struct AsteroidSpawner{
    SDL_Texture* sprite_large;
    SDL_Texture* sprite_mid;
    SDL_Texture* sprite_small;
	SDL_Texture* bg_image;
    double frequency;
    double timer;
	struct Explosionmanager* exp_manager;
    struct Asteroid* nextAsteroid;
} AsteroidSpawner;

//! Megváltoztatja az aszteroidák gyakoriságát a nehézségtől függően.
void change_diff(AsteroidSpawner* asteroid_spawner,int difficulty);

//! Kirajzolja a hháttérbe lévő aszteroidákat.
void render_background(AsteroidSpawner* asteroid_spawner,RenderingContext* rendering_context);

//! megnézi, hogy a játékos el lett-e találva egy aszteroida által.
void check_player_hit(Player* player,AsteroidSpawner* asteroid_spawner);

//! Inicializálja az aszteroida kezelőt, és elemeit, betölti a textúrákat
void setup_asteroid_spawner(AsteroidSpawner* asteroid_spawner,RenderingContext* context,int difficulty);

//! Kitörli az összes aszteroidát
void clear_asteroids(AsteroidSpawner* asteroid_spawner);

//! Felszabadítja az aszteroida kezelő és elemei lefoglalt memóriáját.
void cleanup_asteroid_manager(AsteroidSpawner* asteroid_spawner);

//! Egy véletlenszerű aszteroida kezdőpozíciót ad vissza, a képernyő valamelyik szélén.
Vector2D get_random_asteroid_start(void);

//! Véletlenszerű aszteroidasebességet ad vissza (és írányt ami kb. a pálya közepe fele mutat)
Vector2D get_random_asteroid_vel(Vector2D position);

//! hozzáadja az új aszteroidát az aszteroidák listájához.
void try_append_asteroid(AsteroidSpawner* asteroid_spawner,Asteroid* asteroid);

//! Széttöri az aszteroidát kisebb darabokra és pontot ad a játékosnak a találatért. Ha nagyon kicsit volt az aszteroida akkor csak eltűnik.
void shatter_asteroid(AsteroidSpawner* asteroid_spawner,int type, Vector2D pos,ScoreManager* score_manager,Player* player);

//! Megnézi, hogy kaptak-e az aszteroidák találatot.
void update_bullet_hits(BulletManager* bullet_manager,AsteroidSpawner* asteroid_spawner,ScoreManager* score_manager,Player* player);

//! Inicializál egy új aszteroidát.
void spawn_asteroid(AsteroidSpawner* asteroid_spawner);

//! Megpróbál idézni egy aszteroidát, bizonyos időközönként, ami nehézségtől függ.
void try_spawn_asteroid(AsteroidSpawner* asteroid_spawner,GameState* game_state);

//! Frissíti, mozgatja az aszteroidákat, ha kimentek a pályáról akkor eltüntetí őket a függvény.
void update_asteroids(AsteroidSpawner* asteroid_spawner, GameState* game_state);

//! Kirajzol egy aszteroidát.
void render_asteroid(Asteroid* asteroid,RenderingContext* rendering_context);

//! Végigmegy minden aszteroidán és kirajzolja őket.
void render_asteroids(AsteroidSpawner* asteroid_spawner, RenderingContext* rendering_context);

//! Kirajzolja a robbanásokat.
void render_explosions(AsteroidSpawner* asteroid_spawner, RenderingContext* rendering_context);

//! Hozzáad egy robbanást a robbanások láncolt listájához.
void add_explosion(AsteroidSpawner* asteroid_spawner,AsteroidExplosion* new_explosion);

//! Inicializál egy robbanást egy megadott helyen.
void spawn_explosion(AsteroidSpawner* asteroid_spawner,Vector2D pos);

//! Frissíti a robbanásokat. Ha lejárt az élettartamuk akkor kitörli őket.
void update_explosions(AsteroidSpawner* asteroid_spawner, GameState* game_state);
#endif