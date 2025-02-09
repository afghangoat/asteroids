#ifndef KONSTANSOK_H
#define KONSTANSOK_H

//? A konstansok 1 header-be vannak, hogy könnyen lehessen konfigurálni a játékot

//A játéktér hossza és szélessége, fix egész érték.
#define JATEKTER_HOSSZ 1000
#define JATEKTER_MAGASSAG 600

//A megcélzott fps.
#define FPS 50
#define FRAME_TIME (1000/FPS)

#define RAD 0.0174532925

#define MAX_ASTEROID_SPEED 2.5

#define BULLET_SPEED 300.0
#define BULLET_RADIUS 6.0

#define RELOAD_COOLDOWN 0.5

#define FONT_WIDTH 16
#define FONT_HEIGHT 16

#define OUTLINE_SIZE 4

#define PLAYER_IFRAMES 1.0

#define TOP_SCORES_SIZE 10
#define MAX_NAME_LENGTH 12

#define MAX_SCORE_VALUE 999999

#define TOP_LIST_FILE "top_lista.txt"

#endif