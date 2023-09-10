//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include<cmath>
#include "Head.h"
#include"config.h"

void triangle(Draw& drw, Model& vert);
void Parser_file(const std::string file_name, Model& mod);
float norm(const sf::Vector3f& nn);
void ReadTGA(const std::string file_name, Model& mod);

int main(int argc, char** argv)
{
    std::string file_name;
    if (2 == argc) 
        file_name = argv[1];
    else {
        file_name = Dt_Srcs;
    }
    Model model;
    model.vertex.setPrimitiveType(sf::Points);
    Parser_file(file_name + "african_head.obj", model);
    
    ReadTGA(file_name + "african_head_diffuse.tga", model);
    Draw drawstruct;

    sf::Vector3f light_dir(0, 0, -1);
    for (size_t i = 0; i < model.f.size() - 4; i += 3) {
        sf::Vector3f world_coords[3];
        for (size_t j = 0; j < 3; j++) {
            int tmp = model.f[i + j].x;
            drawstruct.s_c[j] = sf::Vector3i((model.v[tmp].x + 1.) * WIDTH / 2., (model.v[tmp].y + 1.) * HEIGHT / 2., (model.v[tmp].z + 1.) * DEPTH / 2.);
            world_coords[j] = model.v[tmp];
        }
        sf::Vector3f V1 = world_coords[2] - world_coords[0];
        sf::Vector3f V2 = world_coords[1] - world_coords[0];
        sf::Vector3f n = { V1.y * V2.z - V1.z * V2.y, V1.z * V2.x - V1.x * V2.z, V1.x * V2.y - V1.y * V2.x };
        drawstruct.intensity = norm(n);
        n = { n.x * (1 / norm(n)), n.y * (1 / norm(n)), n.z * (1 / norm(n)) };
        drawstruct.intensity = n.x * light_dir.x + n.y * light_dir.y + n.z * light_dir.z;
        if (drawstruct.intensity > 0) {
            for (size_t k = 0; k < 3; k++) {
                int tm = model.f[i + k].y;
                drawstruct.vt[k] = sf::Vector2i(model.vt[tm].x * 1024, model.vt[tm].y * 1024);
            }
            triangle(drawstruct, model);
        }
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
