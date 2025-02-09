
#include "asteroidmanager.h"


void check_player_hit(Player* player,AsteroidSpawner* asteroid_spawner){
    Asteroid* current = asteroid_spawner->nextAsteroid;
    while (current->nextAsteroid != NULL&&current->angle<99999.0&&player->is_dead==false) {
        if(overlapping(current->pos,current->radius,player->pos,player->player_radius)==true){
            player->is_dead=true;
        }
        current = current->nextAsteroid;
    }
}
void setup_asteroid_spawner(AsteroidSpawner* asteroid_spawner,RenderingContext* context,int difficulty){
    asteroid_spawner->sprite_large=load_sprite(context,"assets/asteroid_large.bmp");
    asteroid_spawner->sprite_mid=load_sprite(context,"assets/asteroid_middle.bmp");
    asteroid_spawner->sprite_small=load_sprite(context,"assets/asteroid_small.bmp");
	change_diff(asteroid_spawner,difficulty);
    asteroid_spawner->frequency=1.0/((difficulty+1)*2.0)*5.0;
    asteroid_spawner->timer=0.0;
    asteroid_spawner->nextAsteroid=NULL;
	asteroid_spawner->bg_image=load_sprite(context,"assets/bg.bmp");
	
	Explosionmanager* eman=(Explosionmanager*) malloc(sizeof(Explosionmanager));
	eman->next_explosion=NULL;
	
	asteroid_spawner->exp_manager=eman;
	asteroid_spawner->exp_manager->explosion_atlas=load_sprite(context,"assets/explosion.bmp");
}
void change_diff(AsteroidSpawner* asteroid_spawner,int difficulty){
	asteroid_spawner->frequency=1.0/((difficulty+1)*2.0)*5.0;
}
void clear_asteroids(AsteroidSpawner* asteroid_spawner){
	Asteroid* current = asteroid_spawner->nextAsteroid;
    while (current != NULL) {
        Asteroid* asteroid_to_free = current;
        current = current->nextAsteroid;
        free(asteroid_to_free);
		asteroid_to_free=NULL;
    }

    asteroid_spawner->nextAsteroid = NULL;
	
	AsteroidExplosion* ecurrent = asteroid_spawner->exp_manager->next_explosion;
    while (ecurrent != NULL) {
        AsteroidExplosion* exp_to_free = ecurrent;
        ecurrent = ecurrent->next_explosion;
        free(exp_to_free);
		exp_to_free=NULL;
    }
	asteroid_spawner->exp_manager->next_explosion=NULL;
}

void cleanup_asteroid_manager(AsteroidSpawner* asteroid_spawner) {
    if (asteroid_spawner == NULL) {
        return;
    }
    clear_asteroids(asteroid_spawner);
	
	if(asteroid_spawner->exp_manager!=NULL){
		free(asteroid_spawner->exp_manager);
		asteroid_spawner->exp_manager=NULL;
	}
	
}
Vector2D get_random_asteroid_start(void){
    Vector2D start;
    switch(rand()%4){
        case 0: //fent
            start.y=-40;
            start.x=rand()%JATEKTER_HOSSZ;
            break;
        case 1: //lent
            start.y=JATEKTER_MAGASSAG+40;
            start.x=rand()%JATEKTER_HOSSZ;
            break;
        case 2: //bal
            start.y=rand()%JATEKTER_MAGASSAG;
            start.x=-40;
            break;
        case 3: //jobb
            start.y=rand()%JATEKTER_MAGASSAG+40;
            start.x=JATEKTER_HOSSZ;
            break;
    }
    return start;
}
Vector2D get_random_asteroid_vel(Vector2D position){
    Vector2D velocity;
    if(position.x<JATEKTER_HOSSZ/2.0){
        velocity.x=rand()%20+10;
    } else {
        velocity.x=-(rand()%20+10);
    }
    if(position.y<JATEKTER_MAGASSAG/2.0){
        velocity.y=rand()%20+10;
    } else {
        velocity.y=-(rand()%20+10);
    }
    return velocity;
}
void try_append_asteroid(AsteroidSpawner* asteroid_spawner,Asteroid* asteroid){
    if (asteroid_spawner->nextAsteroid == NULL) {
        asteroid_spawner->nextAsteroid = asteroid;
    } else {
        Asteroid* current = asteroid_spawner->nextAsteroid;
        while (current->nextAsteroid != NULL&&current->angle<99999.0) {
            current = current->nextAsteroid;
        }
        current->nextAsteroid = asteroid;
    }
}
void shatter_asteroid(AsteroidSpawner* asteroid_spawner,int type, Vector2D pos,ScoreManager* score_manager,Player* player){
    play_sound(&player->player_sounds,"assets/sfx/explosion.wav");
    int temp_score=400-type*100;
    player->bonus_score=temp_score;
    append_score(score_manager,player); 
    append_score_marker(score_manager, temp_score,pos);

	//Robbanás idézése
	spawn_explosion(asteroid_spawner,pos);
	
    if(type==3){
        return;
    }
    
    for(int i=0;i<3;i++){
        Asteroid* new_asteroid = (Asteroid*) malloc(sizeof(Asteroid));
        if (new_asteroid == NULL) {
            printf("A aszteroidának nem sikerült memóriát foglalni\n");
            return;
        }
        Vector2D shifted_pos;
        shifted_pos.x=pos.x-10.0+rand()%20;
        shifted_pos.y=pos.y-10.0+rand()%20;
        new_asteroid->pos = shifted_pos;
        new_asteroid->vel = get_random_asteroid_vel(new_asteroid->pos);
        new_asteroid->hit=false;
        new_asteroid->angle = (rand()%360)/360.0;
        if(type==1){
            new_asteroid->type = 2;
            new_asteroid->radius=24.0;
            new_asteroid->value=200;
            new_asteroid->self_texture = asteroid_spawner->sprite_mid;
        } else if(type==2){
            new_asteroid->type = 3;
            new_asteroid->radius=13.0;
            new_asteroid->value=50;
            new_asteroid->self_texture = asteroid_spawner->sprite_small;
        }
        new_asteroid->nextAsteroid = NULL;

        try_append_asteroid(asteroid_spawner,new_asteroid);
    }
}
void update_bullet_hits(BulletManager* bullet_manager,AsteroidSpawner* asteroid_spawner,ScoreManager* score_manager,Player* player){
    if (asteroid_spawner->nextAsteroid == NULL||bullet_manager->nextBullet == NULL) {
        return;
    }
    Asteroid* current_asteroid = asteroid_spawner->nextAsteroid;
    bool hit_em=false;
    while (current_asteroid != NULL&&current_asteroid->angle<99999.0) {
        if(current_asteroid->hit==false){
            hit_em=false;
			Bullet* current_bullet = bullet_manager->nextBullet;
            while (current_bullet != NULL&&current_bullet->pos.x<3000.0&&hit_em==false) {
                if (overlapping(current_asteroid->pos,current_asteroid->radius,current_bullet->pos,BULLET_RADIUS)==true) {
                    current_asteroid->hit=true;
                    current_bullet->hit=true;
                    hit_em=true;
                    shatter_asteroid(asteroid_spawner,current_asteroid->type,current_asteroid->pos,score_manager,player);
                }
                current_bullet=current_bullet->nextBullet;
            }
        }
        current_asteroid = current_asteroid->nextAsteroid;
    }
}
void spawn_asteroid(AsteroidSpawner* asteroid_spawner) {
    Asteroid* new_asteroid = (Asteroid*)malloc(sizeof(Asteroid));
    if (new_asteroid == NULL) {
        printf("Az aszteroidának nem sikerült memóriát foglalni\n");
        return;
    }

    new_asteroid->pos = get_random_asteroid_start();
    new_asteroid->vel = get_random_asteroid_vel(new_asteroid->pos);
    new_asteroid->type = 1;
    new_asteroid->radius=50.0;
    new_asteroid->hit=false;
    new_asteroid->self_texture = asteroid_spawner->sprite_large;
    new_asteroid->angle = 0.0;
    new_asteroid->nextAsteroid = NULL;
    new_asteroid->value=400;

    try_append_asteroid(asteroid_spawner,new_asteroid);
}
void try_spawn_asteroid(AsteroidSpawner* asteroid_spawner,GameState* game_state){
    asteroid_spawner->timer-=game_state->delta_time;
    if(asteroid_spawner->timer<0.0){
        spawn_asteroid(asteroid_spawner);
        asteroid_spawner->timer=asteroid_spawner->frequency;
    }
}
void update_asteroids(AsteroidSpawner* asteroid_spawner, GameState* game_state) {
    if (asteroid_spawner->nextAsteroid == NULL) {
        return;
    }
    
    Asteroid* current = asteroid_spawner->nextAsteroid;
    Asteroid* prev = NULL;

    while (current != NULL&&current->angle<99999.0) {
        Vector2D current_vel = multiply_vec2(current->vel, game_state->delta_time*MAX_ASTEROID_SPEED);
        current->pos = add_vec2(current->pos, current_vel);
        current->angle += 20.0 * game_state->delta_time;

        if (current->pos.x < -80 || current->pos.x > JATEKTER_HOSSZ + 80 || current->pos.y < -80 || current->pos.y > JATEKTER_MAGASSAG + 80||current->hit==true) {
            
            if (prev == NULL) {
                asteroid_spawner->nextAsteroid = current->nextAsteroid;
                free(current);
                current = asteroid_spawner->nextAsteroid;
            } else {
                prev->nextAsteroid = current->nextAsteroid;
                free(current);
                current = prev->nextAsteroid;
            }
        } else {
            prev = current;
            current = current->nextAsteroid;
        }
    }
}

//Kirajzolja a hátteret
void render_background(AsteroidSpawner* asteroid_spawner,RenderingContext* rendering_context){
	SDL_Rect bgtexture_r = { 0, 0, JATEKTER_HOSSZ, JATEKTER_MAGASSAG };
    SDL_RenderCopy(rendering_context->renderer, asteroid_spawner->bg_image, &bgtexture_r, &bgtexture_r);
}

void render_asteroid(Asteroid* asteroid,RenderingContext* rendering_context){
    int texture_width;
    int texture_height;
    switch(asteroid->type){
        case 1:
            texture_width=64;
            texture_height=64;
            break;
        case 2:
            texture_width=46;
            texture_height=41;
            break;
        case 3:
            texture_width=23;
            texture_height=24;
            break;
        default:
            return;
    }
    SDL_Rect asteroid_texture_r = { (int)asteroid->pos.x, (int)asteroid->pos.y, texture_width, texture_height };
    SDL_RenderCopyEx(rendering_context->renderer,asteroid->self_texture,NULL,&asteroid_texture_r,asteroid->angle,NULL,SDL_FLIP_NONE);
}
void render_asteroids(AsteroidSpawner* asteroid_spawner, RenderingContext* rendering_context) {
    if (asteroid_spawner->nextAsteroid == NULL) {
        return;
    }
    Asteroid* current = asteroid_spawner->nextAsteroid;

    while (current != NULL&&current->angle<99999.0) {
        render_asteroid(current, rendering_context);
        current = current->nextAsteroid;
    }
}

void render_explosions(AsteroidSpawner* asteroid_spawner, RenderingContext* rendering_context) {
    if (asteroid_spawner->exp_manager->next_explosion == NULL) {
        return;
    }
	AsteroidExplosion* current = asteroid_spawner->exp_manager->next_explosion;

    while (current != NULL) {
		
		SDL_Rect exp_src_texture_r = { (int)(1.0/current->lifetime)*64, 0, 64, 64 };
		SDL_Rect exp_texture_r = { (int)current->pos.x, (int)current->pos.y, 64, 64 };
		SDL_RenderCopy(rendering_context->renderer, asteroid_spawner->exp_manager->explosion_atlas, &exp_src_texture_r, &exp_texture_r);
		
        current = current->next_explosion;
    }
}
void add_explosion(AsteroidSpawner* asteroid_spawner,AsteroidExplosion* new_explosion){
	
    if (asteroid_spawner->exp_manager->next_explosion == NULL) {
        asteroid_spawner->exp_manager->next_explosion = new_explosion;
    } else {
        AsteroidExplosion* current = asteroid_spawner->exp_manager->next_explosion;
        while (current->next_explosion != NULL) {
            current = current->next_explosion;
        }
        current->next_explosion = new_explosion;
    }
}
void update_explosions(AsteroidSpawner* asteroid_spawner, GameState* game_state) {
    if (asteroid_spawner->exp_manager->next_explosion == NULL) {
        return;
    }
    
    AsteroidExplosion* current = asteroid_spawner->exp_manager->next_explosion;
    AsteroidExplosion* prev = NULL;

    while (current != NULL) {
		current->lifetime-=game_state->delta_time;

        if (current->lifetime<=0.0) {
            
            if (prev == NULL) {
                asteroid_spawner->exp_manager->next_explosion = current->next_explosion;
				
                free(current);
                current = asteroid_spawner->exp_manager->next_explosion;
            } else {
                prev->next_explosion = current->next_explosion;
                free(current);
				
                current = prev->next_explosion;
            }
        } else {
            prev = current;
            current = current->next_explosion;
        }
    }
}

void spawn_explosion(AsteroidSpawner* asteroid_spawner,Vector2D pos) {
    AsteroidExplosion* new_explosion = (AsteroidExplosion*)malloc(sizeof(AsteroidExplosion));
    if (new_explosion == NULL) {
        printf("A robbanásnak nem sikerült memóriát foglalni\n");
        return;
    }

    new_explosion->pos = pos;
    new_explosion->lifetime = 1.0;
    new_explosion->next_explosion=NULL;

    add_explosion(asteroid_spawner,new_explosion);
}