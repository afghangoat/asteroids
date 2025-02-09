//? A lövedékkezelő és a lövedékhez szükséges adatstruktúrák, függvények, definíciók vannak itt.
#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <stdbool.h>
#include "./extramath.h"
#include "./renderingmanager.h"
#include "./player.h"
#include "./gamemanager.h"
#include "debugmalloc.h"

//- A lövedék elem adatstruktúrája, tartalmazza a pozíciót, az irányt, azt,hogy talált-e és a szögét, valamint a következő lövedékre mutató pointert.
typedef struct Bullet{
    Vector2D pos;
    Vector2D vel;
    bool hit;
    double angle;
    struct Bullet* nextBullet;
} Bullet;

//- A lövedék kezelő struktúrája, tartalmazza a lövedékek láncolt listájának a fejét és a lövedék textúrát.
typedef struct BulletManager{
    SDL_Texture* bullet_texture;
    struct Bullet* nextBullet;
} BulletManager;

//! Inicializálja a lövedék kezelőt, betölti a lövedék textúráját.
void setup_bullet_manager(BulletManager* bullet_manager,RenderingContext* context);

//! Csinál egy lövedéket a játékos iránya és pozíciója alapján és berakja a láncolt listába.
void shoot_bullet(BulletManager* bullet_manager,Player* player);

//! Frissíti a lövedékek helyét, letörli őket, ha kimentek a pályáról vagy eltaláltak egy aszteroidát.
void update_bullets(BulletManager* bullet_manager,GameState* game_state);

//! Kirajzolja a lövedékeket egyesével.
void render_bullets(BulletManager* bullet_manager,RenderingContext* context);

//! Az összes lövedéket kitörli és a foglalt memóriájukat felszabadítja.
void cleanup_bullets(BulletManager* bullet_manager);
#endif