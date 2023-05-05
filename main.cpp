#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<fstream>
#include <sstream>


unsigned const WIGHT = 800;
unsigned const HEIGHT = 800;

void line(unsigned x0, unsigned y0, unsigned x1, unsigned y1, sf::VertexArray& vert, const sf::Color& colr) {
    if (x0 > WIGHT || x1 > WIGHT)
        return;
    if (y0 > HEIGHT || y1 > HEIGHT)
        return;
    bool steep = false;
    unsigned dx, dy;
    if (x1 > x0)
        dx = x1 - x0;
    else
        dx = x0 - x1;
    if (y1 > y0)
        dy = y1 - y0;
    else
        dy = y0 - y1;
    if (dx < dy) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(dx, dy);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    unsigned derror2 = dx;
    unsigned error2 = 0;
    unsigned y = y0;
    for (unsigned x = x0; x <= x1; x++) {
        if (steep) {
            vert.append(sf::Vector2f(y, HEIGHT - x));
            vert[vert.getVertexCount() - 1].color = colr;
        }
        else
        {
            vert.append(sf::Vector2f(x, HEIGHT - y));
            vert[vert.getVertexCount() - 1].color = colr;
        }
        error2 += dy;

        if (error2 > derror2) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx;
        }
    }
}

void Parser_file(const char* file_name, std::vector<sf::Vector3f>& v_, std::vector<int>& f_)
{
    std::ifstream in;
    in.open(file_name, std::ifstream::in);
    if (in.fail()) { 
        std::cout << "Failed to open file\n";
        exit(EXIT_FAILURE); 
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            sf::Vector3f z;
            iss >> z.x >> z.y >> z.z;
            v_.push_back(z); 
        }
        else if (!line.compare(0, 2, "f ")) {
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; 
                f_.push_back(idx);
            }
        }
    }
    in.close();
}

int main()
{
    sf::VertexArray vertex(sf::Points);

    std::vector<sf::Vector3f> v;
    std::vector<int> f;
    char* file_name = "..//obj//african_head.obj";
    Parser_file(file_name, v, f);
    for (int i = 0; i < f.size() - 1; i += 3) {
        unsigned x0 = (v[f[i]].x + 1.) * WIGHT / 2.;
        unsigned y0 = (v[f[i]].y + 1.) * HEIGHT / 2.;
        unsigned x1 = (v[f[i + 1]].x + 1.) * WIGHT / 2.;
        unsigned y1 = (v[f[i + 1]].y + 1.) * HEIGHT / 2.;
        unsigned x2 = (v[f[i + 2]].x + 1.) * WIGHT / 2.;
        unsigned y2 = (v[f[i + 2]].y + 1.) * HEIGHT / 2.;
        line(x0, y0, x1, y1, vertex, sf::Color::White);
        line(x0, y0, x2, y2, vertex, sf::Color::White);
        line(x2, y2, x1, y1, vertex, sf::Color::White);
    }
    sf::RenderWindow window(sf::VideoMode(WIGHT, HEIGHT), "simplified OpenGL");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(vertex);
        window.display();
    }

    return 0;
}
