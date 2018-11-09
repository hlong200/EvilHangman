#pragma once

struct Vec2f {
    float x, y;
    Vec2f();
    Vec2f(float x, float y);
    Vec2f(int x, int y);
    Vec2f operator+(const Vec2f& other);
    Vec2f& operator+=(const Vec2f& other);
};

struct Vec2i {
    int x, y;
    Vec2i();
    Vec2i(int x, int y);
    Vec2i(float x, float y);
    Vec2i(const Vec2f& other);
    Vec2i operator+(const Vec2i& other);
    Vec2i& operator+=(const Vec2i& other);
    double dist(const Vec2i& other);
};