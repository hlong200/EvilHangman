#pragma once

#include "Color.h"
#include <vector>
#include <string>
#include <map>
#include "Vec2.h"

using namespace std;

class SDL_Plotter;

struct Drawable {
    virtual void draw(SDL_Plotter* g) = 0;
    virtual void update(SDL_Plotter* g, Game* game) = 0;
};

void drawLine(SDL_Plotter* g, const Vec2i& p1, const Vec2i& p2, const Color& color);

class Line : public Drawable {
private:
    Vec2i p1, p2;
    Color color;
public:
    Line();
    Line(const Vec2i& p1 = Vec2i(), const Vec2i& p2 = Vec2i());
    Line(int x1, int y1, int x2, int y2);
    Vec2i getP1() const;
    Vec2i getP2() const;
    Color getColor() const;
    void setP1(const Vec2i& other);
    void setP2(const Vec2i& other);
    void setColor(const Color& color);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g, Game* game);
};

class Rectangle : public Drawable {
private:
    Vec2i location;
    Vec2i size;
    Color color;
    bool fill;
public:
    Rectangle();
    Rectangle(const Vec2i& location = Vec2i(), const Vec2i& size = Vec2i(), const Color& color = BLACK, bool filled = true);
    Rectangle(const Vec2i& location = Vec2i(), int width = 0, int height = 0, const Color& color = BLACK, bool filled = true);
    Rectangle(int x = 0, int y = 0, int width = 0, int height = 0, const Color& color = BLACK, bool filled = true);
    Vec2i getLocation() const;
    Vec2i getSize() const;
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    Color getColor() const;
    bool getFilled() const;
    void setFilled(bool filled);
    void setColor(const Color& color);
    void setLocation(const Vec2i& location);
    void setSize(const Vec2i& size);
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void update(SDL_Plotter* g, Game* game);
    void draw(SDL_Plotter* g);
};

void drawRectangle(SDL_Plotter* g, const Rectangle& rectangle);

class Circle : public Drawable {
private:
    Vec2i location;
    int radius;
    Color color;
    bool fill;
public:
    Circle(const Vec2i& location = Vec2i(), int radius = 0, const Color& color = BLACK, bool filled = false);
    Vec2i getLocation() const;
    int getRadius() const;
    Color getColor() const;
    bool getFilled() const;
    void setLocation(const Vec2i& location);
    void setRadius(int radius);
    void setColor(const Color& color);
    void setFilled(bool filled);
    void update(SDL_Plotter* g, Game* game);
    void draw(SDL_Plotter* g);
};

void drawCircle(SDL_Plotter* g, const Circle& circle);

class Pane : public Drawable {
protected:
    Pane * parent;
    Vec2i location, offset, size;
public:
    Pane(Pane* parent = nullptr, Vec2i offset = Vec2i());
    Pane* getParent() const;
    Vec2i getLocation() const;
    Vec2i getOffset() const;
    Vec2i getSize() const;
    bool inside(const Vec2i& location);
    void setParent(Pane* parent);
    void setLocation(const Vec2i& location);
    void setOffset(const Vec2i& offset);
    void setSize(const Vec2i& size);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g, Game* game);
};

const string RES_IMG = string(SDL_GetBasePath()) + "res\\img\\";

class Image : public Pane {
private:
    vector<Color> data;
public:
    Image(const string& fileName);
    void load(const string& fileName);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g, Game* game);
};

const string RES_CHAR = string(SDL_GetBasePath()) + "res\\char\\";

class Char : public Pane {
private:
    char id;
    vector<uint8_t> data;
    unsigned short size;
    unsigned short scale;
    Color color;
public:
    Char();
    Char(char id, unsigned short size);
    char getId() const;
    void loadChar(const string& fileName);
    void setColor(const Color& color);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g, Game* game);
};

const short CHAR_SIZE = 16;

map<char, Char> letters;

class TextField : public Pane {
private:
    string text;
    vector<Char> field;
    bool canEdit;
    Color fg;
    //Color bg;
public:
    TextField();
    TextField(const string& text);
    string getText() const;
    bool getCanEdit() const;
    void setText(const string& text);
    void setCanEdit(bool b);
    void setFgColor(const Color& color);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g, Game* game);
};

class StickMan : public Drawable {
private:
    vector<Drawable *> body;
public:
    StickMan(const Vec2i& location = Vec2i(), const Color& color = BLACK);
    void removePart();
    bool dead() const;
    void update(SDL_Plotter* g, Game* game);
    void draw(SDL_Plotter* g);
};

class Button : public Pane {
private:
    TextField tField;
    Color color;
    bool leave;
public:
    Button(const string& text);
    void onHover(const Vec2i& location);
    void onClick(const Vec2i& location, Game* game);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g, Game* game);
    void setLocation(const Vec2i& location);
};