#pragma once

#include <string>
#include <glm/ext.hpp>

#include "Image.hpp"

// Class for handling UV mapping
class Texture {

public:

    // construct the Texture from a png file
    Texture();

    virtual ~Texture();

    // Gets the colour at the given 
    virtual glm::vec3 getColour(double u, double v);
    virtual glm::vec3 getNormal(const glm::vec3 & normal, double u, double v);

    // Used as the default texture, it affects no change in colour or normal;
    static Texture* defaultTexture; 

};

class BitmapTexture : public Texture {

    // How often the texture should repeat along the U and V axis
    double tileU;
    double tileV;

    Image data;

public:

    BitmapTexture();
    BitmapTexture(const std::string& filename, double tileU, double tileV);

    ~BitmapTexture();

    virtual glm::vec3 getColour(double u, double v);

};

// TODO: actually implement
class BumpmapTexture : public Texture {

    // How often the texture should repeat along the U and V axis
    double tileU;
    double tileV;

    Image data;

public:

    BumpmapTexture();
    BumpmapTexture(const std::string& filename, double tileU, double tileV);

    ~BumpmapTexture();

    virtual glm::vec3 getNormal(const glm::vec3 & normal, double u, double v);

};