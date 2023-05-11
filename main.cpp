#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<fstream>
#include <sstream>

unsigned const WIDTH = 800;
unsigned const HEIGHT = 800;
struct Model
{
     sf::VertexArray vertex;
     std::vector<sf::Vector3f> v;
     std::vector<int> f;
};

void line2(sf::Vector2f x0, sf::Vector2f x1, sf::VertexArray& vert, const sf::Color& colr) {
    if ((x1.y - x0.y) == 0)
    {
        if (x0.x > x1.x) std::swap(x0, x1);
        int y = x0.y;
        for (int x = x0.x; x <= x1.x; x++)
        {
            vert.append(sf::Vector2f(x, y));
            vert[vert.getVertexCount() - 1].color = colr;
        }
    }
}

void line1(sf::Vector2i x0, sf::Vector2i x1, sf::VertexArray& vert, const sf::Color& colr) {
    if (x0.y > x1.y) std::swap(x0, x1);
    float t = (x1.x - x0.x) / (float)(x1.y - x0.y);
    float dt = 0;
    for (int x = x0.y; x <= x1.y; x++) {
        int y = x0.x + dt;
        dt += t;
        vert.append(sf::Vector2f(y, HEIGHT - x));
        vert[vert.getVertexCount() - 1].color = colr;
    }
}

void triangle(sf::Vector2i t0, sf::Vector2i t1, sf::Vector2i t2, sf::VertexArray& vert, const sf::Color& colr) {
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    sf::VertexArray vertexTriangle1(sf::Points);
    sf::VertexArray vertexTriangle2(sf::Points);
    sf::VertexArray vertexTriangle3(sf::Points);

    line1(t0, t1, vertexTriangle1, colr);
    line1(t1, t2, vertexTriangle2, colr);
    line1(t2, t0, vertexTriangle3, colr);

    for (int i = 0; i < t1.y - t0.y; ++i) {
        line2(vertexTriangle1.operator[](i).position, vertexTriangle3.operator[](i).position, vert, colr);
    }
    for (int i = t2.y - t0.y; i >= t1.y - t0.y; --i) {
        line2(vertexTriangle2.operator[](i - t1.y + t0.y).position, vertexTriangle3.operator[](i).position, vert, colr);
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

float norm(const sf::Vector3f& nn)  { return std::sqrt(nn.x * nn.x + nn.y * nn.y + nn.z * nn.z); }

int main()
{
    Model model;
    model.vertex.setPrimitiveType(sf::Points);
    char* file_name = "..//obj//african_head.obj";
    Parser_file(file_name, model.v, model.f);
    sf::Vector3f light_dir(0, 0, -1);
    for (int i = 0; i < model.f.size() - 4; i += 3) {
        sf::Vector2i screen_coords[3];
        sf::Vector3f world_coords[3];
        for (int j = 0; j < 3; j++) {
            int tmp = model.f[i + j];
            screen_coords[j] = sf::Vector2i((model.v[tmp].x + 1.) * WIDTH / 2., (model.v[tmp].y + 1.) * HEIGHT / 2.);
            world_coords[j] = model.v[tmp];
        }
        sf::Vector3f V1 = world_coords[2] - world_coords[0];
        sf::Vector3f V2 = world_coords[1] - world_coords[0];
        sf::Vector3f n = { V1.y * V2.z - V1.z * V2.y, V1.z * V2.x - V1.x * V2.z, V1.x * V2.y - V1.y * V2.x };
        float intensity = norm(n);
        n = { n.x * (1 / norm(n)), n.y * (1 / norm(n)), n.z * (1 / norm(n)) };
        intensity = n.x * light_dir.x + n.y * light_dir.y + n.z * light_dir.z;
        if (intensity > 0)
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], model.vertex, sf::Color::Color(intensity * 255, intensity * 255, intensity * 255, 255));
    }
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "simplified OpenGL");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(model.vertex);
        window.display();
    }
    return 0;
}
