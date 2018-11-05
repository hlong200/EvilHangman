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

// TODO: Implement saving state for resuming game later
struct GameState {
    enum {
        MAIN, PLAY, LENGTH, NAME, SCORE
    };

    unsigned int score;
    string word;
    vector<char> guesses;
};

int update(void* data);

class Game {
private:
    SDL_Plotter* g;
    Vec2i size;
    vector<Drawable *> drawables;
    TextField* tField;
    Drawable* focus;
    Rectangle* testRect;
    Button* testButton;
    SDL_cond* cond;
    bool quit;
    friend int update(void* data);
public:
    Game();
    void mainLoop();
    void draw();

    ~Game();
};