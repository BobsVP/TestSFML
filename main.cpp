#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include<cmath>

unsigned const WIGHT = 800;
unsigned const HEIGHT = 600;

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

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIGHT, HEIGHT), "simplified OpenGL");
    sf::VertexArray vertex(sf::Points);
    line(10, 10, 300, 500, vertex, sf::Color::White);
    line(300, 500, 600, 10, vertex, sf::Color::Red);
    line(10, 10, 600, 10, vertex, sf::Color::Red);

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
