#pragma once

#include "Vec2.h"
#include <SDL_Thread.h>
#include <vector>

using namespace std;

struct Drawable;
class SDL_Plotter;
class TextField;
class Rectangle;
class Button;
class Image;
class Circle;
class StickMan;

// TODO: Implement saving state for resuming game later

enum GameState {
    MAIN_MENU, PLAY, LENGTH, NAME, SCORE, OPTIONS
};


int update(void* data);

class Game;

void playAction(Game* game);
void optionsAction(Game* game);
void quitAction(Game* game);

class Game {
private:
    SDL_Plotter* g;
    Vec2i size;
    vector<Drawable *> drawables;
    vector<Drawable *> mainMenu;
    vector<Drawable *> playScreen;
    TextField* tField;
    Drawable* focus;
    Rectangle* testRect;

    // Order in drawable* vector determines draw order; thus, greater index -> higher layer in draw
    
    // Main Menu items
    Image* logo;
    Button* playButton;
    Button* optionsButton;
    Button* quitButton;

    // Play Screen items
    Rectangle* screenBorder;
    StickMan* stickMan;
    TextField* guessField;
    TextField* inputField;

    // High Score Screen items

    
    SDL_cond* cond;
    Image* testBkg;
    Circle* testCircle;
    StickMan* dave;
    Rectangle* sceneBorder;
    bool quit;
    GameState state;

    friend void playAction(Game* game);
    friend void optionsAction(Game* game);
    friend void quitAction(Game* game);
    friend int update(void* data);
    friend void changeState(Game* game);
    friend class Button;
public:
    Game();
    void mainLoop();
    void draw();

    ~Game();
};