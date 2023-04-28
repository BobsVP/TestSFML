#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>

void line(unsigned x0, unsigned y0, unsigned x1, unsigned y1, sf::VertexArray& vert, const sf::Color& colr) {
    //assert x0 > x1
    float dy = static_cast<float>(y1 - y0) / (x1 - x0);
    float d = 0.;
    for (; x0 < x1; ++x0) {
        vert.append(sf::Vector2f(x0, y0 + round(d)));
        vert[vert.getVertexCount() - 1].color = colr;
        d += dy;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "simplified OpenGL");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    sf::VertexArray vertex(sf::Points);
    line(0, 0, 800, 800, vertex, sf::Color::Red);
    line(13, 20, 80, 40, vertex, sf::Color::White);
    line(20, 13, 40, 80, vertex, sf::Color::Red);
    line(80, 40, 13, 20, vertex, sf::Color::Red);

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
