# C Asteroids

![Spoiler image](https://afghangoat.hu/img/img50.png)

An asteroids game reimplementation in C using the SDL2 library.

The full documentation can be found in the "fejl_dokumentáció.pdf" file.

The full user documentation can be found in the "felhasználói_dokumentáció.pdf" file.

## Features

- Main menu
- Key bindings
- Custom font manager
- High score table
- Music, sound effects
- Pixel perfect graphics

Memory leaks were tested using "debugmalloc.h".

## Usage

- Run the build.bat on windows to compile the game.

- REQUIRES THE SDL2.DLL

## Hungarian guide

Asteroids felhasználói dokumentáció:

### A játékmenet:

A játékos egy űrhajót irányít, melynek az a célja, hogy szétlője a hozzá közel kerülő
aszteroidákat, így azok nem találják el. Az aszteroidák a képernyő széléről jönnek a játékos felé,
és ha meglövik őket, akkor szétválnak 3 kisebb darabra vagy teljesen eltűnnek.

**3 féle aszteroida van:**

1. A nagy aszteroida, ha ezt meglövik, akkor 3 közepes aszteroida lesz a helyén.
2. A közepes aszteroida, ha ezt meglövik, akkor 3 kicsi aszteroida keletkezik.
3. A kicsi aszteroida, ha ezt meglövik, akkor eltűnik, nem bomlik szét több aszteroidává.

A játékos pontot kap az aszteroidák szétlövéséért, minél nagyobb az aszteroida, annál több
pontot kap a felhasználó. Ha a felhasználó elér 10,000 pontot, akkor kap +1 életet.

Ha egy aszteroida eltalálja a játékost (az űrhajót), akkor elveszik egy + élete, ha a játékosnak
nincs több élete, akkor játék vége lesz, és kiértékelődik a pontszám.

Ha a játékos elhagyja a játékteret egy irányba, akkor "hiperugrik", és a képernyő másik
oldalán lyukad ki. Pl.: Ha a játékos felfelé elhagyja a képernyőt, akkor alul fog felbukkanni.

### Toplista:

A toplista a 10 legjobb pontszámot fogja elmenteni, névvel és nehézségi szinttel együtt, a
rangsort csak a pontszám befolyásolja, a nehézség nem. Amikor a játék véget ér, a felhasználó
látni fogja az elért pontszámát, és hogy felkerült-e a toplistára. Ha felkerült, akkor meg kell adnia
egy nevet, amivel ott fog szerepelni.

A toplista az adatokat a toplista.txt-be fogja tárolni és olvasni. Ilyen formátumban:
<pont>, <nehézség>, <játékosnév>. Ha a fájl beolvasásakor valami hiba merül fel, akkor úgy
veszi a program, hogy a fájl üres volt.

A játékos, ha felkerült a legjobb játékosok közé akkor a program bekéri a játékos nevét. A
játékos maximum 12 betű hosszúságú nevet adhat meg. A név csak nem ékezetes betűket és
számokat tartalmazhat.

### Irányítás:

A játékos az űrhajót ily módon tudja irányítani:

**W billentyű** : Az űrhajó előre fele gyorsul, amíg el nem engedi a játékos a billentyűt vagy
el nem éri a végsebességet az űrhajó.


**S billentyű** : Az űrhajó hátrafele elkezd tolatni, amíg a játékos el nem engedi a billentyűt
vagy el nem éri a maximum tolatási sebességet.

Ha a játékos sem a W, sem az S billentyűt nem nyomja le, akkor az űrhajó elkezd lassulni
és megáll.

```
A billentyű : Az űrhajó balra fordul, amíg a játékos nem engedi el a billentyűt.
```
```
D billentyű : Az űrhajó jobbra fordul, amíg a játékos nem engedi el a billentyűt.
```
**Szóköz** : Az űrhajó ki lő egy lövedéket, ami meg tud semmisíteni egy aszteroidát, feltéve,
hogy eltalál egyet is. Ha a játékos nyomva tartja a szóközt, akkor az űrhajó folyamatosan,
automatikusan lő.

### Pontok:

A játékos pontokat úgy szerez, ha szétlő aszteroidákat. A nagy aszteroida 400, a közepes
200, a kicsi aszteroida pedig 100 pontot ér. Ha a játékos szétlő egy aszteroidát, akkor egy szöveg
jelenik meg a helyén egy pillanatra, ami kijelzi, hogy hány pontot ért az az aszteroida.

### Menü:

A Menübe 4 gomb van, amire rá tud kattintani a felhasználó:

1. **A start gomb** : Ez elindítja a játékot a kiválasztott nehézségen.
2. **A kilépés gomb** : Ez kilép a játékból
3. **A top 10 gomb** : Ez megmutatja a top 10 játékos nevét, a nehézséget, amin játszott és a
    pontszámát.
4. **A nehézség változtatása gomb** : Ez 1 - gyel nehezíti a nehézséget, ha elérte a maximum
    nehézséget akkor a legkönnyebb nehézségre vált.
