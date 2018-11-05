#include "Game.h"
#include "SDL_Plotter.h"
#include "Vec2.h"
#include "Color.h"
#include "Drawable.h"
#include <fstream>

int main(int argc, char ** argv) {
    Game evilHangman;
    return 0;
}

Game::Game() {
    size = Vec2i(1280, 720);
    g = new SDL_Plotter(size.y, size.x);

    for (char c = 'A'; c < 'Z'; c++) {
        try {
            letters[c] = Char(c, CHAR_SIZE);
        }
        catch (const string& fileName) {
            cerr << fileName << " could not be loaded!" << endl;
        }
    }

    try {
        letters['!'] = Char('!', CHAR_SIZE);
    }
    catch (const string& fileName) {
        cerr << fileName << " could not be loaded!" << endl;
    }

    cout << "Letters loaded!" << endl;

    tField = new TextField("Hello World!");
    tField->setLocation(Vec2i(100, 100));
    tField->setFgColor(BLUE);
    

    testRect = new Rectangle(50, 50, size.x - 100, size.y - 100);
    testRect->setColor(RED);
    drawables.push_back(testRect);

    testButton = new Button("This is a test!");
    testButton->setLocation(Vec2i(400, 400));
    testButton->setSize(Vec2i(100, 100));
    drawables.push_back(testButton);
    drawables.push_back(tField);

    mainLoop();
}

void Game::mainLoop() {
    cond = SDL_CreateCond();
    SDL_Thread *updateThread = SDL_CreateThread(update, "Update Thread", this);

    while (!quit) {
        draw();

        if (g->kbhit()) {
            g->getKey();
        }
        quit = g->getQuit();

        g->update();
        g->Sleep(16);
        g->clear();
    }

    SDL_WaitThread(updateThread, NULL);
}

int update(void* data) {
    Game* game = (Game*)data;

    while (!game->g->getQuit()) {
        for (int i = 0; i < game->drawables.size(); i++) {
            game->drawables[i]->update(game->g);
        }
    }

    return 0;
}

void Game::draw() {
    for (int i = 0; i < drawables.size(); i++) {
        drawables[i]->draw(g);
    }
}

Game::~Game()
{
}

void plot(SDL_Plotter * g, const Vec2i & p, const Color& color) {
  if (p.x >= 0 && p.x < g->getCol() && p.y >= 0 && p.y < g->getRow()) {
    g->plotPixel(p.x, p.y, color.r, color.g, color.b);
  }
}

Vec2f::Vec2f() {
  x = y = 0;
}

Vec2f::Vec2f(float x, float y) {
  this->x = x;
  this->y = y;
}

Vec2f::Vec2f(int x, int y) : x(float(x)), y(float(y)) {}

Vec2f Vec2f::operator+(const Vec2f & other) {
  Vec2f result = Vec2f(other.x + x, other.y + y);
  return result;
}

Vec2f & Vec2f::operator+=(const Vec2f & other) {
  x += other.x;
  y += other.y;
  return *this;
}

Color::Color() {
  r = g = b = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

Color::Color(int r, int g, int b) : r(uint8_t(r)), g(uint8_t(g)), b(uint8_t(b)) {}

Color Color::operator+(const Color & other) {
  Color result(other.r + r, other.g + g, other.b + b);
  return result;
}

Color & Color::operator+=(const Color & other) {
  r += other.r;
  g += other.g;
  b += other.b;
  return *this;
}

bool Color::operator==(const Color& other) {
    bool res = r == other.r && g == other.g && b == other.b;
    return res;
}

void drawLine(SDL_Plotter* g, const Vec2i& p1, const Vec2i& p2, const Color& color) {
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    dx1 = fabs(dx);
    dy1 = fabs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;

    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = p1.x;
            y = p1.y;
            xe = p2.x;
        }
        else {
            x = p2.x;
            y = p2.y;
            xe = p1.x;
        }
        plot(g, Vec2f(x, y), color);
        for (i = 0; x < xe; i++) {
            x += 1;
            if (px < 0) {
                px += 2 * dy1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y += 1;
                }
                else {
                    y -= 1;
                }
                px += 2 * (dy1 - dx1);
            }
            plot(g, Vec2f(x, y), color);
        }
    }
    else {
        if (dy >= 0) {
            x = p1.x;
            y = p1.y;
            ye = p2.y;
        }
        else {
            x = p2.x;
            y = p2.y;
            ye = p1.y;
        }
        plot(g, Vec2f(x, y), color);
        for (i = 0; y < ye; i++) {
            y += 1;
            if (py <= 0) {
                py += 2 * dx1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x += 1;
                }
                else {
                    x -= 1;
                }
                py += 2 * (dx1 - dy1);
            }
            plot(g, Vec2f(x, y), color);
        }
    }
}

void drawRectangle(SDL_Plotter * g, const Vec2i & location, const Vec2i & size, const Color & color, bool fill) {
    for (int y = 0; y < size.y; y++) {
        drawLine(g, Vec2i(location.x, location.y + y), Vec2i(location.x + size.x, location.y + y), color);
    }
}

Line::Line() : p1(Vec2f()), p2(Vec2f()) {}

Line::Line(const Vec2i & p1, const Vec2i & p2) : p1(p1), p2(p2) {}

Line::Line(int x1, int y1, int x2, int y2) : p1(x1, y1), p2(x2, y2) {}

void Line::draw(SDL_Plotter * g) {
    drawLine(g, p1, p2, color);
}

void Line::update(SDL_Plotter * g) {}

Rectangle::Rectangle() {

}

Rectangle::Rectangle(const Vec2i & location, const Vec2i & size) : location(location), size(size) {}

Rectangle::Rectangle(const Vec2i & location, int width, int height) : Rectangle(location, Vec2i(width, height)) {}

Rectangle::Rectangle(int x, int y, int width, int height) : Rectangle(Vec2i(x, y), Vec2i(width, height)) {}

Vec2i Rectangle::getLocation() const {
    return location;
}

Vec2i Rectangle::getSize() const {
    return size;
}

int Rectangle::getX() const {
    return location.x;
}

int Rectangle::getY() const {
    return location.y;
}

int Rectangle::getWidth() const {
    return size.x;
}

int Rectangle::getHeight() const {
    return size.y;
}

Color Rectangle::getColor() const {
    return color;
}

bool Rectangle::getFilled() const {
    return fill;
}

void Rectangle::setFilled(bool filled) {
    fill = filled;
}

void Rectangle::setColor(const Color & color) {
    this->color = color;
}

void Rectangle::setLocation(const Vec2i & location) {
    this->location = location;
}

void Rectangle::setSize(const Vec2i & size) {
    this->size = size;
}

void Rectangle::setX(int x) {
    location.x = x;
}

void Rectangle::setY(int y) {
    location.y = y;
}

void Rectangle::setWidth(int width) {
    size.x = width;
}

void Rectangle::setHeight(int height) {
    size.y = height;
}

void Rectangle::update(SDL_Plotter * g) {
}

void Rectangle::draw(SDL_Plotter * g) {
    drawRectangle(g, location, size, color, true);
}

Pane::Pane(Pane * parent, Vec2i offset) {
  this->parent = parent;
  this->offset = offset;
}

Pane * Pane::getParent() const {
  return parent;
}

Vec2i Pane::getLocation() const {
  return location;
}

Vec2i Pane::getOffset() const {
  return offset;
}

Vec2i Pane::getSize() const {
  return size;
}

bool Pane::inside(const Vec2i & location) {
    return (location.x > this->location.x &&
        location.x < this->location.x + size.y &&
        location.y > this->location.y &&
        location.y < this->location.y + size.y);
}

void Pane::setParent(Pane * parent) {
  this->parent = parent;
}

void Pane::setLocation(const Vec2i & location) {
  this->location = location;
}

void Pane::setOffset(const Vec2i & offset) {
  this->offset = offset;
}

void Pane::setSize(const Vec2i & size) {
  this->size = size;
}

void Pane::draw(SDL_Plotter * g) {}

void Pane::update(SDL_Plotter * g) {}

Image::Image(const string & fileName) {
  load(fileName);
}

void Image::load(const string & fileName) {
  ifstream file(fileName);
  if (!file) {
    throw fileName;
  }
  file >> size.x >> size.y;
  Color tmp;

  for (int i = 0; i < size.x * size.y; i++) {
    file >> tmp.r >> tmp.g >> tmp.b;
    data.push_back(tmp);
  }
}

void Image::draw(SDL_Plotter * g) {
  Vec2i drawLoc;
  if (parent == nullptr) {
    drawLoc = location;
  }
  else {
    drawLoc = parent->getLocation() + offset;
  }
  for (int r = 0; r < size.y; r++) {
    for (int c = 0; c < size.x; c++) {
      plot(g, drawLoc, data[r * size.x + c]);
    }
  }
}

void Image::update(SDL_Plotter * g) {}

Vec2i::Vec2i() : x(0), y(0) {}

Vec2i::Vec2i(int x, int y) : x(x), y(y) {}

Vec2i::Vec2i(float x, float y) : x(int(x)), y(int(y)) {}

Vec2i::Vec2i(const Vec2f & other) : x(int(other.x)), y(int(other.y)) {}

Vec2i Vec2i::operator+(const Vec2i & other) {
  Vec2i result(other.x + x, other.y + y);
  return result;
}

Vec2i & Vec2i::operator+=(const Vec2i & other) {
  x += other.x;
  y += other.y;
  return *this;
}

TextField::TextField() : Pane(nullptr) {
  this->canEdit = false;
}

TextField::TextField(const string & text) : Pane(nullptr) {
  setText(text);
}

string TextField::getText() const {
  return text;
}

bool TextField::getCanEdit() const {
  return canEdit;
}

void TextField::setText(const string & text) {
  this->text = text;
  field.clear();
  for (unsigned int i = 0; i < text.size(); i++) {
    Char tmp = letters[(char)toupper(text[i])];
    tmp.setParent(this);
    tmp.setOffset(Vec2i(i * CHAR_SIZE, 0));
    field.push_back(tmp);
  }
}

void TextField::setCanEdit(bool b) {
    this->canEdit = b;
}

void TextField::setFgColor(const Color& color) {
    fg = color;
    for (int i = 0; i < field.size(); i++) {
        field[i].setColor(fg);
    }
}

void TextField::draw(SDL_Plotter * g) {
    for (int i = 0; i < field.size(); i++) {
        field[i].draw(g);
    }
}

void TextField::update(SDL_Plotter * g) {
    char key = g->getKey();
    if (g->kbhit()) {
        if (key == '<') {
            setText(text.substr(0, text.size() - 1));
        }
        else if(isalnum(key) || key == ' ') {
            setText(text + key);
        }
    }
}

Char::Char() {
  id = '\0';
  size = 0;
  scale = 0;
  color = BLACK;
}

Char::Char(char id, unsigned short size) : Pane(nullptr) {
  this->id = id;
  this->size = size;
  this->scale = 1;
  this->color = BLACK;
  string fileName = RES_CHAR + char(toupper(id)) + ".txt";
  loadChar(fileName);
}

char Char::getId() const {
  return id;
}

void Char::loadChar(const string & fileName) {
  ifstream file(fileName, ios::binary);
  if (!file) {
    throw fileName;
  }

  uint8_t tmp;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      file >> tmp;
      data.push_back(tmp);
    }
  }
}

void Char::setColor(const Color& color) {
    this->color = color;
}

void Char::draw(SDL_Plotter * g) {
  Vec2i drawLoc;
  if (parent == nullptr) {
    drawLoc = location;
  }
  else {
    drawLoc = parent->getLocation() + offset;
  }

  //cout << "Drawing " << this->id << " at " << drawLoc.x << ", " << drawLoc.y << endl;

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (data[r * size + c] == uint8_t(49)) {
        plot(g, drawLoc + Vec2i(c, r), color);
      }
    }
  }
}

void Char::update(SDL_Plotter * g) {}


Button::Button(const string & text) : tField(TextField(text)) {

}

void Button::onHover(const Vec2i & location) {
    
}

void Button::onClick(const Vec2i & location) {
    if (color == BLUE) {
        color = INDIGO;
    }
    else {
        color = BLUE;
    }
}

void Button::draw(SDL_Plotter * g) {
    drawRectangle(g, location, size, color, true);
}

void Button::update(SDL_Plotter * g) {
    Vec2i mouseLoc;
    if (g->getMouseClick(mouseLoc.x, mouseLoc.y)) {
        if (inside(mouseLoc)) {
            onClick(mouseLoc);
        }
    }
    if (g->getMouseMotion(mouseLoc.x, mouseLoc.y)) {
        if (inside(mouseLoc)) {
            onHover(mouseLoc);
        }
    }
}

void Button::setLocation(const Vec2i & location) {
    Pane::setLocation(location);
    tField.setLocation(location);
}
