#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
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
    std::unique_ptr<int[]> Z_bufer = std::make_unique<int[]> (WIDTH * HEIGHT);
};

void line2(Draw& drw, SidesTriangle& trg, Model& vert) {
    
    for (size_t i = 0; i < trg.sides[2].size(); ++i) {
        sf::Vector3i A = trg.sides[2].operator[](i);
        sf::Vector2i Atx = trg.textures[2].operator[](i);
        sf::Vector3i B;
        sf::Vector2i Btx;
        if (i < trg.sides[0].size()) {
            B = trg.sides[0].operator[](i);
            Btx = trg.textures[0].operator[](i);
        }
        else {
            B = trg.sides[1].operator[](i - trg.sides[0].size() + 1);
            Btx = trg.textures[1].operator[](i - trg.sides[0].size() + 1);
        }
        if (A.y == B.y)
        {
            if (A.x > B.x) { 
                std::swap(B, A);
                std::swap(Btx, Atx);
            }
            for (size_t j = A.x; j <= B.x; j++)
            {
                float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
                sf::Vector3i P = A + sf::Vector3i((B.x - A.x) * phi + .5, (B.y - A.y) * phi + .5, (B.z - A.z) * phi + .5);
                sf::Vector2i Ptx = Atx + sf::Vector2i((Btx.x - Atx.x) * phi + .5, (Btx.y - Atx.y) * phi + .5);
                if (P.z > vert.Z_bufer[P.x * WIDTH + P.y]) {
                    vert.Z_bufer[P.x * WIDTH + P.y] = P.z;
                    vert.vertex.append(sf::Vector2f(P.x, HEIGHT - P.y));
                    drw.colr = vert.vertex_texture.operator[](Ptx.x + 1024 * Ptx.y).color;
                    drw.colr = sf::Color(drw.colr.r * drw.intensity, drw.colr.g * drw.intensity, drw.colr.b * drw.intensity);
                    vert.vertex[vert.vertex.getVertexCount() - 1].color = drw.colr;
                }
            }
        }
    }
}

void line1(Draw& drw, SidesTriangle& trg) {
    for (size_t i = 0; i < 3; i++) {
        int x0 = i, x1 = (i + 1) % 3;
        if (drw.s_c[x0].y > drw.s_c[x1].y) std::swap(x0, x1);
        int total_height =  drw.s_c[x1].y - drw.s_c[x0].y;
        for (size_t x = 0; x <= total_height; x++) {
            float alpha = total_height ? (float)x / total_height : 0.0;
            sf::Vector3i A((drw.s_c[x1].x - drw.s_c[x0].x) * alpha + .5, (drw.s_c[x1].y - drw.s_c[x0].y) * alpha + .5, (drw.s_c[x1].z - drw.s_c[x0].z) * alpha + .5);
            sf::Vector2i B((drw.vt[x1].x - drw.vt[x0].x) * alpha + .5, (drw.vt[x1].y - drw.vt[x0].y) * alpha + .5);
            trg.sides[i].push_back(drw.s_c[x0] + A);
            trg.textures[i].push_back(drw.vt[x0] + B);
        }
    }
}

void triangle(Draw& drw, Model& vert) {
    if (drw.s_c[0].y > drw.s_c[1].y) { std::swap(drw.s_c[0], drw.s_c[1]); std::swap(drw.vt[0], drw.vt[1]); }
    if (drw.s_c[0].y > drw.s_c[2].y) { std::swap(drw.s_c[0], drw.s_c[2]); std::swap(drw.vt[0], drw.vt[2]); }
    if (drw.s_c[1].y > drw.s_c[2].y) { std::swap(drw.s_c[1], drw.s_c[2]); std::swap(drw.vt[1], drw.vt[2]); }

    SidesTriangle Trg;
    line1(drw, Trg);
    line2(drw, Trg, vert);

}

void Parser_file(const char* file_name, Model& mod)
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
            mod.v.push_back(z); 
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            sf::Vector2f uv;
            iss >> uv.x >> uv.y;
            mod.vt.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            sf::Vector3i idx;
            iss >> trash;
            while (iss >> idx.x >> trash >> idx.y >> trash >> idx.z) {
                --idx.x; --idx.y; --idx.z;
                mod.f.push_back(idx);
            }
        }
    }
    in.close();
}

float norm(const sf::Vector3f& nn)  { return std::sqrt(nn.x * nn.x + nn.y * nn.y + nn.z * nn.z); }

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
