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

    /*
    try {
        testBkg = new Image(RES_IMG + "doctor1.txt");
    }
    catch (const string& fileName) {
        cerr << fileName << " could not be opened!" << endl;
    }
    testBkg->setLocation(Vec2i(0, 0));
    testBkg->draw(g);
    //drawables.push_back(testBkg);
    */

    sceneBorder = new Rectangle(10, 10, size.x - 20, 500, BLACK, false);
    drawables.push_back(sceneBorder);

    tField = new TextField("Hello World!");
    tField->setLocation(Vec2i(100, 100));
    tField->setFgColor(BLUE);
    

    //testRect = new Rectangle(50, size.y - 16 - 50, size.x - 100, size.y - 100);
    //testRect->setColor(RED);
    //drawables.push_back(testRect);

    testButton = new Button("This is a test!");
    testButton->setLocation(Vec2i(400, 400));
    testButton->setSize(Vec2i(100, 100));
    drawables.push_back(testButton);
    drawables.push_back(tField);

    testCircle = new Circle(Vec2i(size.x/2, size.y/2), 75, VIOLET, false);
    drawables.push_back(testCircle);

    dave = new StickMan(Vec2i(350, 350), BLACK);
    drawables.push_back(dave);

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
    }

    SDL_WaitThread(updateThread, NULL);
}

int update(void* data) {
    Game* game = (Game*)data;

    while (!game->g->getQuit()) {
        for (int i = 0; i < game->drawables.size(); i++) {
            game->drawables[i]->update(game->g, game);
        }
    }

    return 0;
}

void Game::draw() {
    for (int i = 0; i < drawables.size(); i++) {
        drawables[i]->draw(g);
    }
    g->update();
    g->Sleep(10);
}

Game::~Game()
{
}

void plot(SDL_Plotter * g, const Vec2i & p, const Color& color) {
  if (p.x >= 0 && p.x < g->getCol() && p.y >= 0 && p.y < g->getRow()) {
      //cout << (int)color.r << " " << (int)color.g << " " << (int)color.b << endl;
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

Color::Color(unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

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

void drawRectangle(SDL_Plotter * g, const Rectangle& rect) {
    if (rect.getFilled()) {
        for (int y = 0; y < rect.getHeight(); y++) {
            drawLine(g, Vec2i(rect.getLocation().x, rect.getLocation().y + y), Vec2i(rect.getLocation().x + rect.getWidth(), rect.getLocation().y + y), rect.getColor());
        }
    }
    else {
        // Top line
        drawLine(g, rect.getLocation(), rect.getLocation() + Vec2i(rect.getWidth(), 0), rect.getColor());
        // Left line
        drawLine(g, rect.getLocation(), rect.getLocation() + Vec2i(0, rect.getHeight()), rect.getColor());
        // Right line
        drawLine(g, rect.getLocation() + Vec2i(rect.getWidth(), 0), rect.getLocation() + Vec2i(rect.getWidth(), rect.getHeight()), rect.getColor());
        // Bottom line
        drawLine(g, rect.getLocation() + Vec2i(0, rect.getHeight()), rect.getLocation() + Vec2i(rect.getWidth(), rect.getHeight()), rect.getColor());
    }
}

Line::Line() : p1(Vec2f()), p2(Vec2f()) {}

Line::Line(const Vec2i & p1, const Vec2i & p2) : p1(p1), p2(p2) {}

Line::Line(int x1, int y1, int x2, int y2) : p1(x1, y1), p2(x2, y2) {}

Vec2i Line::getP1() const {
    return p1;
}
Vec2i Line::getP2() const {
    return p2;
}
Color Line::getColor() const {
    return color;
}
void Line::setP1(const Vec2i& other) {
    p1 = other;
}
void Line::setP2(const Vec2i& other) {
    p2 = other;
}
void Line::setColor(const Color& color) {
    this->color = color;
}

void Line::draw(SDL_Plotter * g) {
    drawLine(g, p1, p2, color);
}

void Line::update(SDL_Plotter * g, Game* game) {}

Rectangle::Rectangle() {

}

Rectangle::Rectangle(const Vec2i & location, const Vec2i & size, const Color& color, bool filled) : location(location), size(size), color(color), fill(filled) {}

Rectangle::Rectangle(const Vec2i & location, int width, int height, const Color& color, bool filled) : Rectangle(location, Vec2i(width, height), color, filled) {}

Rectangle::Rectangle(int x, int y, int width, int height, const Color& color, bool filled) : Rectangle(Vec2i(x, y), Vec2i(width, height), color, filled) {}

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

void Rectangle::update(SDL_Plotter * g, Game* game) {
}

void Rectangle::draw(SDL_Plotter * g) {
    drawRectangle(g, *this);
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

void Pane::update(SDL_Plotter * g, Game* game) {}

void drawCircle(SDL_Plotter* g, const Circle& circle) {
    Vec2i curLoc;
    for (int x = circle.getLocation().x - circle.getRadius(); x < circle.getLocation().x + circle.getRadius(); x++) {
        for (int y = circle.getLocation().y - circle.getRadius(); y < circle.getLocation().y + circle.getRadius(); y++) {
            curLoc = Vec2i(x, y);

            if (circle.getLocation().dist(curLoc) <= circle.getRadius()) {
                if (circle.getFilled()) {
                    plot(g, curLoc, circle.getColor());
                }
                else if (circle.getLocation().dist(curLoc) > circle.getRadius() - 1) {
                    plot(g, curLoc, circle.getColor());
                }
            }
        }
    }
}

Circle::Circle(const Vec2i& location, int radius, const Color& color, bool filled) {
    this->location = location;
    this->radius = radius;
    this->color = color;
    this->fill = filled;
}

Vec2i Circle::getLocation() const {
    return location;
}

int Circle::getRadius() const {
    return radius;
}

Color Circle::getColor() const {
    return color;
}

bool Circle::getFilled() const {
    return fill;
}

void Circle::setLocation(const Vec2i& location) {
    this->location = location;
}

void Circle::setRadius(int radius) {
    this->radius = radius;
}

void Circle::setColor(const Color& color) {
    this->color = color;
}

void Circle::setFilled(bool filled) {
    fill = filled;
}

void Circle::update(SDL_Plotter* g, Game* game) {

}

void Circle::draw(SDL_Plotter* g) {
    drawCircle(g, *this);
}

Image::Image(const string & fileName) {
  load(fileName);
}

void Image::load(const string & fileName) {
  ifstream file(fileName);
  if (!file) {
    throw fileName;
  }
  file >> size.y >> size.x;
  int r, g, b;

  for (int i = 0; i < size.x * size.y; i++) {
    file >> r >> g >> b;
    data.push_back(Color(r, g, b));
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
            Color col = data[r * size.x + c];
            //cout << "(" << (int)col.r << "," << (int)col.g << "," << (int)col.b << ")" << endl;
            plot(g, drawLoc + Vec2i(c, r), data[r * size.x + c]);
        }
    }
}

void Image::update(SDL_Plotter * g, Game* game) {}

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

double Vec2i::dist(const Vec2i& other) {
    return sqrt(pow(x - other.x, 2.0) + pow(y - other.y, 2.0));
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

void TextField::update(SDL_Plotter * g, Game* game) {
    char key = g->getKey();
    if (g->kbhit()) {
        if (key == '<') {
            setText(text.substr(0, text.size() - 1));
            g->clear();
        }
        else if(isalnum(key) || key == ' ' || key == '!') {
            setText(text + key);
            g->clear();
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

void Char::update(SDL_Plotter * g, Game* game) {}


Button::Button(const string & text) : tField(TextField(text)) {
    leave = true;
}

void Button::onHover(const Vec2i & location) {
    if (leave) {
        color = GRAY;
    }
    else {
        color = ORANGE;
    }
}

void Button::onClick(const Vec2i & location, Game* game) {
    game->dave->removePart();
    game->g->clear();
}

void Button::draw(SDL_Plotter * g) {
    drawRectangle(g, Rectangle(location, size, color, true));
}

void Button::update(SDL_Plotter * g, Game* game) {
    Vec2i mouseLoc;
    if (g->getMouseClick(mouseLoc.x, mouseLoc.y)) {
        if (inside(mouseLoc)) {
            onClick(mouseLoc, game);
        }
    }
    if (g->getMouseMotion(mouseLoc.x, mouseLoc.y)) {
        if (inside(mouseLoc)) {
            if (leave != false) {
                leave = false;
            }
        }
        else {
            if (leave != true) {
                leave = true;
            }
        }
        onHover(mouseLoc);
    }
}

void Button::setLocation(const Vec2i & location) {
    Pane::setLocation(location);
    tField.setLocation(location);
}

StickMan::StickMan(const Vec2i& location, const Color& color) {
    body.push_back(new Circle(location, 10, color, false));
    body.push_back(new Line(Vec2i(location.x, location.y + 10), Vec2i(location.x, location.y + 50)));
    body.push_back(new Line(((Line*)body[1])->getP2(), ((Line*)body[1])->getP2() + Vec2i(-18, 24)));
    body.push_back(new Line(((Line*)body[1])->getP2(), ((Line*)body[1])->getP2() + Vec2i(18, 24)));
    body.push_back(new Line(((Line*)body[1])->getP2() + Vec2i(0, -30), ((Line*)body[1])->getP2() + Vec2i(-12, -10)));
    body.push_back(new Line(((Line*)body[1])->getP2() + Vec2i(0, -30), ((Line*)body[1])->getP2() + Vec2i(12, -10)));
}
void StickMan::removePart() {
    if (!body.empty()) {
        body.pop_back();
    }
}
bool StickMan::dead() const {
    return body.empty();
}

void StickMan::update(SDL_Plotter* g, Game* game) {
    for (int i = 0; i < body.size(); i++) {
        body[i]->update(g, game);
    }
}
void StickMan::draw(SDL_Plotter* g) {
    for (int i = 0; i < body.size(); i++) {
        body[i]->draw(g);
    }
}