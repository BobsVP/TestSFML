#include "Head.h"


void line2(Draw& drw, SidesTriangle& trg, Model& vert, Texture& tex) {        //заполняем треугольник
    
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
            sf::Color colr;
            for (size_t j = A.x; j <= B.x; j++)
            {
                float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
                sf::Vector3i P = A + sf::Vector3i((B.x - A.x) * phi + .5, (B.y - A.y) * phi + .5, (B.z - A.z) * phi + .5);
                sf::Vector2i Ptx = Atx + sf::Vector2i((Btx.x - Atx.x) * phi + .5, (Btx.y - Atx.y) * phi + .5);
                if (P.z > drw.Z_bufer[P.x * WIDTH + P.y]) {
                    drw.Z_bufer[P.x * WIDTH + P.y] = P.z;
                    drw.vertex.append(sf::Vector2f(P.x, HEIGHT - P.y));
                    colr = tex.vertex_texture.operator[](Ptx.x + tex.width * Ptx.y).color;
                    colr = sf::Color(colr.r * drw.intensity, colr.g * drw.intensity, colr.b * drw.intensity);
                    drw.vertex[drw.vertex.getVertexCount() - 1].color = colr;
                }
            }
        }
    }
}

void line1(Draw& drw, SidesTriangle& trg) {     //определяем границы треугольника
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

void triangle(Draw& drw, Model& vert, Texture& tex) {
    if (drw.s_c[0].y > drw.s_c[1].y) { std::swap(drw.s_c[0], drw.s_c[1]); std::swap(drw.vt[0], drw.vt[1]); }
    if (drw.s_c[0].y > drw.s_c[2].y) { std::swap(drw.s_c[0], drw.s_c[2]); std::swap(drw.vt[0], drw.vt[2]); }
    if (drw.s_c[1].y > drw.s_c[2].y) { std::swap(drw.s_c[1], drw.s_c[2]); std::swap(drw.vt[1], drw.vt[2]); }

    SidesTriangle Trg;
    line1(drw, Trg);
    line2(drw, Trg, vert, tex);

}

void Parser_file(const std::string file_name, Model& mod)       //читаем файл модели
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
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            sf::Vector2f un;
            iss >> un.x >> un.y;
            mod.vn.push_back(un);
        }
        else if (!line.compare(0, 2, "f ")) {
            sf::Vector3i idx;
            int x = 0;
            iss >> trash;
            while (iss >> idx.x >> trash >> idx.y >> trash >> idx.z) {
                --idx.x; --idx.y; --idx.z;
                mod.f[x % 3].push_back(idx);
                ++x;
            }
        }
    }
    in.close();
}

float norm(const sf::Vector3f& nn)  { return std::sqrt(nn.x * nn.x + nn.y * nn.y + nn.z * nn.z); }

sf::Vector3i conv3f3i(const sf::Vector3f& vec) {
    sf::Vector3i tmp;
    tmp.x = vec.x + 0.5;
    tmp.y = vec.y + 0.5;
    tmp.z = vec.z + 0.5;
    return tmp;
}


void ReadTGA(const std::string file_name, Texture& tex) {
    std::ifstream in;
    in.open(file_name, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "can't open file african_head_diffuse.tga" << "\n";
        in.close();
        return;
    }
    int trash;
    sf::Color color_texture;
    unsigned char chunkheader = 0;
    bool flag = 0;
    in.read((char*)&trash, 2);
    in.read((char*)&tex.datatypecode, 1);
    in.read((char*)&trash, 4);
    in.read((char*)&trash, 4);
    in.read((char*)&trash, 1);
    in.read((char*)&tex.width, 2);
    in.read((char*)&tex.height, 2);
    in.read((char*)&tex.bitsperpixel, 2);
    for (size_t i = tex.height; 1 <= i; i--)
    {
        for (size_t j = 1; j <= tex.width; j++)
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
            tex.vertex_texture.append(sf::Vector2f(j, i));
            tex.vertex_texture[tex.vertex_texture.getVertexCount() - 1].color = color_texture;
            chunkheader--;
            if (flag && chunkheader) {
                color_texture.b = in.get();
                color_texture.g = in.get();
                color_texture.r = in.get();
            }
        }
    }

}