//? A kiegészített matematikai struktúrák, függvények vannak itt tárolva.
#ifndef EXTRAMATH_H
#define EXTRAMATH_H


#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "./konstansok.h"

//- Egy kétdimenziós vektor struktúrája, tartalmazza az x és y koordinátát.
typedef struct Vector2D {
    double x;
    double y;
} Vector2D;

//! Inicializál egy 2 dimenziós vektort a paraméterként kapott x és y pozíción és visszaadja azt.
Vector2D init_vec2(double x1,double y1);

//! Megszoroz egy paraméterként kapott vektort egy lambda valós számmal. Visszaadja a szorzatvektort.
Vector2D multiply_vec2(Vector2D v1, double lambda);

//! összead 2 darab 2 dimenziós vektort és visszaadja az összeget.
Vector2D add_vec2(Vector2D v1, Vector2D v2);

//! Visszaadja az távolságot 2 db 2 dimenziós vektor között.
double distance_vec2(Vector2D v1, Vector2D v2);

//! Megnézi, hogy 2 körnek van-e metszete. A paraméterek a 2 kör koordinátája és sugarai.
bool overlapping(Vector2D pos1, double radius1, Vector2D pos2,double radius2);

//! Ha az 1. paraméter kisebb, mint a minimum paraméter, akkor a minimum paramétert adja vissza, ha nagyobb mint a maximum paraméter akkor a maximum paramétert adja vissza,különben csak visszaadja az 1. paramétert.
double clampd(double num, double min, double max);
#endif