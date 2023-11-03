#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<fstream>
#include <sstream>

unsigned const WIDTH = 800;
unsigned const HEIGHT = 800;
const int DEPTH = 255;


/*//////////////////////////////////////////////Matrix/////////////////////////////////////////////////////*/
template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t numRows = 4, size_t numCols = 4) : rows(numRows), cols(numCols) {
        data.resize(rows, std::vector<T>(cols));
    }

    Matrix(Matrix<T>& matr) : rows(matr.rows), cols(matr.cols) {
        data.resize(rows, std::vector<T>(cols));
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                data[i][j] = matr[i][j];
            }
        }
    }

    size_t numRows() const {
        return rows;
    }

    size_t numCols() const {
        return cols;
    }

    void identity() {
        if (rows != 0 && cols != 0) {
            size_t minim = rows < cols ? rows : cols;
            for (size_t i = 0; i < minim; ++i)
                data[i][i] = 1;
        }
    }

    std::vector<T>& operator[](size_t rowIndex) {
        if (rowIndex >= rows) {
            std::cerr << "Row index out of range";
        }
        return data[rowIndex];
    }

    Matrix<T> operator*(Matrix<T>& matr) {
        if (&matr == this)
            return std::move(*this);
        if (cols != matr.rows) {
            std::cerr << "Rows != Cols";
        }
        Matrix result(rows, matr.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < matr.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    result[i][j] += data[i][k] * matr[k][j];
                }
            }
        }
        return std::move(result);
    }

    const std::vector<T>& operator[](size_t rowIndex) const {
        if (rowIndex >= rows) {
            std::cerr << "Row index out of range";
            EXIT_FAILURE;
        }
        return data[rowIndex];
    }

    void print() const {
        for (const auto& row : data) {
            for (const auto& element : row) {
                std::cout << element << "\t";
            }
            std::cout << std::endl;
        }
    }
};

template <typename T>
void viewport(Matrix<T>& m, int x, int y, int w, int h) {
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = DEPTH / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = DEPTH / 2.f;
}

template <typename T>
Matrix<T> v2m(sf::Vector3f v) {
    Matrix<float> m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

template <typename T>
sf::Vector3f m2v(Matrix<T> m) {
    return sf::Vector3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

/*//////////////////////////////////////////////////////////////////////////*/
struct Texture
{
    short width;
    short height;
    short bitsperpixel;
    char datatypecode;
    sf::VertexArray vertex_texture;
};

struct Draw
{
    Draw() {
        for (size_t i = 0; i < WIDTH * HEIGHT; ++i)
            Z_bufer[i] = std::numeric_limits<int>::min();
    }
    sf::VertexArray vertex;
    sf::Vector3i s_c[3];
    sf::Vector2i vt[3];
    float intensity;
    std::unique_ptr<int[]> Z_bufer = std::make_unique<int[]>(WIDTH * HEIGHT);
};

struct SidesTriangle
{
    std::vector<sf::Vector3i> sides[3];
    std::vector<sf::Vector2i> textures[3];
};

struct Model
{
    std::vector<sf::Vector3f> v;
    std::vector<sf::Vector2f> vt;
    std::vector<sf::Vector2f> vn;
    std::vector<sf::Vector3i> f[3];
};



