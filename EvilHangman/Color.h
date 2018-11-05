#pragma once

#include <cstdint>

struct Color {
    uint8_t r, g, b;
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    Color(int r, int g, int b);
    Color operator+(const Color& other);
    Color& operator+=(const Color& other);
    bool operator==(const Color& other);
};

const Color BLACK(0, 0, 0);
const Color _WHITE(255, 255, 255);
const Color RED(255, 0, 0);
const Color ORANGE(255, 165, 0);
const Color YELLOW(255, 255, 0);
const Color GREEN(0, 100, 0);
const Color BLUE(0, 0, 255);
const Color INDIGO(75, 0, 130);
const Color VIOLET(238, 130, 238);