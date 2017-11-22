#pragma once

#include <string>
#include <glm/ext.hpp>

#include "Image.hpp"

// Class for handling UV mapping
class Texture {

protected:

    glm::mat4 getBumpToWorldMat(const glm::vec3 & normal, const glm::vec3 & upV);

public:

    // construct the Texture from a png file
    Texture();

    virtual ~Texture();

    // Gets the colour at the given 
    virtual glm::vec3 getColour(double u, double v);
    virtual glm::vec3 getNormal(const glm::vec3 & normal, const glm::vec3 & upV, double u, double v);

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

class BumpmapTexture : public Texture {

    double getBumpHeight(int u, int v);

    // How often the texture should repeat along the U and V axis
    double tileU;
    double tileV;

    double depth;

    Image data;

public:

    BumpmapTexture();
    BumpmapTexture(const std::string& filename, double tileU, double tileV, double depth);

    ~BumpmapTexture();

    virtual glm::vec3 getNormal(const glm::vec3 & normal, const glm::vec3 & upV, double u, double v);

};

class NoiseTexture : public Texture {

public:

    NoiseTexture();

    NoiseTexture(uint width, uint height);

    ~NoiseTexture();

    virtual glm::vec3 getColour(double u, double v);

};

class RippleTexture : public Texture {

    // Center of the ripple, between 0 and 1
    double centerU;
    double centerV;

    // Size of the ripple
    double scaleU;
    double scaleV;

    // Amplitude of wave
    double waveHeight;

    double getDerivative(double t);

public:

    RippleTexture();
    RippleTexture(double centerU, double centerV, double scaleU, double scaleV, double waveHeight);

    ~RippleTexture();

    virtual glm::vec3 getNormal(const glm::vec3 & normal, const glm::vec3 & upV, double u, double v);
};