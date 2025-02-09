
#include "bulletmanager.h"


void setup_bullet_manager(BulletManager* bullet_manager,RenderingContext* context){
    bullet_manager->bullet_texture=load_sprite(context,"assets/bullet.bmp");
    bullet_manager->nextBullet=NULL;
}
void shoot_bullet(BulletManager* bullet_manager,Player* player){
    player->shooting=false;
    player->cooldown=RELOAD_COOLDOWN;
    Bullet* new_bullet = (Bullet*)malloc(sizeof(Bullet));
    if (new_bullet == NULL) {
        printf("A lövedéknek nem sikerült memóriát foglalni!\n");
        return;
    }

    new_bullet->pos = player->pos;
    Vector2D offset_to_player;
    offset_to_player.x=16;
    offset_to_player.y=8;
    new_bullet->hit=false;
    new_bullet->pos=add_vec2(new_bullet->pos,offset_to_player);
    
    new_bullet->angle = player->angle;

    new_bullet->vel.x=cos(new_bullet->angle*RAD)*BULLET_SPEED;
    new_bullet->vel.y=sin(new_bullet->angle*RAD)*BULLET_SPEED;
    new_bullet->nextBullet = NULL;

    if (bullet_manager->nextBullet == NULL) {
        bullet_manager->nextBullet = new_bullet;
    } else {
        Bullet* current = bullet_manager->nextBullet;
        while (current->nextBullet != NULL&&current->pos.x<3000.0) {
            current = current->nextBullet;
        }
        current->nextBullet = new_bullet;
    }
}
void update_bullets(BulletManager* bullet_manager,GameState* game_state){
    if(bullet_manager->nextBullet==NULL){
        return;
    }
    Bullet* current = bullet_manager->nextBullet;
    Bullet* prev = NULL;

    while (current != NULL&&current->pos.x<3000.0) {
        Vector2D current_vel = multiply_vec2(current->vel, game_state->delta_time);
        current->pos = add_vec2(current->pos, current_vel);

        if (current->pos.x < -80 || current->pos.x > JATEKTER_HOSSZ + 80 || current->pos.y < -80 || current->pos.y > JATEKTER_MAGASSAG + 80||current->hit==true) {
            
            if (prev == NULL) {
                bullet_manager->nextBullet = current->nextBullet;
                free(current);
                current = bullet_manager->nextBullet;
            } else {
                prev->nextBullet = current->nextBullet;
                free(current);
                current = prev->nextBullet;
            }
        } else {
            prev = current;
            current = current->nextBullet;
        }
    }
}
void render_bullets(BulletManager* bullet_manager,RenderingContext* context){
    if(bullet_manager->nextBullet==NULL){
        return;
    }
    Bullet* current = bullet_manager->nextBullet;

    while (current != NULL&&current->pos.x<3000.0) {
        SDL_Rect bullet_texture_r = { (int)current->pos.x, (int)current->pos.y, 16, 8 };
        SDL_RenderCopyEx(context->renderer,bullet_manager->bullet_texture,NULL,&bullet_texture_r,current->angle,NULL,SDL_FLIP_NONE);
        current = current->nextBullet;
    }
}

//Végig megy a fel nem szabadított lövedékeken én
void cleanup_bullets(BulletManager* bullet_manager) {
    if (bullet_manager->nextBullet == NULL) {
        return;
    }

    Bullet* current = bullet_manager->nextBullet;
    Bullet* next;

    while (current != NULL) {
        next = current->nextBullet;
        free(current);
        current = next;
    }

    bullet_manager->nextBullet = NULL;
}