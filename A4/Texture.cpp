#include <cmath>

#include "Texture.hpp"

//-----------------------------------------------------
// Texture base class
//-----------------------------------------------------
Texture* Texture::defaultTexture = new Texture(); 

Texture::Texture() {}

Texture::~Texture() {}

// just return white, should be multiplied with actual colour to get the same colour
glm::vec3 Texture::getColour(double u, double v) 
{
    return glm::vec3(1.0);
}

// Normal doesn't change
glm::vec3 Texture::getNormal(const glm::vec3 & normal, double u, double v)
{
    return normal;
}

//-----------------------------------------------------
// Bitmap Texture class 
//-----------------------------------------------------

BitmapTexture::BitmapTexture()
    : tileU(1.0), tileV(1.0), data(Image())
{
}

BitmapTexture::BitmapTexture(const std::string& filename, double tileU, double tileV)
    : tileU(tileU), tileV(tileV), data(Image(filename))
{
}

BitmapTexture::~BitmapTexture()
{
}

glm::vec3 BitmapTexture::getColour(double u, double v)
{
    // Remember that v is upside down
    double intPart;
    double texU = glm::clamp(u, 0.0, 1.0) * tileU;
    double texV = (1.0 - glm::clamp(v, 0.0, 1.0)) * tileV;
    texU = std::modf(texU, &intPart);
    texV = std::modf(texV, &intPart);

    int indexU = data.width() * texU; 
    int indexV = data.height() * texV; 

    return glm::vec3(data(indexU,indexV,0), data(indexU,indexV,1), data(indexU, indexV, 2));
}

//-----------------------------------------------------
// Bumpmap Texture class 
// TODO: Actually implement
//-----------------------------------------------------

BumpmapTexture::BumpmapTexture()
    : tileU(1.0), tileV(1.0), data(Image())
{
}

BumpmapTexture::BumpmapTexture(const std::string& filename, double tileU, double tileV)
    : tileU(tileU), tileV(tileV), data(Image(filename))
{
}

BumpmapTexture::~BumpmapTexture()
{
}

// TODO: make this return the changed normal
// We'll just use the red channel for the normal and ignore the green and blue.
// It's a huge waste of space, but I don't want to spend time on something that's not gonna be an issue
glm::vec3 BumpmapTexture::getNormal(const glm::vec3 & normal, double u, double v)
{
    // Remember that v is upside down
    double intPart;
    double texU = glm::clamp(u, 0.0, 1.0) * tileU;
    double texV = (1.0 - glm::clamp(v, 0.0, 1.0)) * tileV;
    texU = std::modf(texU, &intPart);
    texV = std::modf(texV, &intPart);

    int indexU = data.width() * texU; 
    int indexV = data.height() * texV; 

    return glm::vec3(data(indexU,indexV,0), data(indexU,indexV,1), data(indexU, indexV, 2));
}
