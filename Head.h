#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<fstream>
#include <sstream>

unsigned const WIDTH = 800;
unsigned const HEIGHT = 800;
const int DEPTH = 255;

struct Texture
{
    short width;
    short height;
    short bitsperpixel;
    char datatypecode;
    sf::VertexArray vertex_texture;
};

struct Draw
{
    Draw() {
        for (size_t i = 0; i < WIDTH * HEIGHT; ++i)
            Z_bufer[i] = std::numeric_limits<int>::min();
    }
    sf::VertexArray vertex;
    sf::Vector3i s_c[3];
    sf::Vector2i vt[3];
    float intensity;
    std::unique_ptr<int[]> Z_bufer = std::make_unique<int[]>(WIDTH * HEIGHT);
};

struct SidesTriangle
{
    std::vector<sf::Vector3i> sides[3];
    std::vector<sf::Vector2i> textures[3];
};

struct Model
{
    std::vector<sf::Vector3f> v;
    std::vector<sf::Vector2f> vt;
    std::vector<sf::Vector2f> vn;
    std::vector<sf::Vector3i> f[3];
};



