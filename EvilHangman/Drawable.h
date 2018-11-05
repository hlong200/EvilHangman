#pragma once

#include "Vec2.h"
#include "Color.h"
#include <vector>
#include <string>
#include <map>
#include <filesystem>

using namespace std;

class SDL_Plotter;

struct Drawable {
    virtual void draw(SDL_Plotter* g) = 0;
    virtual void update(SDL_Plotter* g) = 0;
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
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g);
};

// TODO: Implement filled AND unfilled rectangle drawing...
void drawRectangle(SDL_Plotter* g, const Vec2i& location, const Vec2i& size, const Color& color, bool fill = false);

class Rectangle : public Drawable {
private:
    Vec2i location;
    Vec2i size;
    Color color;
    bool fill;
public:
    Rectangle();
    Rectangle(const Vec2i& location = Vec2i(), const Vec2i& size = Vec2i());
    Rectangle(const Vec2i& location = Vec2i(), int width = 0, int height = 0);
    Rectangle(int x = 0, int y = 0, int width = 0, int height = 0);
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
    void update(SDL_Plotter* g);
    void draw(SDL_Plotter* g);
};

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
    void update(SDL_Plotter* g);
};

class Image : public Pane {
private:
    vector<Color> data;
public:
    Image(const string& fileName);
    void load(const string& fileName);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g);
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
    void update(SDL_Plotter* g);
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
    void update(SDL_Plotter* g);
};

class Button : public Pane {
private:
    TextField tField;
    Color color;
public:
    Button(const string& text);
    void onHover(const Vec2i& location);
    void onClick(const Vec2i& location);
    void draw(SDL_Plotter* g);
    void update(SDL_Plotter* g);
    void setLocation(const Vec2i& location);
};