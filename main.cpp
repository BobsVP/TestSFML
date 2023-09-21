//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include<cmath>
#include "Head.h"
#include"config.h"

void triangle(Draw& drw, Model& vert, Texture& tex);
void Parser_file(const std::string file_name, Model& mod);
float norm(const sf::Vector3f& nn);
void ReadTGA(const std::string file_name, Texture& tex);

int main(int argc, char** argv)
{
    std::string file_name;
    if (2 == argc) 
        file_name = argv[1];
    else {
        file_name = Dt_Srcs;
    }
    Model model;
    Parser_file(file_name + "african_head.obj", model);
    
    Texture HeadTexture;
    ReadTGA(file_name + "african_head_diffuse.tga", HeadTexture);
    Draw drawstruct;
    drawstruct.vertex.setPrimitiveType(sf::Points);

    sf::Vector3f light_dir(0, 0, -1);
    sf::Vector3f camera(0, 0, 3);
    for (size_t i = 0; i < model.f[0].size(); ++i) {
        float Width2 = WIDTH / 2.;
        float Height2 = HEIGHT / 2.;
        float Depth2 = DEPTH / 2.;
        sf::Vector3f world_coords[3];
        for (size_t j = 0; j < 3; j++) {
            int tmp = model.f[j][i].x;
            drawstruct.s_c[j] = sf::Vector3i((model.v[tmp].x + 1.) * Width2, (model.v[tmp].y + 1.) * Height2, (model.v[tmp].z + 1.) * Depth2);
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
                int tm = model.f[k][i].y;
                drawstruct.vt[k] = sf::Vector2i(model.vt[tm].x * HeadTexture.width, model.vt[tm].y * HeadTexture.height);
            }
            triangle(drawstruct, model, HeadTexture);
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
        window.draw(drawstruct.vertex);
        window.display();
    }
    return 0;
}
