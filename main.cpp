#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include<cmath>
#include "Head.h"

void triangle(Draw& drw, Model& vert);
void Parser_file(const char* file_name, Model& mod);
float norm(const sf::Vector3f& nn);

int main(int argc, char** argv)
{
    char* file_name;
    if (2 == argc) 
        file_name = argv[1];
    else
        file_name = "..//obj//african_head.obj";
    Model model;
    model.vertex.setPrimitiveType(sf::Points);
    Parser_file(file_name, model);
    
    Draw drawstruct;
    std::ifstream in;
    in.open("..//obj//african_head_diffuse.tga", std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "can't open file african_head_diffuse.tga" << "\n";
        in.close();
        return false;
    }
    int trash;
    sf::Color color_texture;
    unsigned char chunkheader = 0;
    bool flag = 0;
    char datatypecode;
    short width, height, bitsperpixel;
    in.read((char*)&trash, 2);
    in.read((char*)&datatypecode, 1);
    in.read((char*)&trash, 4);
    in.read((char*)&trash, 4);
    in.read((char*)&trash, 1);
    in.read((char*)&width, 2);
    in.read((char*)&height, 2);
    in.read((char*)&bitsperpixel, 2);
    for (size_t i = height; 1 <= i; i--)
    {
        for (size_t j = 1; j <= width; j++)
        {
            if (chunkheader == 0) {
                chunkheader = in.get();
                if (chunkheader < 128) {
                    chunkheader++;
                    flag = 1;
                }
                else {
                    chunkheader -= 127;
                    flag = 0;
                }
                color_texture.b = in.get();
                color_texture.g = in.get();
                color_texture.r = in.get();
            }
            model.vertex_texture.append(sf::Vector2f(j, i));
            model.vertex_texture[model.vertex_texture.getVertexCount() - 1].color = color_texture;
            chunkheader--;
            if (flag && chunkheader) {
                color_texture.b = in.get();
                color_texture.g = in.get();
                color_texture.r = in.get();
            }
        }
    }

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
                drawstruct.vt[k] = sf::Vector2i(model.vt[tm].x * width, model.vt[tm].y * height);
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
