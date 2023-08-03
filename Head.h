#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<fstream>
#include <sstream>

unsigned const WIDTH = 800;
unsigned const HEIGHT = 800;
const int DEPTH = 255;

struct Draw
{
    sf::Vector3i s_c[3];
    sf::Vector2i vt[3];
    sf::Color colr;
    float intensity;
};

struct SidesTriangle
{
    std::vector<sf::Vector3i> sides[3];
    std::vector<sf::Vector2i> textures[3];
};

struct Model
{
    Model() {
        for (size_t i = 0; i < WIDTH * HEIGHT; ++i)
            Z_bufer[i] = std::numeric_limits<int>::min();
    }
    sf::VertexArray vertex;
    sf::VertexArray vertex_texture;
    std::vector<sf::Vector3f> v;
    std::vector<sf::Vector2f> vt;
    std::vector<sf::Vector3i> f;
    std::unique_ptr<int[]> Z_bufer = std::make_unique<int[]>(WIDTH * HEIGHT);
};



